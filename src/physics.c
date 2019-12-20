#include <debug.h>
#include <graphx.h>
#include "obstacle.h"
#include "physics.h"
#include "clouds.h"
#include "config.h"
#include "util.h"
#include "dino.h"

/* Apologies to whoever has to read this. I tried to keep it contained to one function. */
bool check_collision(const obstacle_t *obstacle) {
    aabb_t obstacle_box = {1, 1, 0, 0};
    uint8_t base_y = game.dino.y.parts.iPart - (game.dino.ducking ? DINO_HEIGHT_DUCK : DINO_HEIGHT);

    obstacle_box.x2 = obstacle->width - 1;
    obstacle_box.y2 = obstacle->height - 1;

    if(check_AABB_collision(game.dino.ducking ? &dino_box_ducking : &rough_dino_box,
                            game.distance, base_y, &obstacle_box, obstacle->x, obstacle->y)) {
        uint8_t max_obstacle = obstacle_types[obstacle->type].num_boxes;
        uint8_t obstacle_num;
        for(obstacle_num = 0; obstacle_num < max_obstacle; obstacle_num++) {
            uint8_t j;
            for(j = 0; j < obstacle->size; j++) {
                if(game.dino.ducking) {
                    if(check_AABB_collision(&dino_box_ducking, game.distance, base_y,
                                            &obstacle_boxes[obstacle->type][obstacle_num],
                                            obstacle->x + j * obstacle_types[obstacle->type].width,
                                            obstacle->y)) {
                        return true;
                    }
                } else {
                    uint8_t dino_box;
                    for(dino_box = 0; dino_box < 6; dino_box++) {
                        if(check_AABB_collision(&dino_fine_boxes[dino_box],
                                game.distance, base_y,
                                &obstacle_boxes[obstacle->type][obstacle_num],
                                obstacle->x + j * obstacle_types[obstacle->type].width,
                                obstacle->y)) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool check_AABB_collision(const aabb_t *bb1, uint24_t x1, uint8_t y1,
                          const aabb_t *bb2, uint24_t x2, uint8_t y2) {
    return bb1->x1 + x1 < bb2->x2 + x2 &&
           bb1->x2 + x1 > bb2->x1 + x2 &&
           bb1->y1 + y1 < bb2->y2 + y2 &&
           bb1->y2 + y1 > bb2->y1 + y2;
}

const aabb_t obstacle_boxes[NUM_OBSTACLE_TYPES][5] = {
    {
        {0, 7, 5, 34},
        {4, 0, 10, 34},
        {10, 4, 17, 18}
    },
    {
        {0, 12, 7, 50},
        {8, 0, 15, 49},
        {13, 10, 23, 48}
    },
    {
        {15, 15, 31, 20},
        {18, 21, 42, 27},
        {2, 14, 6, 17},
        {6, 10, 10, 17},
        {10, 8, 16, 17}
    }
};

const aabb_t dino_fine_boxes[6] = {
    {22, 0, 39, 16},
    {1, 18, 31, 27},
    {10, 35, 24, 43},
    {1, 24, 30, 29},
    {5, 30, 26, 34},
    {9, 34, 24, 38}
};

const aabb_t rough_dino_box = {1, 1, DINO_WIDTH - 1, DINO_HEIGHT - 1};
const aabb_t dino_box_ducking = {1, 1, DINO_WIDTH_DUCK - 1, DINO_HEIGHT_DUCK - 1};
