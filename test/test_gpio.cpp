/// \file test_gpio.cpp
/// \brief Created on 2021-08-28 by Ben

#include <catch2/catch_test_macros.hpp>

#include <hh/gpio.hpp>

TEST_CASE("gpio config", "[gpio]")
{
    GPIO_TypeDef dummy_port{};
    hh::gpio::gpio_config_t config{};

    hh::gpio::config(&dummy_port, 0x1, config);
}