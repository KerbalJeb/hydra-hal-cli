/** @file shell_tests.cpp.c
 *   @brief Created on 2021-04-01 by Ben
 */
/**********************************************************************
 * Includes
 **********************************************************************/
#include "mock_serial.hpp"
#include "shell.hpp"
#include "util.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

/**********************************************************************
 * Test Fixtures
 **********************************************************************/

class ShellTests : public testing::Test
{
protected:
  void
  SetUp () override
  {
    mock_serial::clear ();
  }
  cli::shell<mock_serial, ring_buffer<char, 32> > shell{ "$",
                                                         "Welcome Message" };
  char buffer[256];
};

/**********************************************************************
 * Tests
 **********************************************************************/

TEST_F (ShellTests, echo_alphanumeric)
{
  std::string text = "hello world 42!";
  mock_serial::istring = text;
  shell.receive ();
  EXPECT_EQ (mock_serial::ostring, text);
}

TEST_F (ShellTests, welcome_message)
{
  shell.connected ();
  EXPECT_EQ (mock_serial::ostring, "\n\rWelcome Message\n\r$");
}

TEST_F (ShellTests, get_input_after_newline)
{
  std::string text = "hello world 42!\n";
  mock_serial::istring = text;
  shell.receive ();
  auto &cin = shell.cin;

  int return_value = cin.get_line (' ', buffer, 32);
  EXPECT_STREQ (buffer, "hello");
  EXPECT_EQ (return_value, 0);

  return_value = cin.get_line (' ', buffer, 32);
  EXPECT_STREQ (buffer, "world");
  EXPECT_EQ (return_value, 0);

  return_value = cin.get_line (' ', buffer, 32);
  EXPECT_STREQ (buffer, "42!");
  EXPECT_EQ (return_value, 0);

  EXPECT_EQ (mock_serial::ostring, "hello world 42!\n\r");
}

TEST_F (ShellTests, backspace)
{
  std::string text = "hello\b\n";
  mock_serial::istring = text;
  shell.receive ();
  auto &cin = shell.cin;
  int return_value = cin.get_line ('\n', buffer, 32);
  EXPECT_STREQ (buffer, "hell");
  EXPECT_EQ (return_value, 0);
}

TEST_F (ShellTests, move_cursor_and_backspace)
{
  std::string text = "test\x1b[1D\b\n";
  mock_serial::istring = text;
  shell.receive ();
  auto &cin = shell.cin;
  int return_value = cin.get_line ('\n', buffer, 32);
  EXPECT_STREQ (buffer, "tet");
  EXPECT_EQ (return_value, 0);
}

TEST_F (ShellTests, buffer_overflow)
{
  std::string text = "very_long_word_that_will_overflow_the_buffer\n";
  mock_serial::istring = text;
  shell.receive ();
  auto &cin = shell.cin;
  int return_value = cin.get_line ('\n', buffer, 256);
  EXPECT_STREQ (buffer, "very_long_word_that_will_over");
  EXPECT_EQ (return_value, 0);
}

TEST_F (ShellTests, new_line_before_get)
{
  std::string text = "a two line message\nto test\n";
  mock_serial::istring = text;
  shell.receive ();
  auto &cin = shell.cin;
  int return_value;

  return_value = cin.get_line ('\n', buffer, 32);
  EXPECT_STREQ (buffer, "a two line message");
  EXPECT_EQ (return_value, 0);

  return_value = cin.get_line ('\n', buffer, 32);
  EXPECT_STREQ (buffer, "to test");
  EXPECT_EQ (return_value, 0);
}

TEST_F (ShellTests, get_cmd_history)
{
  std::string text = "cmd1\ncmd2\n";
  mock_serial::istring = text;
  shell.receive ();
  auto &cin = shell.cin;
  int return_value;

  return_value = cin.get_line ('\n', buffer, 32);
  EXPECT_STREQ (buffer, "cmd1");
  EXPECT_EQ (return_value, 0);

  return_value = cin.get_line ('\n', buffer, 32);
  EXPECT_STREQ (buffer, "cmd2");
  EXPECT_EQ (return_value, 0);

  mock_serial::istring = std::string{ cli::ansi_cmd::down } + "\n";
  shell.receive ();
  return_value = cin.get_line ('\n', buffer, 32);
  EXPECT_STREQ (buffer, "cmd2");
  EXPECT_EQ (return_value, 0);
}

TEST_F (ShellTests, multiple_cmd_history)
{
  std::string text = "cmd1\ncmd2\n";
  mock_serial::istring = text;
  shell.receive ();
  auto &cin = shell.cin;
  int return_value;
  (void)cin.get_line ('\n', buffer, 32);
  (void)cin.get_line ('\n', buffer, 32);

  mock_serial::istring
      = std::string{ cli::ansi_cmd::down } + cli::ansi_cmd::down + "\n";
  shell.receive ();

  return_value = cin.get_line ('\n', buffer, 32);
  EXPECT_STREQ (buffer, "cmd1");
  EXPECT_EQ (return_value, 0);
}

TEST_F (ShellTests, cmd_history_up)
{
  std::string text = "cmd1\ncmd2\ncmd3\n";
  mock_serial::istring = text;
  shell.receive ();
  auto &cin = shell.cin;
  int return_value;
  (void)cin.get_line ('\n', buffer, 32);
  (void)cin.get_line ('\n', buffer, 32);

  mock_serial::istring = std::string{ cli::ansi_cmd::down }
                         + cli::ansi_cmd::down + cli::ansi_cmd::up + "\n";
  shell.receive ();

  return_value = cin.get_line ('\n', buffer, 32);
  EXPECT_STREQ (buffer, "cmd3");
  EXPECT_EQ (return_value, 0);
}