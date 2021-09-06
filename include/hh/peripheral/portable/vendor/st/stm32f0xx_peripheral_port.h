/// \file stm32f0xx_peripheral_port.h
/// \brief Created on 2021-08-28 by Ben

#pragma once

#include <cstdint>
#include <stm32f0xx.h>

namespace hh::portable {

/// \section GPIO defs

enum pin_enumeration{
  pin_0 = 1 << 0,
  pin_1 = 1 << 1,
  pin_3 = 1 << 3,
  pin_4 = 1 << 4,
  pin_5 = 1 << 5,
  pin_6 = 1 << 6,
  pin_7 = 1 << 7,
  pin_8 = 1 << 8,
  pin_9 = 1 << 9,
  pin_10 = 1 << 10,
  pin_11 = 1 << 11,
  pin_12 = 1 << 12,
  pin_13 = 1 << 13,
  pin_14 = 1 << 14,
  pin_15 = 1 << 15,
};

using pin_t = std::uint32_t;
using pin_state_t = std::uint32_t;
using register_t = std::uint32_t;
using gpio_port_t = GPIO_TypeDef*;
using gpio_af_config_t = std::uint32_t;

// GPIO mode degs
constexpr register_t GPIO_INPUT = 0b00;
constexpr register_t GPIO_OUTPUT = 0b00;

constexpr register_t GPIO_PULL_NONE = 0b00;
constexpr register_t GPIO_PULL_UP = 0b01;
constexpr register_t GPIO_PULL_DOWN = 0b10;

constexpr register_t PUSH_PULL_OUTPUT = 0b0;
constexpr register_t OPEN_DRAIN_OUTPUT = 0b1;
}