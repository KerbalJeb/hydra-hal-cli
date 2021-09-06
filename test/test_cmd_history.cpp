/// \file test_cmd_history.cpp
/// \brief Created on 2021-09-01 by Ben

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <hh/cmd_history.hpp>
#include <string>

using namespace std::string_literals;

SCENARIO("multiple lines can be stored, accessed and iterated over") {
    GIVEN("a default constructed history") {
        hh::shell::cmd_history<2, 64> history{};
        CHECK(history.size() == 0);
        CHECK(history.empty());

        WHEN("push back is called with a string") {
            history.push_back("cmd1");
            THEN("the string is stored in the history") {
                CHECK(history.size() == 1);
                CHECK(history.front() == "cmd1");
                CHECK(history.back() == "cmd1");
            }
            AND_WHEN("push back is called again") {
                history.push_back("cmd2");
                THEN("the second line has been append to the history") {
                    CHECK(history.size() == 2);
                    CHECK(history.front() == "cmd1");
                    CHECK(history.back() == "cmd2");
                }
            }
        }
    }

    GIVEN("a full history") {
        hh::shell::cmd_history<4, 4> history;
        int offset = GENERATE(0, 1, 2, 3, 4);
        CAPTURE(offset);

        for (int i = 0; i < offset; ++i) {
            auto s = "cmd"s;
            s += (char)('a' + i);
            history.push_back(s.c_str());
        }

        std::vector<std::string> cmds{"cmd1", "cmd2", "cmd3", "cmd4"};
        for (const auto &s : cmds) { history.push_back(s.c_str()); }
        std::vector<std::string> foundCmds{};
        for (auto cmd : history) { foundCmds.emplace_back(cmd.data()); }

        CHECK(foundCmds == cmds);
        CHECK(history.size() == 4);

        WHEN("push back is called with command not stored in the buffer") {
            history.push_back("cmd5");

            THEN("the command is added to the history and the oldest command is overwritten") {
                std::vector<std::string> expectedCmds{"cmd2", "cmd3", "cmd4", "cmd5"};
                std::vector<std::string> actualCmds{};
                for (auto cmd : history) { actualCmds.emplace_back(cmd.data()); }
                CHECK(actualCmds == expectedCmds);
                CHECK(history.back() == "cmd5");
                CHECK(history.front() == "cmd2");
            }
        }

        WHEN("push back is called with command stored in the buffer") {
            auto [s, expected] = GENERATE(
                                         std::tuple{"cmd1", std::vector{"cmd2"s, "cmd3"s, "cmd4"s, "cmd1"s}},
                                         std::tuple{"cmd2", std::vector{"cmd1"s, "cmd3"s, "cmd4"s, "cmd2"s}},
                                         std::tuple{"cmd3", std::vector{"cmd1"s, "cmd2"s, "cmd4"s, "cmd3"s}},
                                         std::tuple{"cmd4", std::vector{"cmd1"s, "cmd2"s, "cmd3"s, "cmd4"s}}
                                         );

            history.push_back(s);
            CAPTURE(s);
            THEN("the command is moved to the most recent position in the command history") {
                std::vector<std::string> actualCmds{};
                for (auto cmd : history) { actualCmds.emplace_back(cmd.data()); }
                CHECK(actualCmds == expected);
            }

        }

    }
}