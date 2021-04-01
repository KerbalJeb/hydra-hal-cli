/** @file shell.hpp.h
*   @brief Created on 2021-04-01 by Ben
*/
#pragma once

/**********************************************************************
* Includes
**********************************************************************/
#include "mini_stream.hpp"

/**********************************************************************
* Templates
**********************************************************************/

namespace cli {

template<class IO>
requires SerialIO<IO>
class shell {
 private:
  struct cli_io
  {
    static inline void puts(const char*s){IO::getc(s);}
    static inline void putc(const char c){IO::putc(c);}
    static inline int getc(){return IO::getc();}
    static inline int peek(int idx){return IO::peek(idx);}
    static inline void flush(){IO::flush();}
  };

 public:
  /**
   * @brief Used stream the input into an object
   */

  mini_istream<cli_io> cin;
  /**
   * @brief Lets the application print to the shell
   */
  mini_ostream<cli_io> cout;

  /**
   * @brief Adds a recived char to the shell, it will be echoed back using the provided IO serial interface
   * @param c The new char
   */
  void receive_char(char c)
  {
    echo_to_shell(c);
    /* Command History */
    /* Add to cout */
    if (c == '\n')
    {

    }
  }

 private:
  void echo_to_shell(char c)
  {

  }
  int get_prev_cmd(int idx, char* buffer, std::size_t buffer_size){}
  char line_buffer[128];
  char* line_start = line_buffer;
  char* line_end = line_buffer;
  char* cursor = line_buffer;
};
}