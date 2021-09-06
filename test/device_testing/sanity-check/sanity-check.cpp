#include <hh/peripheral/gpio.hpp>
#include <stm32f042x6.h>

int main()
{
    hh::gpio::enable_port(GPIOA);
    hh::gpio::gpio_config_t gpio_config{
            hh::gpio::io_mode_t::input,
            hh::gpio::pull_mode_t::none,
            hh::gpio::output_mode_t::push_pull
    };
    hh::gpio::config(GPIOB, hh::portable::pin_0, gpio_config);
    hh::gpio::write(GPIOB, hh::portable::pin_0, 0x1);

    while (true);
}
