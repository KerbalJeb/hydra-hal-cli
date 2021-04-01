/** @file shell_tests.cpp.c
*   @brief Created on 2021-04-01 by Ben
*/
/**********************************************************************
* Includes
**********************************************************************/
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock_serial.hpp"
#include "shell.hpp"

/**********************************************************************
* Test Fixtures
**********************************************************************/

/**********************************************************************
* Tests
**********************************************************************/

class ShellTests: testing::Test{
 protected:
  void SetUp () override
  {
    mock_serial::clear ();
  }
  cli::shell<mock_serial> shell{};
};