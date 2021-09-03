/// \file test_shell.cpp
/// \brief Created on 2021-08-31 by Ben

#include <catch2/catch_test_macros.hpp>

#include <hh/shell.hpp>
#include <sstream>

struct mock_serial {
  std::stringstream istream{};
  std::stringstream ostream{};
  void write(const char* s, std::size_t count)
  {
      ostream.write(s, count);
  }
  void flush()
  {
      ostream.flush();
  }
  int get()
  {
      return istream.get();
  }
};

using shell_t = hh::cli::shell<mock_serial, 128>;

struct shell_test_t : public shell_t {
  using shell_t::shell_t;
  void notify() { notify_rx(); }
  void connect() { notify_connected(); }
};

TEST_CASE("shell echo alpha numeric", "[shell][echo]")
{
    mock_serial serial;
    shell_test_t shell{serial};
    std::string str{"hello world 42!"};

    serial.istream << str;
    shell.notify();

    CHECK(serial.ostream.str()==str);
    CHECK(shell.current_line()==str);
}

TEST_CASE("shell newline", "[shell]")
{
    mock_serial serial;
    shell_test_t shell{serial};
    std::string str{"hello world 42!\n"};

    serial.istream << str;
    shell.notify();

    CHECK(serial.ostream.str()=="hello world 42!\n\r>");
    CHECK(shell.current_line()==std::string{""});
}

TEST_CASE("shell backspace", "[shell]")
{
    mock_serial serial;
    shell_test_t shell{serial};
    std::string str{"hello world 42!\b\b\b\b"};

    serial.istream << str;
    shell.notify();

    CHECK(serial.ostream.str()=="hello world 42!"
                                "\x1b[s\x1b[2K>hello world 42\x1b[u\x1b[1D"
                                "\x1b[s\x1b[2K>hello world 4\x1b[u\x1b[1D"
                                "\x1b[s\x1b[2K>hello world \x1b[u\x1b[1D"
                                "\x1b[s\x1b[2K>hello world\x1b[u\x1b[1D");
    CHECK(shell.current_line()==std::string{"hello world"});
}

TEST_CASE("shell backspace overflow", "[shell]")
{
    mock_serial serial;
    shell_test_t shell{serial};
    std::string str{"hi!\b\b\b\bhi!"};

    serial.istream << str;
    shell.notify();

    CHECK(serial.ostream.str()=="hi!"
                                "\x1b[s\x1b[2K>hi\x1b[u\x1b[1D"
                                "\x1b[s\x1b[2K>h\x1b[u\x1b[1D"
                                "\x1b[s\x1b[2K>\x1b[u\x1b[1D"
                                "hi!");
    CHECK(shell.current_line()==std::string{"hi!"});
}

TEST_CASE("shell move cursor and backspace", "[shell]")
{
    mock_serial serial;
    shell_test_t shell{serial};
    std::string str{"hello world 42!\x1b[1D\b"};

    serial.istream << str;
    shell.notify();

    CHECK(serial.ostream.str()=="hello world 42!\x1b[1D"
                                "\x1b[s\x1b[2K>hello world 4!\x1b[u\x1b[1D");
    CHECK(shell.current_line()==std::string{"hello world 4!"});

}
