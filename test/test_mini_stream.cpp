/// \file test_mini_stream.cpp
/// \brief Created on 2021-08-30 by Ben

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <sstream>
#include <hh/mini_stream.hpp>

using mini_ostream = hh::cli::mini_ostream<std::stringstream>;

TEST_CASE("mini_ostream put char", "[ostream][put]")
{
    mini_ostream stream{std::stringstream{}};
    auto& ss = stream.native_stream();

    stream.put('a');
    auto str = ss.str();
    CHECK(str=="a");
}

TEST_CASE("mini_ostream write string", "[ostream][write]")
{
    mini_ostream stream{std::stringstream{}};
    auto& ss = stream.native_stream();

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
    mini_ostream stream{std::stringstream{}};
    auto& ss = stream.native_stream();

    stream << 'c';

    CHECK(stream.native_stream().str()=="c");
}

TEST_CASE("mini_ostream string stream inserter", "[ostream][stream_inserter]")
{
    mini_ostream stream{std::stringstream{}};

    auto str = GENERATE(
            std::string{"abcdefg"},
            std::string{"testing"},
            std::string{"a"}
    );

    CAPTURE(str);

    stream << str.c_str();

    CHECK(stream.native_stream().str()==str);
}

TEMPLATE_TEST_CASE("mini_ostream int stream inserter", "[ostream][stream_inserter]", signed char, short, int)
{
    mini_ostream stream{std::stringstream{}};
    auto i = GENERATE(0, 1, -1, -5, 127, -128);
    stream << i;
    CAPTURE(i);
    CHECK(stream.native_stream().str()==std::to_string(i));
}

TEMPLATE_TEST_CASE("mini_ostream uint stream inserter", "[ostream][stream_inserter]", unsigned char, unsigned short,
        unsigned int)
{
    mini_ostream stream{std::stringstream{}};
    auto i = GENERATE(0, 1, 127, 255);
    stream << i;
    CAPTURE(i);
    CHECK(stream.native_stream().str()==std::to_string(i));
}