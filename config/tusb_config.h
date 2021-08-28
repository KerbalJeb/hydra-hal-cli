/** @file tusb_config.h
*   @brief Created on 2021-03-20 by Ben
*/
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
// DEVICE CONFIGURATION
//--------------------------------------------------------------------

#define BOARD_DEVICE_RHPORT_SPEED  OPT_MODE_FULL_SPEED
#define CFG_TUSB_RHPORT0_MODE      (OPT_MODE_DEVICE | BOARD_DEVICE_RHPORT_SPEED)
#define CFG_TUD_ENDPOINT0_SIZE    64
#define CFG_TUSB_MCU              OPT_MCU_STM32F0
#define CFG_TUSB_OS               OPT_OS_FREERTOS


//------------- CLASS -------------//
#define CFG_TUD_CDC               1
#define CFG_TUD_MSC               0
#define CFG_TUD_HID               0
#define CFG_TUD_MIDI              0
#define CFG_TUD_VENDOR            0

// MIDI FIFO size of TX and RX
#define CFG_TUD_CDC_RX_BUFSIZE   (TUD_OPT_HIGH_SPEED ? 512 : 64)
#define CFG_TUD_CDC_TX_BUFSIZE   (TUD_OPT_HIGH_SPEED ? 512 : 64)

#ifdef __cplusplus
}
#endif
