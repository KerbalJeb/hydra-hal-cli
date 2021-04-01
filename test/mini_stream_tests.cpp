/** @file cli_tests.cpp.c
*   @brief Created on 2021-03-27 by Ben
*/
/**********************************************************************
* Includes
**********************************************************************/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "mini_stream.hpp"
#include "mock_serial.hpp"

class MiniStreamTests : public testing::Test
{
 protected:
  void SetUp () override
  {
    mock_serial::clear ();
  }
  cli::mini_ostream<mock_serial, 16> cli{};
};

TEST_F(MiniStreamTests, string_output)
{
  cli << "Test String" << cli::format::endl;
  ASSERT_EQ(mock_serial::ostring, "Test String\n\r");
}

TEST_F(MiniStreamTests, int_output)
{
  cli << 42 << cli::format::endl;
  ASSERT_EQ(mock_serial::ostring, "42\n\r");
}

TEST_F(MiniStreamTests, negative_int_output)
{
  cli << -42 << cli::format::endl;
  ASSERT_EQ(mock_serial::ostring, "-42\n\r");
}

TEST_F(MiniStreamTests, unsigned_int_output)
{
  uint8_t x = -1;
  cli << x << cli::format::endl;

  ASSERT_EQ(mock_serial::ostring, "255\n\r");
}

TEST_F(MiniStreamTests, hex_int_output)
{

  cli << cli::format::hex << 42 << cli::format::endl;

  ASSERT_EQ(mock_serial::ostring, "2A\n\r");
}

TEST_F(MiniStreamTests, bin_int_output)
{

  cli << cli::format::bin << 42 << cli::format::endl;

  ASSERT_EQ(mock_serial::ostring, "101010\n\r");
}

TEST_F(MiniStreamTests, oct_int_output)
{

  cli << cli::format::oct << 42 << cli::format::endl;

  ASSERT_EQ(mock_serial::ostring, "52\n\r");
}

TEST_F(MiniStreamTests, char_output)
{
  cli << 'c' << cli::format::endl;

  ASSERT_EQ(mock_serial::ostring, "c\n\r");
}

TEST_F(MiniStreamTests, chained_output)
{
  int answer = 42;
  cli << "The answer to life, the universe and every thing is: " << answer << cli::format::endl;

  ASSERT_EQ(mock_serial::ostring, "The answer to life, the universe and every thing is: 42\n\r");
}

TEST_F(MiniStreamTests, chained_output_multi_base)
{
  int answer = 42;
  cli << "The answer to life, the universe and every thing is: 0x" << cli::format::hex
      << answer << " in hex and " << cli::format::dec << answer << " in decimal" << cli::format::endl;

  ASSERT_EQ(mock_serial::ostring, "The answer to life, the universe and every thing is: 0x2A in hex and 42 in decimal\n\r");
}

TEST_F(MiniStreamTests, multiple_line_streams)
{
  int answer = 42;
  cli << "The answer to life, the universe and every thing is: 0x" << cli::format::hex;
  cli << answer << " in hex and " << cli::format::dec << answer << " in decimal" << cli::format::endl;

  ASSERT_EQ(mock_serial::ostring, "The answer to life, the universe and every thing is: 0x2A in hex and 42 in decimal\n\r");
}

TEST_F(MiniStreamTests, color_output)
{
  cli << cli::format::text::red <<"This text is red " << cli::format::reset <<"this text is not" << cli::format::endl;

  ASSERT_EQ(mock_serial::ostring, "\x1B[31mThis text is red \x1B[0mthis text is not\n\r");
}
