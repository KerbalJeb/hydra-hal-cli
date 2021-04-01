/** @file cli.hpp.h
*   @brief Created on 2021-03-26 by Ben
*/
#pragma once
#include <functional>
#include <cstdint>
#include <cmath>
#include <concepts>
#include <string>

#include "FreeRTOS.h"
#include "task.h"

namespace cli {

namespace ansi_cmd {
constexpr auto left = "\x1b[1D";
constexpr auto right = "\x1b[1C";
constexpr auto clear_line = "\x1b[K";
constexpr auto clear_display = "\x1b[2J";
constexpr auto save_cursor = "\x1b[s";
constexpr auto restore_cursor = "\x1b[u";
}

namespace format {
enum int_format {
  dec = 10,
  hex = 16,
  bin = 2,
  oct = 8,
};

enum cmd {
  endl,
  reset,
};
constexpr auto bold = "\x1B[1m";
constexpr auto underscore = "\x1B[4m";
constexpr auto blink = "\x1B[5m";
constexpr auto invert = "\x1B[7m";
constexpr auto hide = "\x1B[8m";

namespace text {
constexpr auto black = "\x1B[30m";
constexpr auto red = "\x1B[31m";
constexpr auto green = "\x1B[32m";
constexpr auto yellow = "\x1B[33m";
constexpr auto blue = "\x1B[34m";
constexpr auto magenta = "\x1B[35m";
constexpr auto cyan = "\x1B[36m";
constexpr auto white = "\x1B[37m";
}

namespace background {
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

template<typename T> concept SerialOutput =
requires(T o, const char *s, const char c) {
  T::puts(s);
  T::putc(c);
  T::flush();
};

template<typename T> concept SerialInput =
requires(T o, int idx) {
  { T::getc() } -> std::convertible_to<int>;
  { T::peek(idx) } -> std::convertible_to<int>;
};

template<typename T> concept SerialIO =
requires { SerialInput<T> && SerialOutput<T>; };

/* todo: auto flush */
template<class Out, std::size_t N>
requires SerialOutput<Out>
class mini_ostream {
 public:
  inline mini_ostream &operator<<(format::cmd cmd) {

    switch (cmd)
    {
      case format::endl:*this << "\n\r";
        Out::flush();
      case format::reset:*this << "\x1B[0m" << format::dec;
        break;
    }
    return *this;
  }

  inline mini_ostream &operator<<(const char *string) {
    /* print output */
    Out::puts(string);
    return *this;
  }

  inline mini_ostream &operator<<(const char c) {
    /* print output */
    Out::putc(c);
    return *this;
  }

  inline mini_ostream &operator<<(format::int_format base) {
    /* print output */
    this->integer_base = base;
    return *this;
  }

  template<typename T>
  requires std::integral<T>
  inline mini_ostream &operator<<(T d) {
    /* print output */
    Out::puts(int_to_string(d, buffer, sizeof(buffer), integer_base));
    return *this;
  }

 private:
  char buffer[N]{};
  format::int_format integer_base = format::dec;

  static char *int_to_string(int value, char *buffer, std::size_t max_len, std::size_t base) {
    unsigned int n = std::abs(value);

    auto buffer_pos = int_to_string(n, buffer, max_len - 1, base);

    if (value < 0)
    {
      *(--buffer_pos) = '-';
    }
    return buffer_pos;
  }

  static char *int_to_string(unsigned int value, char *buffer, std::size_t max_len, std::size_t base) {
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

template<typename IO>
requires SerialIO<IO>
class mini_istream {
 public:
  char get(){}
  char peek(){}
  int get_line(char key, char *buffer, std::size_t buffer_size){}
  int put(char c){}
  bool operator!(){}
  mini_istream &operator>>(int &x){}
  mini_istream &operator>>(char &c){}
 private:
};

template<typename IO>
requires SerialIO<IO>
class shell {
 public:
  mini_istream<IO> cin;
  mini_ostream<IO, 32> cout;
  void putc(char c){}
 private:
  void echo_to_shell(){}
  int get_prev_cmd(int idx, char* buffer, std::size_t buffer_size){}
  char line_buffer[32];
  char* line_end;
  char* cursor;
};

/* todo add cmd history */
template<typename IO, std::size_t cmd_buffer_length, std::size_t cmd_history_length = 8>
requires SerialIO<IO>
class text_input_handler {
 public:

  inline static void rx_callback() {
    int c;
    char *start;
    while ((c = IO::getc()) != -1)
    {
      switch (c)
      {
        case '\x1b':if (IO::peek(0) != '[') break;

          while ((c = IO::getc()) != -1)
          {
            if (std::isalpha(c))
            {
              switch (c)
              {
                case 'C':if (cursorPos >= currentCommandEnd) break;
                  IO::puts(ansi_cmd::left);
                  cursorPos++;
                  break;
                case 'D':if (cursorPos <= buffer) break;
                  IO::puts(ansi_cmd::left);
                  cursorPos--;
                  break;
                default:break;
              }
              break;
            }
          }
          break;

        case '\b':if (cursorPos <= buffer) break;
          start = cursorPos - 1;
          while (start < currentCommandEnd)
          {
            *start = *(start + 1);
            start++;
          }
          cursorPos--;
          currentCommandEnd--;
          *currentCommandEnd = '\0';
          IO::puts("\x1b[s\x1b[99D");
          IO::puts(ansi_cmd::clear_line);
          IO::puts(prompt);
          IO::puts(buffer);
          IO::puts("\x1b[u\b");
          break;

        case '\r':
        case '\n':
          if (IO::peek(0) == '\r')
          {
            (void)IO::getc();
          }
          *currentCommandEnd = '\0';
          /* Handle cmd */
          current_cmd_idx += 1;
          current_cmd_idx %= cmd_history_length;
          buffer = cmds[current_cmd_idx];
          currentCommandEnd = buffer;
          cursorPos = buffer;
          cmdBufferTextEnd = &buffer[cmd_buffer_length - 2];
          if (reading_task != nullptr)
          {
            xTaskNotifyGive(reading_task);
          }
          break;
        default:
          /* Limit to text chars and cmd buffer length */
          if (cursorPos >= cmdBufferTextEnd || c > '~' || c < ' ') break;
          IO::putc(c);
          if (cursorPos >= currentCommandEnd)
          { currentCommandEnd++; }
          *cursorPos++ = static_cast<char>(c);
          break;
      }
    }
    IO::flush();
  }

  static int get_next_cmd(char *output_begin, std::size_t output_buffer_size) {
    if (reading_task == nullptr)
    {
      reading_task = xTaskGetCurrentTaskHandle();
    }

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    char *input_buffer = cmds[unread_cmd_idx];
    std::size_t count = 0;
    while (*input_buffer != '\0')
    {
      if (count >= output_buffer_size - 1)
      {
        *output_begin = '\0';
        return -1;
      }
      *output_begin++ = *input_buffer++;
      count++;
    }

    *output_begin = '\0';
    unread_cmd_idx++;
    unread_cmd_idx %= cmd_history_length;
    return 0;
  }

  static void clear() {
    buffer = cmds[0];
    currentCommandEnd = &buffer[0];
    cursorPos = &buffer[0];
    current_cmd_idx = 0;
    unread_cmd_idx = 0;
    cmdBufferTextEnd = &buffer[cmd_buffer_length - 2];
    reading_task = nullptr;
  }

  static inline const char *prompt = ">>> ";

 private:
  static inline char cmds[cmd_history_length][cmd_buffer_length]{};
  static inline auto buffer = cmds[0];
  static inline auto currentCommandEnd = &buffer[0];
  static inline auto cursorPos = &buffer[0];
  static inline uint8_t current_cmd_idx = 0;
  static inline uint8_t unread_cmd_idx = 0;
  static inline auto cmdBufferTextEnd = &buffer[cmd_buffer_length - 2];
  static inline TaskHandle_t reading_task = nullptr;
};

}
