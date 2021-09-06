/// \file test_gpio.cpp
/// \brief Created on 2021-08-28 by Ben

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <tuple>

#include <hh/peripheral/gpio.hpp>

using namespace hh::gpio;

TEST_CASE("gpio basic config", "[gpio][config]")
{
    GPIO_TypeDef dummy_port{};

    io_mode_t io_mode = GENERATE(io_mode_t::input, io_mode_t::output);
    pull_mode_t pull_mode = GENERATE(pull_mode_t::down, pull_mode_t::none, pull_mode_t::up);
    output_mode_t output_mode = GENERATE(output_mode_t::open_drain, output_mode_t::push_pull);
    gpio_config_t test_config{io_mode, pull_mode, output_mode};

    CAPTURE(io_mode, pull_mode, output_mode);

    CHECK(config(&dummy_port, 0x1, test_config)==hh::status::success);

    CHECK(dummy_port.ODR==0);
    CHECK(dummy_port.MODER==(std::uint32_t)test_config.io_mode);
    CHECK(dummy_port.AFR[0]==0);
    CHECK(dummy_port.AFR[1]==0);
    CHECK(dummy_port.BRR==0);
    CHECK(dummy_port.BSRR==0);
    CHECK(dummy_port.IDR==0);
    CHECK(dummy_port.LCKR==0);
    CHECK(dummy_port.OSPEEDR==0);
    CHECK(dummy_port.OTYPER==(std::uint32_t)test_config.output_mode);
    CHECK(dummy_port.PUPDR==(std::uint32_t)test_config.pull_mode);
}

TEST_CASE("gpio multi pin config", "[gpio][config]")
{
    GPIO_TypeDef dummy_port{};

    io_mode_t io_mode = GENERATE(io_mode_t::input, io_mode_t::output);
    pull_mode_t pull_mode = GENERATE(pull_mode_t::down, pull_mode_t::none, pull_mode_t::up);
    output_mode_t output_mode = GENERATE(output_mode_t::open_drain, output_mode_t::push_pull);
    gpio_config_t test_config{io_mode, pull_mode, output_mode};
    CAPTURE(io_mode, pull_mode, output_mode);

    CHECK(config(&dummy_port, 0x3 | 1 << 15, test_config)==hh::status::success);

    auto moder_val = (std::uint32_t)test_config.io_mode;
    auto expected_moder = moder_val | (moder_val << 2*1) | (moder_val << 2*15);

    auto otyper_val = (std::uint32_t)test_config.output_mode;
    auto expected_otyper = otyper_val | (otyper_val << 1) | (otyper_val << 15);

    auto pull_val = (std::uint32_t)test_config.pull_mode;
    auto expected_pull = pull_val | (pull_val << 2) | (pull_val << 2*15);

    CHECK(dummy_port.ODR==0);
    CHECK(dummy_port.MODER==expected_moder);
    CHECK(dummy_port.AFR[0]==0);
    CHECK(dummy_port.AFR[1]==0);
    CHECK(dummy_port.BRR==0);
    CHECK(dummy_port.BSRR==0);
    CHECK(dummy_port.IDR==0);
    CHECK(dummy_port.LCKR==0);
    CHECK(dummy_port.OSPEEDR==0);
    CHECK(dummy_port.OTYPER==expected_otyper);
    CHECK(dummy_port.PUPDR==expected_pull);
}

TEST_CASE("gpio write", "[gpio][write]")
{
    GPIO_TypeDef dummy_port{};
    using namespace hh::portable;
    auto[initial_value, pin_msk, pin_values, expected_value] = GENERATE(
            std::tuple<int, int, int, int>{0x0000, pin_0, pin_0, 0x001}, // Basic
            std::tuple<int, int, int, int>{0x0040, pin_15, pin_15, 0x8040}, // non zero
            std::tuple<int, int, int, int>{0xffff, pin_0, 0, 0xfffe}, // bit clear
            std::tuple<int, int, int, int>{0x0000, pin_0 | pin_5, pin_5, pin_5}, // multi pin
            std::tuple<int, int, int, int>{0xffff, pin_0 | pin_5, pin_5, 0xfffe} // multi pin
    );

    CAPTURE(initial_value, expected_value, pin_msk, pin_values);

    dummy_port.ODR = initial_value;
    hh::gpio::write(&dummy_port, pin_msk, pin_values);

    CHECK(dummy_port.ODR==expected_value);
    CHECK(dummy_port.MODER==0);
    CHECK(dummy_port.AFR[0]==0);
    CHECK(dummy_port.AFR[1]==0);
    CHECK(dummy_port.BRR==0);
    CHECK(dummy_port.BSRR==0);
    CHECK(dummy_port.IDR==0);
    CHECK(dummy_port.LCKR==0);
    CHECK(dummy_port.OSPEEDR==0);
    CHECK(dummy_port.OTYPER==0);
    CHECK(dummy_port.PUPDR==0);
}

TEST_CASE("gpio write high", "[gpio][write]")
{
    GPIO_TypeDef dummy_port{};
    using namespace hh::portable;

    auto pins_to_set = GENERATE(pin_0, pin_15 | pin_0, pin_5 | pin_6);

    hh::gpio::set(&dummy_port, pins_to_set);

    CHECK(dummy_port.ODR==0);
    CHECK(dummy_port.MODER==0);
    CHECK(dummy_port.AFR[0]==0);
    CHECK(dummy_port.AFR[1]==0);
    CHECK(dummy_port.BRR==0);
    CHECK(dummy_port.BSRR==pins_to_set << 15);
    CHECK(dummy_port.IDR==0);
    CHECK(dummy_port.LCKR==0);
    CHECK(dummy_port.OSPEEDR==0);
    CHECK(dummy_port.OTYPER==0);
    CHECK(dummy_port.PUPDR==0);
}

TEST_CASE("gpio write low", "[gpio][write]")
{
    GPIO_TypeDef dummy_port{};
    using namespace hh::portable;

    auto pins_to_set = GENERATE(pin_0, pin_15 | pin_0, pin_5 | pin_6);

    hh::gpio::reset(&dummy_port, pins_to_set);

    CHECK(dummy_port.ODR==0);
    CHECK(dummy_port.MODER==0);
    CHECK(dummy_port.AFR[0]==0);
    CHECK(dummy_port.AFR[1]==0);
    CHECK(dummy_port.BRR==0);
    CHECK(dummy_port.BSRR==pins_to_set);
    CHECK(dummy_port.IDR==0);
    CHECK(dummy_port.LCKR==0);
    CHECK(dummy_port.OSPEEDR==0);
    CHECK(dummy_port.OTYPER==0);
    CHECK(dummy_port.PUPDR==0);
}

TEST_CASE("gpio read", "[gpio][write]")
{
    GPIO_TypeDef dummy_port{};
    using namespace hh::portable;

    auto[pin_values, pins_to_read] = GENERATE(
            std::tuple<int, int>{0x0001, pin_0},
            std::tuple<int, int>{0x8001, pin_0 | pin_15},
            std::tuple<int, int>{0x0001, pin_0 | pin_15}
    );

    dummy_port.IDR = pin_values;
    auto ret_value = hh::gpio::read(&dummy_port, pins_to_read);
    CHECK(ret_value == (pin_values & pins_to_read));
}

TEST_CASE("gpio port enable", "[gpio][config]")
{
    GPIO_TypeDef dummy_ports[6];
    using namespace hh::portable;
    RCC_TypeDef dummy_rcc{};
    RCC = &dummy_rcc;
    GPIOA = &dummy_ports[0];
    GPIOB = &dummy_ports[1];
    GPIOC = &dummy_ports[2];
    GPIOF = &dummy_ports[5];

    auto [port, expected] = GENERATE(
        std::tuple{GPIOA, RCC_AHBENR_GPIOAEN},
        std::tuple{GPIOB, RCC_AHBENR_GPIOBEN},
        std::tuple{GPIOC, RCC_AHBENR_GPIOCEN},
        std::tuple{GPIOF, RCC_AHBENR_GPIOFEN}
    );

    hh::gpio::enable_port(port);
    CHECK(RCC->AHBENR == expected);
}