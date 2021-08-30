/// @file gpio.hpp
/// @brief Created on 2021-08-28 by Ben
/// \todo analog io functions

#pragma once
#include <hh/portable/peripheral_port.hpp>
#include <hh/common.hpp>

namespace hh::gpio {

/// \brief The gpio isr callback function pointer type
using callback_t = void (*)(portable::pin_t);

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

/// \brief Portable GPIO configuration struct
struct gpio_config_t {
  io_mode_t io_mode;
  pull_mode_t pull_mode;
  output_mode_t output_mode;
};

/// \brief Portable ISR trigger condition type
struct isr_condition {
  enum class edge {
    rising,
    falling
  };
};

/// \brief
/// \param port
/// \return The status of the operation
status enable_port(hh::portable::gpio_port_t port);

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
status config(portable::gpio_port_t port, portable::pin_t pins, const gpio_config_t& config);

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
status config(portable::gpio_port_t port, portable::pin_t pins, const gpio_config_t& config,
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
status set_isr_callback(portable::gpio_port_t port, portable::pin_t pins, const isr_condition& condition,
        callback_t callback);

/// \brief Sets the state of one or more output pins
/// \pre The pin(s) must be configured as an output pin
/// \post The pin(s) state will be set according the the state parameter
/// \param port A pointer to the GPIO port hardware registers
/// \param pins An unsigned int with bits corresponding pins to configure are set,
///// where the nth bit corresponds to the nth pin on the GPIO port.
/// \param state The new states of the pins as an unsigned int, where the nth bit corresponds to the nth pin
/// \return The status of the operation
status write(portable::gpio_port_t port, portable::pin_t pins, portable::pin_state_t state);

/// \brief Sets one or more output pins to high
/// \pre The pin(s) must be configured as an output pin
/// \post The pin(s) state will be set to high
/// \param port A pointer to the GPIO port hardware registers
/// \param pins An unsigned int with bits corresponding pins to configure are set,
///// where the nth bit corresponds to the nth pin on the GPIO port.
/// \return The status of the operation
status set(portable::gpio_port_t port, portable::pin_t pins);

/// \brief Sets one or more output pins to low
/// \pre The pin(s) must be configured as an output pin
/// \post The pin(s) state will be set to low
/// \param port A pointer to the GPIO port hardware registers
/// \param pins An unsigned int with bits corresponding pins to configure are set,
///// where the nth bit corresponds to the nth pin on the GPIO port.
/// \return The status of the operation
status reset(portable::gpio_port_t port, portable::pin_t pins);

/// \brief Read the state of one or more gpio pins
/// \param port A pointer to the GPIO port hardware registers
/// \param pins An unsigned int with bits corresponding pins to read are set,
///// where the nth bit corresponds to the nth pin on the GPIO port.
/// \return The states of the pins as an unsigned int, where the nth bit corresponds to the nth pin
portable::pin_state_t read(portable::gpio_port_t port, portable::pin_t pins);

}
