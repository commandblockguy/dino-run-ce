#ifndef DINO_STEAM_CONTROLLER_H
#define DINO_STEAM_CONTROLLER_H

#include <stdint.h>
#include <usbdrvce.h>
#include <stdbool.h>

enum {
    SC_HAPTIC_RIGHT,
    SC_HAPTIC_LEFT
};
typedef bool sc_haptic_t;

/**
 * Gets the interface number to use for controlling haptic feedback
 * @param dev USB device to check
 * @return Interface number, or -1 if not a steam controller
 */
int8_t sc_GetInterface(usb_device_t dev);

/**
 * Plays a tone on the Steam controller
 * @param dev USB device
 * @param interface Interface from \c sc_GetInterface
 * @param haptic Which haptic feedback motor to use
 * @param pulse_duration Duration of each pulse. Use \c sc_FrequencyToDuration
 * @param repeat_count Number of times to repeat. Use \c sc_TimeToRepeatCount
 */
void sc_PlayTone(usb_device_t dev, uint8_t interface,
                        sc_haptic_t haptic, uint16_t pulse_duration,
                        uint16_t repeat_count);

/**
 * Stops playing a tone on the Steam controller
 * @param dev USB device
 * @param interface Interface from \c sc_GetInterface
 * @param haptic Which haptic feedback motor to stop
 */
void sc_StopTone(usb_device_t dev, uint8_t interface,
                        sc_haptic_t haptic);

#define STEAM_CONTROLLER_MAGIC_PERIOD_RATIO 495483

/**
 * Converts a frequency to a pulse duration
 * @param freq Frequency in hertz
 * @return Pulse duration for use with sc_PlayTone
 */
#define sc_FrequencyToDuration(freq) ((uint16_t)(STEAM_CONTROLLER_MAGIC_PERIOD_RATIO / freq))

/**
 * Converts a time and frequency to a repeat count
 * @param ms Time in milliseconds
 * @param freq Frequency in hertz
 * @return Repeat count for use with sc_PlayTone
 */
#define sc_TimeToRepeatCount(ms, freq) ((uint16_t)((ms * freq) / 1000))


#endif //DINO_STEAM_CONTROLLER_H
