/** @file shell.hpp.h
 *   @brief Created on 2021-04-01 by Ben
 */
#pragma once

/**********************************************************************
 * Includes
 **********************************************************************/
#include "mini_stream.hpp"
#include <algorithm>
#include <cctype>
#include <concepts>
#include <cstring>

/**********************************************************************
 * Templates
 **********************************************************************/

namespace cli
{

struct cmd_history
{
public:
  void
  add_cmd (const char *cmd)
  {
    while (*cmd != '\0')
      {
        add_c (*cmd);
        cmd++;
      }
    current_cmd_idx = head - 1;
    if (current_cmd_idx < 0)
      {
        current_cmd_idx = size + current_cmd_idx;
      }

    add_c ('\0');
  }

  std::optional<const char *>
  get_cmd_down ()
  {
    while (internal_storage[current_cmd_idx] != '\0')
      {
        current_cmd_idx--;
        if (current_cmd_idx < 0)
          {
            current_cmd_idx = size + current_cmd_idx;
          }
      }
    auto idx = (current_cmd_idx + 1) % size;

    current_cmd_idx--;
    if (current_cmd_idx < 0)
      {
        current_cmd_idx = size + current_cmd_idx;
      }
    return &internal_storage[idx];
  }

  std::optional<const char *>
  get_cmd_up ()
  {
    current_cmd_idx = (current_cmd_idx + 2) % size;
    auto idx = current_cmd_idx;
    while (internal_storage[current_cmd_idx] != '\0')
      {
        current_cmd_idx = (current_cmd_idx + 1) % size;
      }
    return &internal_storage[idx];
  }

private:
  void
  add_c (char c)
  {
    internal_storage[head] = c;
    if (full)
      {
        tail = (tail + 1) % N;
      }
    head = (head + 1) % N;
    if (!full)
      {
        size += 1;
      }
    full = (tail == head);
  }

  static constexpr int N = 128;
  static_assert (N > 0, "N must be greater than 0");
  char internal_storage[128];
  int head;
  int tail;
  int size;
  int current_cmd_idx;
  bool full;
};

template <class IO, class StreamBuffer>
requires SerialIO<IO> &&Queue<StreamBuffer, char> class shell
{
public:
  /**
   * @brief Used stream the input into an object
   */

  mini_istream<IO, StreamBuffer> cin;
  /**
   * @brief Lets the application print to the shell
   */
  mini_ostream<IO> cout;

  const char *prompt;

  explicit shell (const char *prompt, const char *welcome_message)
      : prompt (prompt), welcome_message (welcome_message)
  {
  }

  /**
   * @brief Needs to be called every time a new char is received
   */
  void
  receive ()
  {
    int c;
    while ((c = IO::getc ()) != -1)
      {
        echo_to_shell (c);
      }
    IO::flush ();
  }

  /**
   * @brief Must be called whenever the shell is connected
   */
  void
  connected ()
  {
    /* Print welcome message */
    IO::puts ("\n\r");
    IO::puts (welcome_message);
    IO::puts ("\n\r");
    IO::puts (prompt);
    IO::flush ();
  }

private:
  void
  echo_to_shell (char c)
  {
    switch (c)
      {
      case '\b':
        backspace ();
        break;
      case '\x1b':
        handle_escape_sequence ();
        break;
      case '\r':
      case '\n':
        IO::puts ("\n\r");
        //        IO::puts (prompt);
        /* add current buffer to cin */
        *line_end = '\0';
        history.add_cmd (line_start);
        *line_end++ = '\n';
        *line_end = '\0';
        cin.put (line_start);
        line_end = line_buffer;
        cursor = line_buffer;
        break;
      default:
        if (line_end >= buffer_end - 2)
          {
            break;
          }
        IO::putc (c);
        *line_end++ = c;
        cursor++;
        break;
      }
  }

  void
  handle_escape_sequence ()
  {
    if (IO::peek (0) != '[')
      {
        return;
      }

    int c;
    while ((c = IO::getc ()) != -1)
      {
        if (std::isalpha (c) != 0)
          {
            std::optional<const char *> cmd;
            std::size_t len;
            switch (c)
              {
              case 'C':
                if (cursor >= line_end)
                  {
                    break;
                  }
                IO::puts (ansi_cmd::right);
                cursor++;
                break;
              case 'D':
                if (cursor <= line_buffer)
                  {
                    break;
                  }
                IO::puts (ansi_cmd::left);
                cursor--;
                break;
              case 'A':
                cmd = history.get_cmd_up ();
                set_line(cmd);
                break;
              case 'B':
                cmd = history.get_cmd_down ();
                set_line(cmd);
                break;
              default:
                break;
              }
            break;
          }
      }
  }

  void
  set_line (std::optional<const char *> line_str)
  {
    if (!line_str.has_value ())
      {
        return;
      }
    clear_line ();
    std::size_t len = std::strlen (*line_str);
    (void)std::copy_n (*line_str, len, line_buffer);
    line_buffer[len+1] = '\0';
    IO::puts (line_buffer);
    cursor += len;
    line_end += len;
  }

  void
  backspace ()
  {
    if (cursor <= line_start)
      {
        return;
      }
    char *start = cursor - 1;
    while (start < line_end)
      {
        *start = *(start + 1);
        start++;
      }
    cursor--;
    line_end--;
    *line_end = '\0';
    IO::puts ("\x1b[s\x1b[99D");
    IO::puts (ansi_cmd::clear_line);
    IO::puts (prompt);
    IO::puts (line_buffer);
    IO::puts ("\x1b[u\b");
  }

  void
  clear_line ()
  {
    IO::puts (ansi_cmd::clear_line);
    IO::puts ("\x1b[99D");
    IO::puts (prompt);
    cursor = line_buffer;
    line_end = line_buffer;
  }

  char line_buffer[32]{};
  char *line_end = line_buffer;
  char *cursor = line_buffer;
  const char *buffer_end = &line_buffer[31];
  const char *line_start = line_buffer;
  cmd_history history{};
  const char *welcome_message;
};
} // namespace cli