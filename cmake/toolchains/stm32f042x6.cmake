include(${CMAKE_CURRENT_LIST_DIR}/cortex-m0.cmake)

# Set MCU specific definitions
set(MCU_NAME stm32f042x6)
set(MCU_LINE stm32f0)

# Set library specific port paths

# CMSIS Device paths
set(START_UP_PATH Source/Templates/gcc/startup_${MCU_NAME}.s)
set(SYSTEM_UP_PATH Source/Templates/system_stm32f0xx.c)

# Tiny USB port path
set(TINYUSB_PORT portable/st/stm32_fsdev/dcd_stm32_fsdev.c)

# Required for some libraries
add_definitions(-DSTM32F0)
add_definitions(-DSTM32F042x6)
