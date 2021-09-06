# Requires TINYUSB_VERSION to be set to a valid git tag
# Requires ${CONFIG_DIR}/usb_descriptors.c to be present
include(UsePackage)
message(STATUS "Using TinyUSB ${TINYUSB_VERSION}")

use_package(
        TARGET tinyusb
        GIT_REPOSITORY "https://github.com/hathach/tinyusb.git"
        GIT_TAG "${TINYUSB_VERSION}"
        GIT_SUBMODULES ""
)

set(tinyusb_SOURCE_DIR ${tinyusb_SOURCE_DIR}/src)
cmake_path(GET TINYUSB_PORT ROOT_PATH TINYUSB_PORT_DIR)

add_library(tinyusb
        ${tinyusb_SOURCE_DIR}/class/cdc/cdc_device.c
        ${tinyusb_SOURCE_DIR}/device/usbd.c
        ${tinyusb_SOURCE_DIR}/device/usbd_control.c
        ${tinyusb_SOURCE_DIR}/common/tusb_fifo.c
        ${tinyusb_SOURCE_DIR}/tusb.c
        ${tinyusb_SOURCE_DIR}/${TINYUSB_PORT}
        ${CONFIG_DIR}/usb_descriptors.c
        )

target_include_directories(tinyusb
        PUBLIC
        ${tinyusb_SOURCE_DIR}
        ${tinyusb_SOURCE_DIR}/${TINYUSB_PORT_DIR}
        ${CONFIG_DIR}
        )

target_link_libraries(tinyusb cmsis::cmsis freertos::freertos)

add_library(tinyusb::tinyusb ALIAS tinyusb)