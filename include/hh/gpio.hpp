/// @file gpio.hpp
/// @brief Created on 2021-08-28 by Ben

#pragma once
#include <port.hpp>

namespace hh::peripheral {

using callback_t = void (*)(port::gpio_ptr, port::pin_t);


struct gpio_config_t {
  enum class io_mode_t {
    input = port::GPIO_INPUT,
    output = port::GPIO_OUTPUT,
  };
  enum class pull_mode_t {
    none = port::GPIO_PULL_NONE,
    up = port::GPIO_PULL_UP,
    down = port::GPIO_PULL_DOWN,
  };
  enum class output_mode_t {
    push_pull = port::PUSH_PULL_OUTPUT,
    open_drain = port::OPEN_DRAIN_OUTPUT,
  };

  io_mode_t io_mode;
  pull_mode_t pull_mode;
  output_mode_t output_mode;
};

struct gpio_isr_config_t{
  enum class edge{
      rising,
      falling
  };
};

enum class gpio_status{
  success = 0, ///< successful operation
};

/// \brief
/// \pre
/// \post
/// \param gpio
/// \param pin
/// \param config
/// \return
int gpio_config(port::gpio_ptr gpio, const port::pin_t& pin, const gpio_config_t& config);
/// \brief
/// \param gpio
/// \param pin
/// \param config
/// \param af_config
/// \return
int gpio_config_af(port::gpio_ptr gpio, const port::pin_t& pin, const gpio_config_t& config,
        const port::gpio_af_config_t& af_config);
/// \brief
/// \param gpio
/// \param pin
/// \param config
/// \param callback
/// \return
int gpio_config_isr(port::gpio_ptr gpio, const port::pin_t& pin, const gpio_isr_config_t& config, callback_t callback);
/// \brief
/// \param gpio
/// \param pin
/// \param state
/// \return
int gpio_write(port::gpio_ptr gpio, const port::pin_t& pin, port::pin_state_t state);
/// \brief
/// \param gpio
/// \param pin
/// \return
port::pin_state_t gpio_read(port::gpio_ptr gpio, const port::pin_t& pin);
/// \brief
/// \param reg
/// \return
port::register_t gpio_read_reg(port::register_t* reg);
/// \brief
/// \param reg
/// \param value
/// \return
int gpio_write_reg(port::register_t* reg, port::register_t value);
}
