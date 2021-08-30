/// \file gpio.cpp
/// \brief Created on 2021-08-28 by Ben

#include <hh/gpio.hpp>
#include <hh/hal_assert.hpp>

hh::status hh::gpio::config(hh::portable::gpio_port_t port, portable::pin_t pins,
        const hh::gpio::gpio_config_t& config)
{
    unsigned int idx = 0U;
    do {
        if (pins & 1U) {
            auto moder_val = (std::uint32_t)config.io_mode << 2U*idx;
            auto pupdr_val = (std::uint32_t)config.pull_mode << 2U*idx;
            auto otyper_val = (std::uint32_t)config.output_mode << idx;

            auto msk_2_bit = 0b11 << 2U*idx;
            auto msk_1_bit = 0b1 << idx;

            write_with_msk(port->MODER, moder_val, msk_2_bit);
            write_with_msk(port->PUPDR, pupdr_val, msk_2_bit);
            write_with_msk(port->OTYPER, otyper_val, msk_1_bit);
        }
        ++idx;
    }
    while (pins >>= 1U);
    return hh::status::success;
}

hh::status hh::gpio::config(hh::portable::gpio_port_t port, hh::portable::pin_t pins,
        const hh::gpio::gpio_config_t& config, const hh::portable::gpio_af_config_t& af_config)
{
    /// \todo implement af config overload
    return hh::status::success;
}

hh::status hh::gpio::set_isr_callback(hh::portable::gpio_port_t port, hh::portable::pin_t pins,
        const hh::gpio::isr_condition& condition, hh::gpio::callback_t callback)
{
    /// \todo implement set isr callback
    return hh::status::success;
}

hh::status hh::gpio::write(hh::portable::gpio_port_t port, hh::portable::pin_t pins, hh::portable::pin_state_t state)
{
    assert((~pins & state)==0U);
    write_with_msk(port->ODR, state, pins);
    return hh::status::success;
}

hh::status hh::gpio::set(hh::portable::gpio_port_t port, hh::portable::pin_t pins)
{
    port->BSRR = pins << 15;
    return hh::status::success;
}

hh::status hh::gpio::reset(hh::portable::gpio_port_t port, hh::portable::pin_t pins)
{
    port->BSRR = pins;
    return hh::status::success;
}

hh::portable::pin_state_t hh::gpio::read(hh::portable::gpio_port_t port, hh::portable::pin_t pins)
{
    return port->IDR & pins;
}

hh::status hh::gpio::enable_port(hh::portable::gpio_port_t port)
{
    auto gpio_idx = port-GPIOA;
    set_bits(RCC->AHBENR, 1 << (RCC_AHBENR_GPIOAEN_Pos+gpio_idx));
    return hh::status::success;
}
