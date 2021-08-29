/// @file common.hpp
/// @brief Created on 2021-08-28 by Ben

#pragma once

namespace hh{
/// \brief A status return code
enum class status {
  success = 0,
  invalid_args,
  configuration_failed,
};
}