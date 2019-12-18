#ifndef DINO_GAMESTATE_H
#define DINO_GAMESTATE_H

#include "dino.h"
#include "clouds.h"
#include "obstacle.h"

typedef struct Game {
    uint24_t frame;
    uint24_t distance;
    uint24_t score;
    uint24_t distance_to_score;
    bool distance_overrun;
    dino_t dino;
    cloud_t clouds[NUM_CLOUDS];
    obstacle_t obstacles[NUM_OBSTACLES];
    uint24_t high_score;
} game_t;

#endif //DINO_GAMESTATE_H
