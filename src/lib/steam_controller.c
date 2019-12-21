#include "../config.h"
#if USE_USB
#include <usbdrvce.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>
#include <graphx.h>
#include "steam_controller.h"

#define VENDOR_STEAM  0x28DE
#define DEVICE_WIRED  0x1102
#define DEVICE_DONGLE 0x1142

int8_t sc_GetInterface(usb_device_t dev) {
    usb_device_descriptor_t descriptor;
    usb_GetDescriptor(dev, USB_DEVICE_DESCRIPTOR, 0,
                      &descriptor, sizeof(descriptor), NULL);
    if(descriptor.idVendor != VENDOR_STEAM) return -1;
    switch(descriptor.idProduct) {
        case DEVICE_WIRED:
            return 2;
        case DEVICE_DONGLE:
            return 1;
        default:
            return -1;
    }
}

struct DataPacket {
    uint8_t header;
    uint8_t request;
    sc_haptic_t haptic;
    uint16_t pulse_high;
    uint16_t pulse_low;
    uint16_t repeat_count;
    uint8_t padding[55];
};

usb_error_t tone_callback(usb_endpoint_t pEndpoint,
                          usb_transfer_status_t status,
                          size_t size, void *pVoid) {
    dbg_sprintf(dbgout, "callback called\n");
    free(pVoid);
    return USB_SUCCESS;
}

const struct DataPacket sc_default_data = {0x8f, 0x08};

void sc_PlayTone(usb_device_t dev, uint8_t interface,
                 sc_haptic_t haptic, uint16_t pulse_duration,
                 uint16_t repeat_count) {
    struct DataPacket *data = malloc(sizeof(sc_default_data));
    usb_control_setup_t setup = {0x21, 9, 0x300, 0, 64};

    if(!dev) return;

    if(data) {
        dbg_sprintf(dbgout, "tone %u %u %u\n", haptic, pulse_duration, repeat_count);
        setup.wIndex = interface;
        memcpy(data, &sc_default_data, sizeof(sc_default_data));
        data->haptic = haptic;
        data->pulse_high = data->pulse_low = pulse_duration;
        data->repeat_count = repeat_count;

        usb_ScheduleDefaultControlTransfer(dev, &setup, data, tone_callback, data);
    } else {
        dbg_sprintf(dbgout, "failed to alloc for PlayTone\n");
    }
}

void sc_StopTone(usb_device_t dev, uint8_t interface,
                 sc_haptic_t haptic) {
    struct DataPacket *data = malloc(sizeof(sc_default_data));
    usb_control_setup_t setup = {0x21, 9, 0x300, 0, 64};

    if(data) {
        setup.wIndex = interface;
        memcpy(data, &sc_default_data, sizeof(sc_default_data));
        data->haptic = haptic;

        usb_ScheduleDefaultControlTransfer(dev, &setup, data, tone_callback, data);
    } else {
        dbg_sprintf(dbgout, "failed to alloc for StopTone\n");
    }
}


#endif
