/// @file stm32f0xx_port.h
/// @brief Created on 2021-08-28 by Ben

#pragma once

#include <cstdint>
#include <stm32f0xx.h>

namespace hh::portable {
using pin_t = std::uint32_t;
using pin_state_t = std::uint32_t;
using register_t = std::uint32_t;
using gpio_ptr = GPIO_TypeDef*;
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