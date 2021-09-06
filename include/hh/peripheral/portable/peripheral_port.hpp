/// \file port.hpp
/// \brief Created on 2021-08-28 by Ben

#pragma once

#if defined(STM32F030x6) || defined(STM32F030x8) || defined(STM32F031x6) || defined(STM32F038xx) ||     \
        defined(STM32F042x6) || defined(STM32F048xx) || defined(STM32F051x8) || defined(STM32F058xx) || \
        defined(STM32F070x6) || defined(STM32F070xB) || defined(STM32F071xB) || defined(STM32F072xB) || \
        defined(STM32F078xx) || defined(STM32F030xC) || defined(STM32F091xC) || defined(STM32F098xx)
#include <hh/peripheral/portable/vendor/st/stm32f0xx_peripheral_port.h>
#else
#error "No target selected"
#endif
