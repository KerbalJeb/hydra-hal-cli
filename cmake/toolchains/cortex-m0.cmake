# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m0)

# MCU specific flags
set(VFP_FLAGS "-mfloat-abi=soft")
set(LD_FLAGS "")

# Set the free rtos port directory for MCU
set (FREERTOS_PORT_DIR portable/GCC/ARM_CM0)

# Use the arm-none-eabi gcc toolchain
include(${CMAKE_CURRENT_LIST_DIR}/common/arm-none-eabi.cmake)
