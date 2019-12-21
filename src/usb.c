#include <debug.h>
#include "config.h"
#include "usb.h"
#include "gamestate.h"
#include "lib/steam_controller.h"
#include "hid/hid.h"

#if USE_USB

static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
                                    usb_callback_data_t *callback_data);
static hid_state_t *get_hid_slot(void);

static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
                                    usb_callback_data_t *callback_data) {
    switch(event) {
        case USB_DEVICE_CONNECTED_EVENT: {
            usb_device_t dev = event_data;
            hid_state_t *hid = get_hid_slot();
            int8_t interface;
            dbg_sprintf(dbgout, "got device\n");
            game.usb.controller_dev = dev;
            usb_ResetDevice(dev);
            usb_WaitForEvents();
            interface = sc_GetInterface(dev);
            if(interface != -1) {
                uint8_t conf_desc[256];
                size_t len;
                dbg_sprintf(dbgout, "got steam interface %u\n", interface);
                game.usb.controller_interface = interface;
                len = usb_GetConfigurationDescriptorTotalLength(dev, 0);
                usb_GetDescriptor(dev, USB_CONFIGURATION_DESCRIPTOR, 0,
                                  &conf_desc, 256, NULL);
                usb_SetConfiguration(dev, (usb_configuration_descriptor_t*)&conf_desc, len);
            }

            if(hid) {
                uint8_t i;
                union conf_desc {
                    uint8_t bytes[256];
                    usb_configuration_descriptor_t conf;
                    usb_descriptor_t desc;
                } conf_desc;

                size_t len;

                len = usb_GetConfigurationDescriptorTotalLength(dev, 0);
                usb_GetDescriptor(dev, USB_CONFIGURATION_DESCRIPTOR, 0,
                                  &conf_desc, 256, NULL);
                usb_SetConfiguration(dev, (usb_configuration_descriptor_t*)&conf_desc, len);

                for(i = 0; i < conf_desc.conf.bNumInterfaces; i++) {
                    hid_error_t error;

                    error = hid_Init(hid, dev, i);
                    if(!error) {
                        dbg_sprintf(dbgout, "got interface %u\n", i);
                        hid_SetIdleTime(hid, 0);
                        hid = get_hid_slot();
                        if(!hid) {
                            dbg_sprintf(dbgout, "Ran out of HID slots\n");
                            return USB_SUCCESS;
                        }
                    }
                }
            } else {
                dbg_sprintf(dbgout, "No HID slots available\n");
                return USB_IGNORE;
            }

            break;
        }
        case USB_DEVICE_DISCONNECTED_EVENT: {
            dbg_sprintf(dbgout, "device disconnected\n");
            break;
        }
        default:
            break;
    }
    return USB_SUCCESS;
}

void start_usb(void) {
    dbg_sprintf(dbgout, "initting usb\n");
    usb_Init(handle_usb_event, NULL, NULL, USB_DEFAULT_INIT_FLAGS);
}

static hid_state_t *get_hid_slot(void) {
    uint8_t i;
    for(i = 0; i < MAX_HID_DEVICES; i++) {
        if(!game.usb.hids[i].active) return &game.usb.hids[i];
    }
    return NULL;
}

bool any_hid_held(uint8_t key_code) {
    uint8_t i;
    for(i = 0; i < MAX_HID_DEVICES; i++) {
        if(game.usb.hids[i].active) {
            if(hid_KbdIsKeyDown(&game.usb.hids[i], key_code)) return true;
        }
    }
    return false;
}

bool any_hid_mouse_held(uint8_t button) {
    uint8_t i;
    for(i = 0; i < MAX_HID_DEVICES; i++) {
        if(game.usb.hids[i].active) {
            if(hid_MouseIsButtonDown(&game.usb.hids[i], button)) return true;
        }
    }
    return false;
}

#endif
