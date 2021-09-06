/// \file ansi_codes.hpp
/// \brief Created on 2021-08-31 by Ben

#pragma once
#include <hh/mini_stream.hpp>

namespace hh::ansi {

template<std::size_t N, typename ...Args>
struct code_t {
  explicit constexpr code_t(char ch, Args...args)
  :ch{ ch }, params{ static_cast<uint8_t>(args)... } { }
  const char ch;
  std::uint8_t params[N];
};

template<>
struct code_t<0> {
  explicit constexpr code_t(char ch)
  : ch{ ch }{ }
  const char ch;
};

template<typename ...Args>
code_t (char ch, Args...args) -> code_t<sizeof...(args), Args...>;

template<class T, std::size_t N, typename ...Args>
shell::oserial_stream<T>& operator<<(shell::oserial_stream<T>& os, const code_t<N, Args...>& code)
{
    os << "\x1b[";
    if constexpr(N > 0)
    {
        for (std::size_t i = 0; i<N; ++i) {
            os << code.params[i];
            if (i<N-1) { os << ';'; }
        }
    }
    os << code.ch;
    return os;
}

constexpr code_t clear_line{'K', 2};
constexpr code_t save_cursor{'s'};
constexpr code_t restore_cursor{'u'};

template<std::uint8_t n>
constexpr code_t move_up{'A', n};

template<std::uint8_t n>
constexpr code_t move_down{'B', n};

template<std::uint8_t n>
constexpr code_t move_right{'C', n};

template<std::uint8_t n>
constexpr code_t move_left{'D', n};

}