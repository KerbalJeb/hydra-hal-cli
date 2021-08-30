#include <stm32f042x6.h>

int main()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER = 1;
    GPIOB->ODR = 1;
    while (true);
}
