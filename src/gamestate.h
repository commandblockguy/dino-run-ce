#ifndef DINO_GAMESTATE_H
#define DINO_GAMESTATE_H

#include <usbdrvce.h>
#include "dino.h"
#include "clouds.h"
#include "obstacle.h"
#include "config.h"
#include "hid/hid.h"
#include "night.h"

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
    uint8_t day_stage;
    uint24_t distance_to_time_change;
    moon_t moon;
    star_t stars[MAX_NUM_STARS];
    uint24_t star_movement_frame;
} game_t;

#endif //DINO_GAMESTATE_H
