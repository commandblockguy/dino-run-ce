#ifndef DINO_SOUND_H
#define DINO_SOUND_H

#include <stdint.h>
#include "config.h"

#define MAX_TONES 2

enum Sounds {
    SOUND_JUMP,
    SOUND_DEATH,
    SOUND_ACHIEVEMENT,
};

typedef struct {
    uint24_t time;
    uint16_t pulse_duration;
    uint16_t repeat_count;
} tone_t;

typedef struct {
    uint8_t num_tones;
    tone_t tones[MAX_TONES];
} sound_t;

typedef struct {
    sound_t *sound;
    uint8_t next_tone;
    uint24_t next_tone_time;
} sound_player_t;

#ifndef USE_SOUND
#define USE_SOUND USE_USB
#endif

void play_sound(sound_player_t *player, const sound_t *sound);

void update_sound_player(sound_player_t *player);

void play_tone(const tone_t *tone);

extern const sound_t sounds[3];

#endif //DINO_SOUND_H
