/// \file test_shell.cpp
/// \brief Created on 2021-08-31 by Ben

#include <catch2/catch_test_macros.hpp>

#include <hh/shell.hpp>
#include <sstream>

using namespace std::string_literals;

struct mock_serial {
    std::stringstream istream{};
    std::stringstream ostream{};
    void write(const char *s, std::size_t count) {
        ostream.write(s, count);
    }
    void flush() {
        ostream.flush();
    }
    int get() {
        return istream.get();
    }
};

using shell_t = hh::shell::shell<mock_serial, 10, 64>;

struct shell_test_t : public shell_t {
    using shell_t::shell_t;
    void notify() { notify_rx(); }
    void connect() { notify_connected(); }
};

TEST_CASE("inputted alpha numeric chars are echoed to serial output", "[shell][echo]") {
    mock_serial serial;
    shell_test_t shell{serial};
    std::string str{"hello world 42!"};

    serial.istream << str;
    shell.notify();

    CHECK(serial.ostream.str() == str);
    CHECK(shell.current_line() == str);
}

TEST_CASE("newline char on non empty line outputs NL-CR and clears current line", "[shell]") {
    mock_serial serial;
    shell_test_t shell{serial};
    std::string str{"hello world 42!\n"};

    serial.istream << str;
    shell.notify();

    CHECK(serial.ostream.str() == "hello world 42!\n\r>");
    CHECK(shell.current_line() == std::string{""});
}

TEST_CASE("backspace on non empty line deletes last char", "[shell]") {
    mock_serial serial;
    shell_test_t shell{serial};
    std::string str{"hello world 42!\b\b\b\b"};

    serial.istream << str;
    shell.notify();

    CHECK(serial.ostream.str() == "hello world 42!"
                                  "\x1b[s\x1b[2K>hello world 42\x1b[u\x1b[1D"
                                  "\x1b[s\x1b[2K>hello world 4\x1b[u\x1b[1D"
                                  "\x1b[s\x1b[2K>hello world \x1b[u\x1b[1D"
                                  "\x1b[s\x1b[2K>hello world\x1b[u\x1b[1D");
    CHECK(shell.current_line() == std::string{"hello world"});
}

TEST_CASE("backspace on empty line has no effect", "[shell]") {
    mock_serial serial;
    shell_test_t shell{serial};
    std::string str{"hi!\b\b\b\bhi!"};

    serial.istream << str;
    shell.notify();

    CHECK(serial.ostream.str() == "hi!"
                                  "\x1b[s\x1b[2K>hi\x1b[u\x1b[1D"
                                  "\x1b[s\x1b[2K>h\x1b[u\x1b[1D"
                                  "\x1b[s\x1b[2K>\x1b[u\x1b[1D"
                                  "hi!");
    CHECK(shell.current_line() == std::string{"hi!"});
}

TEST_CASE("left ansi code followed by backspace deletes second to last char", "[shell]") {
    mock_serial serial;
    shell_test_t shell{serial};
    std::string str{"hello world 42!\x1b[1D\b"};

    serial.istream << str;
    shell.notify();

    CHECK(serial.ostream.str() == "hello world 42!\x1b[1D"
                                  "\x1b[s\x1b[2K>hello world 4!\x1b[u\x1b[1D");
    CHECK(shell.current_line() == std::string{"hello world 4!"});
}

SCENARIO("up and down ansi codes allow next a previous commands to be accessed and modified", "[shell][history") {
    GIVEN("A shell with a non empty command history") {
        mock_serial serial;
        shell_test_t shell{serial};

        auto up_code = "\x1b[1A"s;
        auto down_code = "\x1b[1B"s;

        std::string cmd1{"cmd1"};
        std::string cmd2{"cmd2"};
        std::string cmd3{"cmd3"};
        std::string cmd4{"cmd4"};

        serial.istream << cmd1 << '\n'
                       << cmd2 << '\n'
                       << cmd3 << '\n'
                       << cmd4 << '\n';
        shell.notify();
        serial.ostream = std::stringstream{};
        serial.istream.clear();

        WHEN("a up ansi code is received") {
            serial.istream << up_code;
            shell.notify();

            THEN("the previous command is displayed") {
                CHECK(serial.ostream.str() == "\x1B[2K>cmd4");
                CHECK(shell.current_line() == cmd4);
            }

            AND_WHEN("the command is modified") {
                THEN("the modified command is appended to the history") {
                }
            }
        }

        WHEN("two up ansi codes are received") {
            serial.istream << up_code << up_code;
            shell.notify();

            THEN("the second to most recent command is displayed") {
                CHECK(serial.ostream.str() == "\x1B[2K>cmd4\x1B[2K>cmd3");
                CHECK(shell.current_line() == cmd3);
            }

            WHEN("a down ansi code is received") {
                serial.istream.clear();
                serial.istream << down_code;
                shell.notify();
                THEN("the previous command is displayed") {
                    CHECK(serial.ostream.str() == "\x1B[2K>cmd4\x1B[2K>cmd3\x1B[2K>cmd4");
                    CHECK(shell.current_line() == cmd4);
                }
            }
        }

        WHEN("many up ansi codes are received") {
            serial.istream << up_code << up_code << up_code << up_code << up_code;
            shell.notify();

            THEN("the oldest stored command is displayed") {
                CHECK(serial.ostream.str() == "\x1B[2K>cmd4\x1B[2K>cmd3\x1B[2K>cmd2\x1B[2K>cmd1");
                CHECK(shell.current_line() == cmd1);
            }

            AND_WHEN("a down ansi code is received") {
                serial.istream.clear();
                serial.istream << down_code;
                shell.notify();

                THEN("the second oldest stored command is displayed") {
                    CHECK(serial.ostream.str() == "\x1B[2K>cmd4\x1B[2K>cmd3\x1B[2K>cmd2\x1B[2K>cmd1\x1B[2K>cmd2");
                    CHECK(shell.current_line() == cmd2);
                }
            }
        }

        WHEN("a down ansi code is received") {
            serial.istream << down_code;
            shell.notify();
            THEN("nothing changes") {
                CHECK(serial.ostream.str() == "");
                CHECK(shell.current_line().empty());
            }
            AND_WHEN("a up ansi code is received") {
                serial.istream.clear();
                serial.istream << up_code;
                shell.notify();
                THEN("the previous command is displayed") {
                    CHECK(serial.ostream.str() == "\x1B[2K>cmd4");
                    CHECK(shell.current_line() == cmd4);
                }
            }
        }

        // todo modification and command entered
        // todo repeated command
    }
}