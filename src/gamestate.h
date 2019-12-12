#ifndef DINO_GAMESTATE_H
#define DINO_GAMESTATE_H

#include "dino.h"
#include "clouds.h"
#include "obstacle.h"

typedef struct Game {
    uint24_t frame;
    uint24_t distance;
    dino_t dino;
    cloud_t clouds[NUM_CLOUDS];
    obstacle_t obstacles[NUM_OBSTACLES];
} game_t;

#endif //DINO_GAMESTATE_H
