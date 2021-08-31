/// \file ansi_escape_codes.hpp
/// \brief Created on 2021-08-30 by Ben

#pragma once
#include <cstdint>

namespace hh::ansi {
constexpr char escape[] = "\x1b[";

class parser {
public:
    parser() = default;

    struct ansi_code {
      std::uint8_t num_params{};
      std::uint8_t params[3]{};
      char control_char{};
    };

    parser(const char* s, std::size_t count);
    [[nodiscard]] const ansi_code& code() const;

private:
    enum class state {
      start,
      parse,
      digit,
      done,
      error
    };

    bool status_{true};
    state state_{state::start};
    ansi_code code_{};
};
}