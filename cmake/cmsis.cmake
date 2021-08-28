include(UsePackage)
message(STATUS "Using CMSIS ${CMSIS_VERSION}")


use_package(
    TARGET cmsis_core
    REMOTE "https://github.com/ARM-software/CMSIS_5.git"
    LOCAL "${LOCAL_REPO_DIR}/cmsis_core"
    GIT_TAG "${CMSIS_VERSION}"
)


add_library(cmsis_core INTERFACE)
target_include_directories(cmsis_core INTERFACE ${cmsis_core_SOURCE_DIR}/CMSIS/Core/Include)
add_library(cmsis::core ALIAS cmsis_core)

if (${MCU_LINE} STREQUAL stm32f0)
  set(${CMSIS_DEVICE_URL} https://github.com/STMicroelectronics/cmsis_device_f0.git)
else()
  message(FATAL_ERROR "Unknown MCU Line: ${MCU_LINE}")
endif ()

use_package(
    TARGET cmsis_device
    REMOTE "https://github.com/STMicroelectronics/cmsis_device_f0.git"
    LOCAL "${LOCAL_REPO_DIR}/cmsis_device"
)

add_library(cmsis_device INTERFACE)
target_include_directories(cmsis_device INTERFACE ${cmsis_device_SOURCE_DIR}/Include)
add_library(cmsis::device ALIAS cmsis_device)


add_library(cmsis INTERFACE)
target_link_libraries(cmsis INTERFACE cmsis::device cmsis::core)
add_library(cmsis::cmsis ALIAS cmsis)
