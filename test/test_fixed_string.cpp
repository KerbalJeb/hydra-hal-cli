#include <catch2/catch_test_macros.hpp>

#include <hh/fixed_string.h>

using namespace std::string_literals;

SCENARIO("strings can be modified") {
    GIVEN("a default constructed string") {
        hh::container::fixed_string<16> string{};

        CHECK(string.empty());
        CHECK(string.begin() == string.end());
        CHECK(string.size() == 0);
        CHECK(string.c_str() == ""s);

        WHEN("push back is called with a char") {
            string.push_back('c');

            THEN("the string contains only that char") {
                CHECK(string.c_str() == "c"s);
            }
        }

        WHEN("append is called with a c string") {
            auto str = "a string"s;
            string.append(str.c_str());

            THEN("the fixed_string contains only the given c string") {
                CHECK(string.c_str() == str);
            }
        }
    }

    GIVEN("a non empty/full string"){
        hh::container::fixed_string<40> string{"a string"};

        CHECK(!string.empty());
        CHECK(string.size() == 8);
        CHECK(string.c_str() == "a string"s);

        WHEN("push back is called with a char"){
            string.push_back('!');
            THEN("the char is appended to the string"){
                CHECK(string.c_str() == "a string!"s);
            }
        }

        WHEN("append is called with a c string that will fit into the buffer"){
            auto str = ", and this is another string"s;
            string.append(str.c_str());
            THEN("the c string was append to the fixed_string"){
                CHECK(string.c_str() == "a string, and this is another string"s);
            }
        }

        WHEN("append is called with a string that will NOT fit into the buffer"){
            auto str = ", and this is another very, very, very long string"s;
            string.append(str.c_str());
            THEN("a portion of the c string is append fixed_string, until the fixed_string is full"){
                CHECK(string.c_str() == "a string, and this is another very, very"s);
                CHECK(string.size() == string.max_size());
            }
        }
    }

    GIVEN("a full string"){
        hh::container::fixed_string<13> string{"a full string"};
        CHECK(string.size() == string.max_size());
        CHECK(string.c_str() == "a full string"s);

        WHEN("push back is called with a char"){
            string.push_back('c');
            THEN("the string is unmodified"){
                CHECK(string.size() == string.max_size());
                CHECK(string.c_str() == "a full string"s);
            }
        }

        WHEN("append is called with a c string"){
            string.append("a string");
            THEN("the string is unmodified"){
                CHECK(string.size() == string.max_size());
                CHECK(string.c_str() == "a full string"s);
            }
        }
    }
}