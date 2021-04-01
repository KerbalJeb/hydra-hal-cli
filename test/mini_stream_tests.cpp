/** @file cli_tests.cpp.c
*   @brief Created on 2021-03-27 by Ben
*/
/**********************************************************************
* Includes
**********************************************************************/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "cli.hpp"

class mock_output
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
  static void clear ()
  {
    buffer.clear ();
    string.clear ();
  }
  static void flush ()
  {
    string += buffer;
    buffer.clear();
  }
  static inline std::string string{};
 private:
  static inline std::string buffer;
};


class MiniStreamTests : public testing::Test
{
 protected:
  void SetUp () override
  {
    mock_output::clear ();
  }
  cli::mini_ostream<mock_output, 16> cli{};
};

TEST_F(MiniStreamTests, string_output)
{
  cli << "Test String" << cli::format::endl;
  ASSERT_EQ(mock_output::string, "Test String\n\r");
}

TEST_F(MiniStreamTests, int_output)
{
  cli << 42 << cli::format::endl;
  ASSERT_EQ(mock_output::string, "42\n\r");
}

TEST_F(MiniStreamTests, negative_int_output)
{
  cli << -42 << cli::format::endl;
  ASSERT_EQ(mock_output::string, "-42\n\r");
}

TEST_F(MiniStreamTests, unsigned_int_output)
{
  uint8_t x = -1;
  cli << x << cli::format::endl;

  ASSERT_EQ(mock_output::string, "255\n\r");
}

TEST_F(MiniStreamTests, hex_int_output)
{

  cli << cli::format::hex << 42 << cli::format::endl;

  ASSERT_EQ(mock_output::string, "2A\n\r");
}

TEST_F(MiniStreamTests, bin_int_output)
{

  cli << cli::format::bin << 42 << cli::format::endl;

  ASSERT_EQ(mock_output::string, "101010\n\r");
}

TEST_F(MiniStreamTests, oct_int_output)
{

  cli << cli::format::oct << 42 << cli::format::endl;

  ASSERT_EQ(mock_output::string, "52\n\r");
}

TEST_F(MiniStreamTests, char_output)
{
  cli << 'c' << cli::format::endl;

  ASSERT_EQ(mock_output::string, "c\n\r");
}

TEST_F(MiniStreamTests, chained_output)
{
  int answer = 42;
  cli << "The answer to life, the universe and every thing is: " << answer << cli::format::endl;

  ASSERT_EQ(mock_output::string, "The answer to life, the universe and every thing is: 42\n\r");
}

TEST_F(MiniStreamTests, chained_output_multi_base)
{
  int answer = 42;
  cli << "The answer to life, the universe and every thing is: 0x" << cli::format::hex
      << answer << " in hex and " << cli::format::dec << answer << " in decimal" << cli::format::endl;

  ASSERT_EQ(mock_output::string, "The answer to life, the universe and every thing is: 0x2A in hex and 42 in decimal\n\r");
}

TEST_F(MiniStreamTests, multiple_line_streams)
{
  int answer = 42;
  cli << "The answer to life, the universe and every thing is: 0x" << cli::format::hex;
  cli << answer << " in hex and " << cli::format::dec << answer << " in decimal" << cli::format::endl;

  ASSERT_EQ(mock_output::string, "The answer to life, the universe and every thing is: 0x2A in hex and 42 in decimal\n\r");
}

TEST_F(MiniStreamTests, color_output)
{
  cli << cli::format::text::red <<"This text is red " << cli::format::reset <<"this text is not" << cli::format::endl;

  ASSERT_EQ(mock_output::string, "\x1B[31mThis text is red \x1B[0mthis text is not\n\r");
}
