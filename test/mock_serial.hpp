/** @file mock_serial.hpp.h
*   @brief Created on 2021-04-01 by Ben
*/
#pragma once

/**********************************************************************
* Includes
**********************************************************************/
#include <string>
#include <stdexcept>

/**********************************************************************
* Classes
**********************************************************************/
class mock_serial
{
 public:
  static void puts (const char *s)
  {
    buffer += s;
  }
  static void putc (const char c)
  {
    buffer += c;
  }
  static int getc ()
  {
    if (istring.empty ())
    {
      return -1;
    }
    char c = istring.at (0);
    istring = istring.substr (1);
    return c;
  }
  static int peek (int idx)
  {
    try
    {
      return istring.at (idx);
    }
    catch (std::out_of_range const &e)
    {
      return -1;
    }
  }
  static void clear ()
  {
    buffer.clear ();
    ostring.clear ();
    istring.clear();
  }
  static void flush ()
  {
    ostring += buffer;
    buffer.clear();
  }
  static inline std::string ostring{};
  static inline std::string istring{};
 private:
  static inline std::string buffer;
};
