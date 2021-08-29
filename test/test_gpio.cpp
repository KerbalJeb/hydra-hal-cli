/// \file test_gpio.cpp
/// \brief Created on 2021-08-28 by Ben

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <hh/gpio.hpp>

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