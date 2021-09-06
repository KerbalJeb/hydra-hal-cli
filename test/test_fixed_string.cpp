#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <hh/fixed_string.h>
#include <tuple>

using namespace std::string_literals;

TEST_CASE("string equality comparison") {
    auto s = GENERATE(""s, "str"s, "125a"s);
    hh::container::fixed_string<16> string1{s.c_str()};
    hh::container::fixed_string<16> string2{s.c_str()};
    CHECK(string1 == string2);
}

TEST_CASE("string inequality comparison") {
    auto s = GENERATE(""s, "str"s, "125a"s);
    hh::container::fixed_string<16> string1{s.c_str()};
    hh::container::fixed_string<16> string2{"other"};
    CHECK(string1 != string2);
}

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

        WHEN("insert is called with a char") {
            auto it = GENERATE_REF(string.begin(), string.end());
            it = string.insert(it, 'c');

            THEN("the string contains only that char") {
                CHECK(string.c_str() == "c"s);
                CHECK(*it == 'c');
            }
        }

        WHEN("append is called with a c string") {
            auto str = "a string"s;
            string.append(str.c_str());

            THEN("the fixed_string contains only the given c string") {
                CHECK(string.c_str() == str);
            }
        }

        WHEN("insert is called with a string") {
            auto str = "a string"s;
            auto it = GENERATE_REF(string.begin(), string.end());
            string.insert(it, str.c_str());

            THEN("the fixed_string contains only the given string") {
                CHECK(string.c_str() == str);
            }
        }

        WHEN("pop back is called") {
            string.pop_back();
            THEN("the string remains empty") {
                CHECK(string.c_str() == ""s);
            }
        }
    }

    GIVEN("a non empty/full string") {
        hh::container::fixed_string<40> string{"a string"};

        CHECK(!string.empty());
        CHECK(string.size() == 8);
        CHECK(string.c_str() == "a string"s);

        WHEN("push back is called with a char") {
            string.push_back('!');
            THEN("the char is appended to the string") {
                CHECK(string.c_str() == "a string!"s);
            }
        }

        WHEN("insert is called with a position iterator and a char") {
            auto [it, expected] = GENERATE_REF(
                    std::tuple{string.begin(), "ca string"s},
                    std::tuple{string.end(), "a stringc"s},
                    std::tuple{string.begin() + 2, "a cstring"s});

            it = string.insert(it, 'c');

            THEN("the char is inserted before the iterator") {
                CHECK(string.c_str() == expected);
                CHECK(*it == 'c');
            }
        }

        WHEN("append is called with a string that will fit into the buffer") {
            auto str = ", and this is another string"s;
            string.append(str.c_str());
            THEN("the c string was append to the fixed_string") {
                CHECK(string.c_str() == "a string, and this is another string"s);
            }
        }

        WHEN("insert is called with a position iterator and a string that fits") {
            auto [it, expected] = GENERATE_REF(
                    std::tuple{string.begin(), " str a string"s},
                    std::tuple{string.end(), "a string str "s},
                    std::tuple{string.begin() + 2, "a  str string"s});
            auto str = " str "s;
            string.insert(it, str.c_str());
            THEN("the string is inserted before the iterator") {
                CHECK(string.c_str() == expected);
            }
        }

        WHEN("append is called with a string that will NOT fit into the buffer") {
            auto str = ", and this is another very, very, very long string"s;
            string.append(str.c_str());
            THEN("a portion of the c string is append fixed_string, until the fixed_string is full") {
                CHECK(string.c_str() == "a string, and this is another very, very"s);
                CHECK(string.size() == string.max_size());
            }
        }

        WHEN("pop back is called") {
            string.pop_back();
            THEN("the last char of the string is removed") {
                CHECK(string.c_str() == "a strin"s);
            }
        }

        WHEN("erase is called") {
            auto [it, expected] = GENERATE_REF(
                    std::tuple{string.begin(), " string"s},
                    std::tuple{string.end() - 1, "a strin"s},
                    std::tuple{string.begin() + 2, "a tring"s});
            string.erase(it);

            THEN("the char at pos is erased") {
                CHECK(string.c_str() == expected);
            }
        }
    }

    GIVEN("a full string") {
        hh::container::fixed_string<13> string{"a full string"};
        CHECK(string.size() == string.max_size());
        CHECK(string.c_str() == "a full string"s);

        WHEN("push back is called with a char") {
            string.push_back('c');
            THEN("the string is unmodified") {
                CHECK(string.size() == string.max_size());
                CHECK(string.c_str() == "a full string"s);
            }
        }

        WHEN("insert is called with a char") {
            auto pos = GENERATE_REF(string.begin(), string.end(), string.begin() + 5);
            auto it = string.insert(pos, 'c');

            THEN("the string is unmodified") {
                CHECK(string.size() == string.max_size());
                CHECK(string.c_str() == "a full string"s);
                CHECK(it == pos);
            }
        }

        WHEN("append is called with a c string") {
            string.append("a string");
            THEN("the string is unmodified") {
                CHECK(string.size() == string.max_size());
                CHECK(string.c_str() == "a full string"s);
            }
        }

        WHEN("insert is called with a string") {
            auto pos = GENERATE_REF(string.begin(), string.end(), string.begin() + 5);
            string.insert(pos, "test");

            THEN("the string is unmodified") {
                CHECK(string.size() == string.max_size());
                CHECK(string.c_str() == "a full string"s);
            }
        }
    }
}