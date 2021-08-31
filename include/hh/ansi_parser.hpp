/// \file ansi_parser.hpp
/// \brief Created on 2021-08-30 by Ben

#pragma once
#include <cstdint>
#include <cctype>

#include <unordered_set>

namespace hh::ansi {

class parser {
public:
    parser() = default;

    struct ansi_code {
      std::uint8_t num_params{};
      std::uint8_t params[3]{};
      char control_char{};
    };

    parser(const char* s, std::size_t count);
    [[nodiscard]] const ansi_code& code() const { return code_; }
    [[nodiscard]] bool good() const { return status_; }

private:
    enum class state {
      start,
      digit_terminator,
      digit_terminator_sep,
      digit,
      done,
    };

    void update_state(char ch);

    static bool is_terminator(char ch){
        return std::isalpha(ch);
    }

    bool status_{true};
    state currentState_{state::start};
    ansi_code code_{};
};
}