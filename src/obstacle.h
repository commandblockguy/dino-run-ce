#ifndef DINO_OBSTACLE_H
#define DINO_OBSTACLE_H

#include <stdint.h>
#include <stdbool.h>

/* The max number of cacti in one group */
#define MAX_CACTI_LENGTH 3

#define NUM_OBSTACLES 5

//enum CactusType {
//
//};
typedef uint8_t cactus_type_t;

typedef struct Obstacle {
    uint24_t x;
    uint8_t height;
    uint8_t width;
    uint8_t base_y;
    bool isPterodactyl;
    cactus_type_t cacti[MAX_CACTI_LENGTH];
} obstacle_t;

#endif //DINO_OBSTACLE_H
