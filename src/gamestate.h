#ifndef DINO_GAMESTATE_H
#define DINO_GAMESTATE_H

#include <usbdrvce.h>
#include "dino.h"
#include "clouds.h"
#include "obstacle.h"
#include "config.h"
#include "hid/hid.h"

enum {
    DAY,
    DUSK,
    NIGHT,
    DAWN,
    NUM_DAY_STAGES
};
typedef uint8_t day_stage_t;

typedef struct Game {
    uint24_t frame;
    uint24_t distance;
    uint24_t score;
    uint24_t distance_to_score;
    bool distance_overrun;
    dino_t dino;
    cloud_t clouds[NUM_CLOUDS];
    uint24_t distance_to_cloud_movement;
    obstacle_t obstacles[NUM_OBSTACLES];
    uint24_t high_score;
    day_stage_t day_stage;
    uint24_t distance_to_time_change;
} game_t;

#endif //DINO_GAMESTATE_H
