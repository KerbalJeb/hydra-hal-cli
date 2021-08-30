/** @file usb_descriptors.c
*   @brief Created on 2021-03-20 by Ben
*/
/**********************************************************************
* Includes
**********************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <tusb.h>
/**********************************************************************
* Module Preprocessor Constants
**********************************************************************/

/**********************************************************************
* Module Preprocessor Macros
**********************************************************************/

#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
#define USB_PID           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                           _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )


#define CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN)

#define EPNUM_CDC_NOTIF   0x81
#define EPNUM_CDC_OUT     0x02
#define EPNUM_CDC_IN      0x82

#define EPNUM_MSC_OUT     0x03
#define EPNUM_MSC_IN      0x83

/**********************************************************************
* Module Typedefs
**********************************************************************/
enum
{
  ITF_NUM_CDC = 0,
  ITF_NUM_CDC_DATA,
  ITF_NUM_TOTAL
};

/*********************************************************************
* Module Variable Definitions
**********************************************************************/

tusb_desc_device_t const desc_device =
                             {
                                 .bLength            = sizeof(tusb_desc_device_t),
                                 .bDescriptorType    = TUSB_DESC_DEVICE,
                                 .bcdUSB             = 0x0200,

                                 // Use Interface Association Descriptor (IAD) for CDC
                                 // As required by USB Specs IAD's subclass must be common class (2) and protocol must be IAD (1)
                                 .bDeviceClass       = TUSB_CLASS_CDC,
                                 .bDeviceSubClass    = CDC_COMM_SUBCLASS_ABSTRACT_CONTROL_MODEL,
                                 .bDeviceProtocol    = CDC_COMM_PROTOCOL_NONE,

                                 .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

                                 .idVendor           = 0xCafe,
                                 .idProduct          = USB_PID,
                                 .bcdDevice          = 0x0100,

                                 .iManufacturer      = 0x01,
                                 .iProduct           = 0x02,
                                 .iSerialNumber      = 0x03,

                                 .bNumConfigurations = 0x01
                             };

uint8_t const desc_fs_configuration[] =
                  {
                      // Config number, interface count, string index, total length, attribute, power in mA
                      TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_SELF_POWERED, 1),

                      // Interface number, string index, EP notification address and size, EP data address (out, in) and size.
                      TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64),
                  };

// array of pointer to string descriptors
char const* string_desc_arr [] =
              {
                  (const char[]) { 0x09, 0x04 }, // 0: is supported language is English (0x0409)
                  "TinyUSB",                     // 1: Manufacturer
                  "TinyUSB Device",              // 2: Product
                  "123456",                      // 3: Serials, should use chip ID
                  "TinyUSB CDC",                 // 4: CDC Interface
              };

static uint16_t _desc_str[32];

/**********************************************************************
* Function Prototypes
**********************************************************************/

/**********************************************************************
* Function Definitions
**********************************************************************/

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const * tud_descriptor_device_cb(void)
{
  return (uint8_t const *) &desc_device;
}

uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
  (void) index;
  return desc_fs_configuration;
}

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void) langid;

  uint8_t chr_count;

  if ( index == 0)
  {
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
  }else
  {
    // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
    // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

    if (index >= sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) return NULL;

    const char* str = string_desc_arr[index];

    // Cap at max char
    chr_count = strlen(str);
    if ( chr_count > 31 ) chr_count = 31;

    // Convert ASCII string into UTF-16
    for(uint8_t i=0; i<chr_count; i++)
    {
      _desc_str[1+i] = str[i];
    }
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);

  return _desc_str;
}