cmake_minimum_required(VERSION 3.20)
project("Arm Cortex-M Base" C CXX ASM)
option(CLONE_TO_LOCAL OFF)

# C++ Standard, GNU extensions disabled
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Executable ELF File
set(EXECUTABLE_NAME sanity-check)
add_executable(${EXECUTABLE_NAME}.elf
        sanity-check.cpp
        ../startup.cpp
        ../vectors.cpp
        )

# Link Options/Libs
target_link_libraries(${EXECUTABLE_NAME}.elf cmsis::cmsis hh::peripheral)
target_link_options(${EXECUTABLE_NAME}.elf
        PRIVATE
        # Set custom linker script
        # todo make configuration file that picks up start and end of RAM/ROM from toolchain file
        -T ${CMAKE_SOURCE_DIR}/LinkerScript.ld
        # Link for no system calls
        --specs=nano.specs
        # Print memory usage and generate map file
        -Wl,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/map.map
        )

# Use objcopy to create bin, hex and asm files
set(HEX_FILE ${PROJECT_BINARY_DIR}/${EXECUTABLE_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${EXECUTABLE_NAME}.bin)
set(ASM_FILE ${PROJECT_BINARY_DIR}/${EXECUTABLE_NAME}.s)

add_custom_command(TARGET ${EXECUTABLE_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${EXECUTABLE_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${EXECUTABLE_NAME}.elf> ${BIN_FILE}
        COMMAND ${CMAKE_OBJDUMP} -d -C -S $<TARGET_FILE:${EXECUTABLE_NAME}.elf> > ${ASM_FILE}
        )

