/// @file gpio.hpp
/// @brief Created on 2021-08-28 by Ben
/// \todo analog io functions

#pragma once
#include <port.hpp>
#include <hh/common.hpp>

namespace hh::gpio {

using callback_t = void (*)(portable::pin_t);

struct gpio_config_t {
  enum class io_mode_t {
    input = portable::GPIO_INPUT,
    output = portable::GPIO_OUTPUT,
  };
  enum class pull_mode_t {
    none = portable::GPIO_PULL_NONE,
    up = portable::GPIO_PULL_UP,
    down = portable::GPIO_PULL_DOWN,
  };
  enum class output_mode_t {
    push_pull = portable::PUSH_PULL_OUTPUT,
    open_drain = portable::OPEN_DRAIN_OUTPUT,
  };

  io_mode_t io_mode;
  pull_mode_t pull_mode;
  output_mode_t output_mode;
};

struct isr_condition {
  enum class edge {
    rising,
    falling
  };
};


/// \brief Performs basic configuration of one or more GPIO pins
/// \pre The clock to the GPIO port being used must be enabled.
/// All argument parameters must be valid, no internal parameter checking is guaranteed to be performed.
/// \post The given pins will be set according to the provided configuration
/// and the configurations of all other pins will be preserved
/// \param port A pointer to the GPIO port hardware registers
/// \param pins An unsigned int with bits corresponding pins to configure are set,
/// where the nth bit corresponds to the nth pin on the GPIO port.
/// \param config The configuration to apply to all pins
/// \return The status of the operation
status config(portable::gpio_ptr port, const portable::pin_t& pins, const gpio_config_t& config);

/// \brief Initializes one or more GPIO pins in alternate function mode
/// \pre The clock to the GPIO port being used must be enabled.
/// All argument parameters must be valid, no internal parameter checking is guaranteed to be performed.
/// \post The given pins will be set according to the provided configuration
/// and the configurations of all other pins will be preserved
/// \param port A pointer to the GPIO port hardware registers
/// \param pins An unsigned int with bits corresponding pins to configure are set,
/// where the nth bit corresponds to the nth pin on the GPIO port.
/// \param config The configuration to apply to all pins
/// \param af_config The alternate function configuration, device dependant
/// \return The status of the operation
status config(portable::gpio_ptr port, const portable::pin_t& pins, const gpio_config_t& config,
        const portable::gpio_af_config_t& af_config);

/// \brief Sets a callback for the gpio pin(s) isr
/// \pre The pin must have been initialized via a config function
/// \post The callback function will be called whenever the isr_condition is staved
/// \param port A pointer to the GPIO port hardware registers
/// \param pins An unsigned int with bits corresponding pins to configure are set,
///// where the nth bit corresponds to the nth pin on the GPIO port.
/// \param condition The ISR configuration
/// \param callback The callback function to register
/// \return The status of the operation
status set_isr_callback(portable::gpio_ptr port, const portable::pin_t& pins, const isr_condition& condition,
        callback_t callback);

/// \brief Sets the state of one or more output pins
/// \pre The pin(s) must be configured as an output pin
/// \post The pin(s) state will be set according the the state parameter
/// \param port A pointer to the GPIO port hardware registers
/// \param pins An unsigned int with bits corresponding pins to configure are set,
///// where the nth bit corresponds to the nth pin on the GPIO port.
/// \param state The new states of the pins as an unsigned int, where the nth bit corresponds to the nth pin
/// \return The status of the operation
status write(portable::gpio_ptr port, const portable::pin_t& pins, portable::pin_state_t state);

/// \brief Sets one or more output pins to high
/// \pre The pin(s) must be configured as an output pin
/// \post The pin(s) state will be set to high
/// \param port A pointer to the GPIO port hardware registers
/// \param pins An unsigned int with bits corresponding pins to configure are set,
///// where the nth bit corresponds to the nth pin on the GPIO port.
/// \return The status of the operation
status set(portable::gpio_ptr port, const portable::pin_t& pins);

/// \brief Sets one or more output pins to low
/// \pre The pin(s) must be configured as an output pin
/// \post The pin(s) state will be set to low
/// \param port A pointer to the GPIO port hardware registers
/// \param pins An unsigned int with bits corresponding pins to configure are set,
///// where the nth bit corresponds to the nth pin on the GPIO port.
/// \return The status of the operation
status reset(portable::gpio_ptr port, const portable::pin_t& pin);

/// \brief Read the state of one or more gpio pins
/// \param port A pointer to the GPIO port hardware registers
/// \param pins An unsigned int with bits corresponding pins to read are set,
///// where the nth bit corresponds to the nth pin on the GPIO port.
/// \return The states of the pins as an unsigned int, where the nth bit corresponds to the nth pin
portable::pin_state_t read(portable::gpio_ptr port, const portable::pin_t& pins);

/// \brief Read from a gpio register
/// \param reg A pointer to the register
/// \return The new register value
portable::register_t read_reg(portable::register_t* reg);

/// \brief Write to a gpio register
/// \param reg A pointer to the register
/// \param value The value to write to the register
/// \return The new register value
portable::register_t  write_reg(portable::register_t* reg, portable::register_t value);

/// \brief Gets a volatile reference to a gpio register
/// \pre reg must be the address of a valid register
/// \param reg The numerical address of the register
/// \return A reference to the register
volatile portable::register_t& get_reg(std::uintptr_t reg);
}
