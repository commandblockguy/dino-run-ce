#ifndef DINO_OBSTACLE_H
#define DINO_OBSTACLE_H

#include <stdint.h>
#include <stdbool.h>
#include "util.h"

/* The max number of cacti in one group */
#define MAX_CACTI_LENGTH 3

#define NUM_OBSTACLES 5

enum ObstacleType {
    NONE,
    CACTUS_SMALL,
    CACTUS_LARGE,
    PTERODACTYL
};
typedef uint8_t obstacle_type_t;

typedef struct Obstacle {
    uint24_t x;
    uint8_t y;
    uint8_t height;
    uint8_t width;
    obstacle_type_t type;
    uint8_t size;
    ifix_t velocity;
} obstacle_t;

#endif //DINO_OBSTACLE_H
