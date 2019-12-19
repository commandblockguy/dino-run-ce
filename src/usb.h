#ifndef DINO_USB_H
#define DINO_USB_H

#include <usbdrvce.h>
#include "gamestate.h"

extern usb_device_t controller_dev;
extern uint8_t steam_interface;
extern hid_state_t hids[MAX_HID_DEVICES];

void start_usb(game_t *game);

bool any_hid_held(game_t *game, uint8_t key_code);
bool any_hid_mouse_held(game_t *game, uint8_t button);

#endif //DINO_USB_H
