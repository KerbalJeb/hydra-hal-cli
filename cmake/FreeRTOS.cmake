include(UsePackage)
message(STATUS "Using FreeRTOS ${FREERTOS_VERSION}")

use_package(
        TARGET freertos
        GIT_REPOSITORY "https://github.com/FreeRTOS/FreeRTOS-Kernel.git"
        GIT_TAG "${FREERTOS_VERSION}"
)

add_library(freertos
            ${freertos_SOURCE_DIR}/croutine.c
            ${freertos_SOURCE_DIR}/event_groups.c
            ${freertos_SOURCE_DIR}/list.c
            ${freertos_SOURCE_DIR}/queue.c
            ${freertos_SOURCE_DIR}/stream_buffer.c
            ${freertos_SOURCE_DIR}/tasks.c
            ${freertos_SOURCE_DIR}/timers.c
            ${freertos_SOURCE_DIR}/${FREERTOS_PORT_DIR}/port.c
            ${CONFIG_DIR}/FreeRTOSApp.c
            )

target_include_directories(freertos
                           PUBLIC
                           ${freertos_SOURCE_DIR}/include
                           ${freertos_SOURCE_DIR}/${FREERTOS_PORT_DIR}
                           ${CONFIG_DIR}
                           )

add_library(freertos::freertos ALIAS freertos)
