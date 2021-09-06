/// \file test_ansi_parser.cpp
/// \brief Created on 2021-08-30 by Ben

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <hh/ansi_parser.hpp>
#include <string>

using parser = hh::ansi::parser;
TEST_CASE("ansi parser", "[ansi]") {
    auto [str, expected] = GENERATE(
            std::tuple{std::string{"[H"}, parser::ansi_code{0, {0, 0, 0}, 'H'}},
            std::tuple{std::string{"[5A"}, parser::ansi_code{1, {5, 0, 0}, 'A'}},
            std::tuple{std::string{"[38;5m"}, parser::ansi_code{2, {38, 5, 0}, 'm'}},
            std::tuple{std::string{"[38;7;5m"}, parser::ansi_code{3, {38, 7, 5}, 'm'}});

    CAPTURE(str);
    parser p{str.c_str(), str.size()};

    auto code = p.code();
    auto num_params = code.num_params;
    CHECK(num_params == expected.num_params);
    CHECK(code.control_char == expected.control_char);
    CHECK(p.good());
    CHECK(p.done_parsing());
    for (int i = 0; i < num_params; ++i) {
        CHECK(p.code().params[i] == expected.params[i]);
    }
}

TEST_CASE("ansi parser invalid", "[ansi]") {
    auto str = GENERATE(
            std::string{"a"},
            std::string{"["},
            std::string{"[5;A"},
            std::string{"[38-5m"},
            std::string{"[38,7;5m"});

    CAPTURE(str);
    parser p{str.c_str(), str.size()};

    CHECK(!p.good());
}