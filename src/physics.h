#ifndef DINO_PHYSICS_H
#define DINO_PHYSICS_H

#include "gamestate.h"

typedef struct AABB {
    uint24_t x1;
    uint8_t y1;
    uint24_t x2;
    uint8_t y2;
} aabb_t;

bool check_collision(const obstacle_t *obstacle);

bool check_AABB_collision(const aabb_t *bb1, uint24_t x1, uint8_t y1,
                          const aabb_t *bb2, uint24_t x2, uint8_t y2);

extern const aabb_t obstacle_boxes[NUM_OBSTACLE_TYPES][5];
extern const aabb_t dino_fine_boxes[6];
extern const aabb_t rough_dino_box;
extern const aabb_t dino_box_ducking;

#endif //DINO_PHYSICS_H
