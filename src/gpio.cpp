/// @file gpio.cpp
/// @brief Created on 2021-08-28 by Ben

#include <hh/gpio.hpp>

hh::status hh::gpio::config(hh::portable::gpio_ptr port, const hh::portable::pin_t& pins,
        const hh::gpio::gpio_config_t& config)
{
    return hh::status::success;
}
