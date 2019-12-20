#ifndef DINO_USB_H
#define DINO_USB_H

#include <usbdrvce.h>
#include "gamestate.h"

void start_usb();

bool any_hid_held(uint8_t key_code);
bool any_hid_mouse_held(uint8_t button);

#endif //DINO_USB_H
