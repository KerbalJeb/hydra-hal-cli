/// \file test_cmd_history.cpp
/// \brief Created on 2021-09-01 by Ben

#include <catch2/catch_test_macros.hpp>
#include <hh/cmd_history.hpp>
#include <string>

using namespace std::string_literals;

TEST_CASE("push_back on not full buffer adds char")
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

TEST_CASE("push_back on full line does nothing")
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

TEST_CASE("push_cmd_back after non empty line results starts new command")
{
    hh::shell::cmd_history<2, 10> history{};
    history.push_back("cmd1");
    history.push_cmd_back();
    history.push_back("cmd2");

    CHECK(history.size()==2);

    CHECK(history.front()=="cmd1"s);
    CHECK(history.back()=="cmd2"s);
}

TEST_CASE("push_cmd_back on full buffer overrides oldest command")
{
    hh::shell::cmd_history<2, 5> history{};
    history.push_back("cmd1");
    history.push_cmd_back();
    history.push_back("cmd2");
    history.push_cmd_back();
    history.push_back("cmd3");

    CHECK(history.size()==2);

    CHECK(history.front()=="cmd2"s);
    CHECK(history.back()=="cmd3"s);
}

TEST_CASE("incrementing begin on non empty buffer iterates over all stored strings")
{
    hh::shell::cmd_history<3, 5> history{};
    history.push_back("cmd1");
    history.push_cmd_back();
    history.push_back("cmd2");
    history.push_cmd_back();
    history.push_back("cmd3");

    REQUIRE(history.size()==3);

    auto it = history.begin();
    REQUIRE(*it=="cmd1"s);
    REQUIRE(it->size()==4);
    ++it;
    REQUIRE(*it=="cmd2"s);
    REQUIRE(it->size()==4);
    ++it;
    REQUIRE(*it=="cmd3"s);
    REQUIRE(it->size()==4);
    ++it;
    REQUIRE(it==history.end());
}

TEST_CASE("insert char at start of string shifts string right and inserts char at beginning")
{
    hh::shell::cmd_history<3, 20> history{};
    history.push_back("2 is the answer");
    auto cursor = history.back().begin();
    cursor=history.insert(cursor, '4');
    auto str = history.back();
    auto expected = "42 is the answer"s;
    CHECK(str == expected);
    CHECK(cursor == str.begin());
}

TEST_CASE("insert char in middle of string shifts string right and inserts char correct pos")
{
    hh::shell::cmd_history<3, 20> history{};
    history.push_back("42is the answer");
    auto cursor = history.back().begin();
    ++cursor;
    ++cursor;
    cursor=history.insert(cursor, ' ');
    auto str = history.back();
    auto expected = "42 is the answer"s;
    CHECK(str == expected);
    CHECK(*cursor == ' ');
}

TEST_CASE("insert char at end of string shifts string right and inserts char correct pos")
{
    hh::shell::cmd_history<3, 20> history{};
    history.push_back("42 is the answe");
    auto cursor = history.back().end();
    cursor = history.insert(cursor, 'r');
    auto str = history.back();
    auto expected = "42 is the answer"s;
    CHECK(str==expected);
    CHECK(*cursor=='r');
}

TEST_CASE("erase on beginning of string deletes first char")
{
    hh::shell::cmd_history<3, 20> history{};
    history.push_back("42 is the answer");
    auto cursor = history.back().begin();
    cursor = history.erase(cursor);
    auto str = history.back();
    auto expected = "2 is the answer"s;
    CHECK(str==expected);
    CHECK(*cursor=='2');
}

TEST_CASE("erase on middle of string deletes middle char")
{
    hh::shell::cmd_history<3, 20> history{};
    history.push_back("42 is the answer");
    auto cursor = history.back().begin();
    ++cursor;
    ++cursor;
    cursor = history.erase(cursor);
    auto str = history.back();
    auto expected = "42is the answer"s;
    CHECK(str==expected);
    CHECK(*cursor=='i');
}

TEST_CASE("erase on end of string deletes last char")
{
    hh::shell::cmd_history<3, 20> history{};
    history.push_back("42 is the answer");
    auto cursor = history.back().end();
    cursor = history.erase(cursor);
    auto str = history.back();
    auto expected = "42 is the answe"s;
    CHECK(str==expected);
    CHECK(cursor==str.end());
}

TEST_CASE("erase on empty string does nothing")
{
    hh::shell::cmd_history<3, 20> history{};
    auto cursor = history.back().end();
    cursor = history.erase(cursor);
    auto str = history.back();
    auto expected = ""s;
    CHECK(str==expected);
    CHECK(cursor==str.begin());
}