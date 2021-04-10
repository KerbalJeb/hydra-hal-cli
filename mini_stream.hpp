/** @file mini_stream.hpp.h
 *   @brief Created on 2021-04-01 by Ben
 */
#pragma once

/**********************************************************************
 * Includes
 **********************************************************************/
#include "util.hpp"
#include <cmath>
#include <concepts>
#include <optional>

/**********************************************************************
 * Concepts
 **********************************************************************/
template <typename T>
concept SerialOutput = requires (T o, const char *s, const char c)
{
  T::puts (s);
  T::putc (c);
  T::flush ();
};

template <typename T> concept SerialInput = requires (T o, int idx)
{
  {
    T::getc ()
    } -> std::convertible_to<int>;
  {
    T::peek (idx)
    } -> std::convertible_to<int>;
};

template <typename T> concept SerialIO = requires
{
  SerialInput<T> &&SerialOutput<T>;
};

/**********************************************************************
 * Constants
 **********************************************************************/

namespace cli::ansi_cmd
{
constexpr auto up = "\x1b[1A";
constexpr auto down = "\x1b[1B";
constexpr auto right = "\x1b[1C";
constexpr auto left = "\x1b[1D";
constexpr auto clear_line = "\x1b[K";
constexpr auto clear_display = "\x1b[2J";
constexpr auto save_cursor = "\x1b[s";
constexpr auto restore_cursor = "\x1b[u";
}

namespace cli::format
{
enum int_format
{
  dec = 10,
  hex = 16,
  bin = 2,
  oct = 8,
};

enum cmd
{
  endl,
  reset,
  flush,
};
constexpr auto bold = "\x1B[1m";
constexpr auto underscore = "\x1B[4m";
constexpr auto blink = "\x1B[5m";
constexpr auto invert = "\x1B[7m";
constexpr auto hide = "\x1B[8m";

namespace text
{
constexpr auto black = "\x1B[30m";
constexpr auto red = "\x1B[31m";
constexpr auto green = "\x1B[32m";
constexpr auto yellow = "\x1B[33m";
constexpr auto blue = "\x1B[34m";
constexpr auto magenta = "\x1B[35m";
constexpr auto cyan = "\x1B[36m";
constexpr auto white = "\x1B[37m";
}

namespace background
{
constexpr auto black = "\x1B[40m";
constexpr auto red = "\x1B[41m";
constexpr auto green = "\x1B[42m";
constexpr auto yellow = "\x1B[43m";
constexpr auto blue = "\x1B[44m";
constexpr auto magenta = "\x1B[45m";
constexpr auto cyan = "\x1B[46m";
constexpr auto white = "\x1B[47m";
}
}

/**********************************************************************
 * Templates
 **********************************************************************/

namespace cli
{

template <class Out, std::size_t N = 8>
requires SerialOutput<Out> class mini_ostream
{
public:
  inline void
  flush ()
  {
    Out::flush ();
  }

  inline mini_ostream &
  operator<< (format::cmd cmd)
  {

    switch (cmd)
      {
      case format::endl:
        *this << "\n\r";
      case format::reset:
        *this << "\x1B[0m" << format::dec;
      case format::flush:
        Out::flush();
        break;
      }
    return *this;
  }

  inline mini_ostream &
  operator<< (const char *string)
  {
    /* print output */
    Out::puts (string);
    return *this;
  }

  inline mini_ostream &
  operator<< (const char c)
  {
    /* print output */
    Out::putc (c);
    return *this;
  }

  inline mini_ostream &
  operator<< (format::int_format base)
  {
    /* print output */
    this->integer_base = base;
    return *this;
  }

  template <typename T>
  requires std::integral<T> inline mini_ostream &
  operator<< (T d)
  {
    /* print output */
    Out::puts (int_to_string (d, buffer, sizeof (buffer), integer_base));
    return *this;
  }

private:
  char buffer[N]{};
  format::int_format integer_base = format::dec;

  static char *
  int_to_string (int value, char *buffer, std::size_t max_len,
                 std::size_t base)
  {
    unsigned int n = std::abs (value);

    auto buffer_pos = int_to_string (n, buffer, max_len - 1, base);

    if (value < 0)
      {
        *(--buffer_pos) = '-';
      }
    return buffer_pos;
  }

  static char *
  int_to_string (unsigned int value, char *buffer, std::size_t max_len,
                 std::size_t base)
  {
    char *buffer_pos = &buffer[max_len - 1];
    *buffer_pos = '\0';
    unsigned int n = value;

    while (n > 0 && buffer_pos >= buffer)
      {
        auto r = n % base;

        if (r < 10)
          {
            *(--buffer_pos) = '0' + r;
          }
        else
          {
            *(--buffer_pos) = 'A' + r - 10;
          }
        n /= base;
      }

    return buffer_pos;
  }
};

template <class IO, class BufferType>
requires SerialIO<IO> &&Queue<BufferType, char> class mini_istream
{
public:
  std::optional<char>
  get ()
  {
    return r_buffer.pop ();
  }

  std::optional<char>
  peek ()
  {
    return r_buffer.peek ();
  }

  void
  flush ()
  {
    r_buffer.clear ();
  }

  int
  get_line (char key, char *buffer, std::size_t buffer_size)
  {
    std::size_t count = 0;
    while (true)
      {
        auto c = r_buffer.pop ();
        if (*c == key || !c.has_value () || *c == '\n')
          {
            break;
          }
        *buffer++ = *c;
        count++;
        if (count >= buffer_size)
          {
            return -1;
          }
      }
    *buffer = '\0';
    return 0;
  }

  void
  put (char c)
  {
    r_buffer.push (c);
  }

  void
  put (const char *s)
  {
    while (*s != '\0')
      {
        char c = *s++;
        put (c);
      }
  }

private:
  BufferType r_buffer;
};
}
