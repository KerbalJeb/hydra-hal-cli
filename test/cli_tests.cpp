/** @file cli_tests.cpp.c
*   @brief Created on 2021-03-27 by Ben
*/
/**********************************************************************
* Includes
**********************************************************************/
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "cli.hpp"
#include "Mocktask.h"

DEFINE_FFF_GLOBALS;

class mock_serial
{
 public:
  static void puts (const char *s)
  {
    input_buffer += s;
  }
  static void putc (const char c)
  {
    input_buffer += c;
  }

  static int getc ()
  {
    if (output_string.empty ())
    {
      return -1;
    }
    char c = output_string.at (0);
    output_string = output_string.substr (1);
    return c;
  }

  static int peek (int idx)
  {
    try
    {
      return output_string.at (idx);
    }
    catch (std::out_of_range const &e)
    {
      return -1;
    }
  }

  static void flush ()
  {
    input_string += input_buffer;
    input_buffer.clear();
  }

  static void clear ()
  {
    input_string.clear ();
    output_string.clear ();
  }
  static inline std::string input_string{};
  static inline std::string output_string{};
 private:
  static inline std::string input_buffer{};
};

using text_handler = cli::text_input_handler<mock_serial, 128>;

class CliTests : public testing::Test
{
 protected:
  void SetUp () override
  {
    xTaskGenericNotify_reset ();
    RESET_FAKE(ulTaskGenericNotifyTake);
    RESET_FAKE(xTaskGetCurrentTaskHandle);
    RESET_FAKE(xTaskGenericNotify);
    FFF_RESET_HISTORY()
    text_handler::clear();
    mock_serial::clear();
    xTaskGetCurrentTaskHandle_fake.return_val = reinterpret_cast<TaskHandle_t>(0x55);
  }
  char                                      buffer[128];
};

TEST_F(CliTests, rx_alphanumeric_string)
{
  mock_serial::output_string = "hello world: 42!";
  text_handler::rx_callback ();
  ASSERT_EQ(mock_serial::input_string, "hello world: 42!");
}

TEST_F(CliTests, rx_newline)
{
  mock_serial::output_string = "hello world: 42!\n";
  text_handler::rx_callback ();

  auto return_code = text_handler::get_next_cmd (buffer, sizeof (buffer));

  EXPECT_EQ(mock_serial::input_string, "hello world: 42!");
  EXPECT_EQ(return_code, 0);
  EXPECT_STREQ(buffer, "hello world: 42!");
}

TEST_F(CliTests, rx_same_sized_buffer)
{
  mock_serial::output_string = "hello world: 42!\n";
  text_handler::rx_callback ();

  auto return_code = text_handler::get_next_cmd (buffer, sizeof (buffer));

  EXPECT_EQ(mock_serial::input_string, "hello world: 42!");
  EXPECT_EQ(return_code, 0);
  EXPECT_STREQ(buffer, "hello world: 42!");
}

TEST_F(CliTests, rx_newline_carrige_return)
{
  mock_serial::output_string = "hello world: 42!\n\r";
  text_handler::rx_callback ();
  auto return_code = text_handler::get_next_cmd (buffer, sizeof (buffer));

  EXPECT_EQ(mock_serial::input_string, "hello world: 42!");
  EXPECT_EQ(return_code, 0);
  EXPECT_STREQ(buffer, "hello world: 42!");
}

TEST_F(CliTests, rx_carrige_return)
{
  mock_serial::output_string = "hello world: 42!\r";
  text_handler::rx_callback ();

  auto return_code = text_handler::get_next_cmd (buffer, sizeof (buffer));

  EXPECT_EQ(mock_serial::input_string, "hello world: 42!");
  EXPECT_EQ(return_code, 0);
  EXPECT_STREQ(buffer, "hello world: 42!");
}

TEST_F(CliTests, rx_new_data_before_get_cmd)
{
  mock_serial::output_string = "hello world: 42!\r";
  text_handler::rx_callback ();
  mock_serial::output_string = "garbage string\r";
  text_handler::rx_callback ();

  auto return_code = text_handler::get_next_cmd (buffer, sizeof (buffer));
  EXPECT_EQ(return_code, 0);
  EXPECT_STREQ(buffer, "hello world: 42!");
  return_code = text_handler::get_next_cmd (buffer, sizeof (buffer));

  EXPECT_EQ(mock_serial::input_string, "hello world: 42!garbage string");
  EXPECT_EQ(return_code, 0);
  EXPECT_STREQ(buffer, "garbage string");
}

TEST_F(CliTests, rx_backspace)
{
  mock_serial::output_string = "hello world: 42!\b\b\b\b\n";
  text_handler::rx_callback ();
  auto return_code = text_handler::get_next_cmd (buffer, sizeof (buffer));

  EXPECT_EQ(return_code, 0);

  EXPECT_STREQ(buffer, "hello world:");
}

TEST_F(CliTests, rx_move_and_backspace)
{
  mock_serial::output_string = "hello world: 42!\x1B[1D\b\n";
  text_handler::rx_callback ();
  auto return_code = text_handler::get_next_cmd (buffer, sizeof (buffer));

  EXPECT_EQ(return_code, 0);

  EXPECT_STREQ(buffer, "hello world: 4!");
}

TEST_F(CliTests, rx_move_and_type)
{
  mock_serial::output_string = "hello world: !\x1B[1D42\n";
  text_handler::rx_callback ();
  auto return_code = text_handler::get_next_cmd (buffer, sizeof (buffer));

  EXPECT_EQ(return_code, 0);

  EXPECT_STREQ(buffer, "hello world: 42");
}

TEST_F(CliTests, null_task_handle)
{
  xTaskGetCurrentTaskHandle_fake.return_val = nullptr;
  mock_serial::output_string = "hello world: 42!\n";
  text_handler::rx_callback ();

  EXPECT_EQ(xTaskGenericNotify_fake.call_count, 0) << "Task notify called with null param";
}

TEST_F(CliTests, task_notification)
{
  auto return_code = text_handler::get_next_cmd (buffer, sizeof (buffer));
  mock_serial::output_string = "hello";
  text_handler::rx_callback ();
  EXPECT_EQ(xTaskGenericNotify_fake.call_count, 0);
  mock_serial::output_string = "world: 42!\r";
  text_handler::rx_callback ();
  EXPECT_EQ(xTaskGenericNotify_fake.call_count, 1);

  mock_serial::output_string = "hello world: 42!";
  text_handler::rx_callback ();
  EXPECT_EQ(xTaskGenericNotify_fake.call_count, 1);


}
