/// \file test_mini_stream.cpp
/// \brief Created on 2021-08-30 by Ben

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <sstream>
#include <hh/mini_stream.hpp>

using mini_ostream = hh::shell::oserial_stream<std::stringstream>;

TEST_CASE("mini_ostream put char", "[ostream][put]")
{
    std::stringstream ss;
    mini_ostream stream{ss};

    stream.put('a');
    auto str = ss.str();
    CHECK(str=="a");
}

TEST_CASE("mini_ostream write string", "[ostream][write]")
{
    std::stringstream ss;
    mini_ostream stream{ss};

    auto str = GENERATE(
            std::string{"abcdefg"},
            std::string{"testing"},
            std::string{"a"}
    );

    CAPTURE(str);

    stream.write(str.c_str(), str.size());
    CHECK(ss.str()==str);
}

TEST_CASE("mini_ostream char stream inserter", "[ostream][stream_inserter]")
{
    std::stringstream ss;
    mini_ostream stream{ss};

    stream << 'c';

    CHECK(ss.str()=="c");
}

TEST_CASE("mini_ostream string stream inserter", "[ostream][stream_inserter]")
{
    std::stringstream ss;
    mini_ostream stream{ss};

    auto str = GENERATE(
            std::string{"abcdefg"},
            std::string{"testing"},
            std::string{"a"}
    );

    CAPTURE(str);

    stream << str.c_str();

    CHECK(ss.str()==str);
}

TEMPLATE_TEST_CASE("mini_ostream int stream inserter", "[ostream][stream_inserter]", signed char, short, int)
{
    std::stringstream ss;
    mini_ostream stream{ss};

    auto i = GENERATE(0, 1, -1, -5, 127, -128);
    stream << i;
    CAPTURE(i);
    CHECK(ss.str()==std::to_string(i));
}

TEMPLATE_TEST_CASE("mini_ostream uint stream inserter", "[ostream][stream_inserter]", unsigned char, unsigned short,
        unsigned int)
{
    std::stringstream ss;
    mini_ostream stream{ss};

    auto i = GENERATE(0, 1, 127, 255);
    stream << i;
    CAPTURE(i);
    CHECK(ss.str()==std::to_string(i));
}