/// \file gpio.cpp
/// \brief Created on 2021-08-28 by Ben

#include <hh/gpio.hpp>

hh::status hh::gpio::config(hh::portable::gpio_ptr port, unsigned int pins,
        const hh::gpio::gpio_config_t& config)
{
    unsigned int idx = 0U;
    do {
        if (pins & 1U) {
            auto moder_val = static_cast<std::uint32_t>(config.io_mode) << 2U*idx;
            auto pupdr_val = static_cast<std::uint32_t>(config.pull_mode) << 2U*idx;
            auto otyper_val = static_cast<std::uint32_t>(config.output_mode) << idx;

            auto msk_2bit = 0b11 << 2U*idx;
            auto msk_1bit = 0b1 << idx;

            write_with_msk(port->MODER, moder_val, msk_2bit);
            write_with_msk(port->PUPDR, pupdr_val, msk_2bit);
            write_with_msk(port->OTYPER, otyper_val, msk_1bit);
        }
        ++idx;
    }
    while (pins >>= 1U);
    return hh::status::success;
}
