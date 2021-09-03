/// \file test_cmd_history.cpp
/// \brief Created on 2021-09-01 by Ben

#include <catch2/catch_test_macros.hpp>
#include <hh/cmd_history.hpp>
#include <string>

using namespace std::string_literals;

TEST_CASE("cmd push_back")
{
    hh::shell::cmd_history<1, 10> history{};

    history.push_back('a');
    history.push_back('b');


    CHECK(history.size()==1);

    auto back = history.back();
    auto front = history.front();

    CHECK(back=="ab"s);
    CHECK(front=="ab"s);
}

TEST_CASE("cmd push_back on full buffer")
{
    hh::shell::cmd_history<1, 2> history{};

    history.push_back('a');
    history.push_back('b');
    history.push_back('c');

    auto back = history.back();
    auto front = history.front();

    CHECK(back=="ab"s);
    CHECK(front=="ab"s);
}

TEST_CASE("multiple cmds"){
    hh::shell::cmd_history<2, 10> history{};
    history.push_back("cmd1");
    history.end_string();
    history.push_back("cmd2");

    CHECK(history.size() == 2);

    CHECK(history.front() == "cmd1"s);
    CHECK(history.back() == "cmd2"s);
}

TEST_CASE("cmd overwrite"){
    hh::shell::cmd_history<2, 5> history{};
    history.push_back("cmd1");
    history.end_string();
    history.push_back("cmd2");
    history.end_string();
    history.push_back("cmd3");

    CHECK(history.size() == 2);

    CHECK(history.front() == "cmd2"s);
    CHECK(history.back() == "cmd3"s);
}

TEST_CASE("cmd iterator"){
    hh::shell::cmd_history<3, 5> history{};
    history.push_back("cmd1");
    history.end_string();
    history.push_back("cmd2");
    history.end_string();
    history.push_back("cmd3");

    REQUIRE(history.size() == 3);

    auto it = history.begin();
    REQUIRE(*it == "cmd1"s);
    ++it;
    REQUIRE(*it == "cmd2"s);
    ++it;
    REQUIRE(*it == "cmd3"s);
    ++it;
    REQUIRE(it == history.end());
}