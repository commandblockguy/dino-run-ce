#ifndef DINO_OBSTACLE_H
#define DINO_OBSTACLE_H

#include <stdint.h>
#include <stdbool.h>
#include "util.h"

/* The max number of cacti in one group */
#define MAX_CACTI_LENGTH 3

#define NUM_OBSTACLES 3

#define PTERODACTYL_MIN_SPEED 8.5
#define PTERODACTYL_FLY_SPEED 0.8

#define MAX_GAP_COEFFICIENT 3 / 2

enum ObstacleTypeIndex {
    CACTUS_SMALL,
    CACTUS_LARGE,
    PTERODACTYL,
    NUM_OBSTACLE_TYPES
};
typedef uint8_t obstacle_type_index_t;

typedef struct ObstacleTypeData {
    uint8_t width;
    uint8_t height;
    uint8_t y;
    ufix_t multiple_speed;
    uint8_t min_gap;
    // todo: fine collision boxes?
} obstacle_type_data_t;

extern const obstacle_type_data_t obstacle_types[NUM_OBSTACLE_TYPES];

typedef struct Obstacle {
    uint24_t x;
    uint8_t y;
    uint8_t height;
    uint8_t width;
    obstacle_type_index_t type;
    uint8_t size;
    ifix_t velocity;
    ifix_t x_offset;
    uint24_t gap;
} obstacle_t;

/* Add a new obstacle at location new. */
/* last should be the obstacle furthest from the dino (last to be added) */
void add_obstacle(obstacle_t *new, const obstacle_t *last, ifix_t dino_velocity);

/* A dummy obstacle used to add the first obstacle */
extern const obstacle_t dummy_obstacle;

#endif //DINO_OBSTACLE_H