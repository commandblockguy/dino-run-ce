#include <tice.h>
#include <debug.h>
#include "obstacle.h"
#include "config.h"
#include "util.h"

static uint24_t get_gap(obstacle_t *obstacle, ifix_t dino_velocity);

void add_obstacle(obstacle_t *new, const obstacle_t *last, ifix_t dino_velocity) {
    obstacle_type_index_t max_possible_type;
    const obstacle_type_data_t *type_data;

    if(dino_velocity.combined < FLOAT_TO_FIXED_POINT(PTERODACTYL_MIN_SPEED)) {
        max_possible_type = CACTUS_LARGE;
    } else {
        max_possible_type = PTERODACTYL;
    }

    new->type = randInt(0, max_possible_type);
    type_data = &obstacle_types[new->type];

    if(dino_velocity.combined > type_data->multiple_speed.combined) {
        new->size = randInt(1, MAX_CACTI_LENGTH);
    } else {
        new->size = 1;
    }

    new->width = type_data->width * new->size;
    new->height = type_data->height;

    if(new->type == PTERODACTYL) {
        uint8_t heights[3] = {TOP_Y + 100, TOP_Y + 75, TOP_Y + 50};
        new->y = heights[randInt(0,2)];

        if(randInt(0,1)) {
            new->velocity.combined = FLOAT_TO_FIXED_POINT(PTERODACTYL_FLY_SPEED);
        } else {
            new->velocity.combined = FLOAT_TO_FIXED_POINT(-PTERODACTYL_FLY_SPEED);
        }
    } else {
        new->y = type_data->y;
        new->velocity.combined = 0;
    }

    new->x_offset.combined = 0;
    new->x = last->x + last->gap;

    new->gap = get_gap(new, dino_velocity);
}

static uint24_t get_gap(obstacle_t *obstacle, ifix_t dino_velocity) {
    uint24_t min_gap = obstacle->width * dino_velocity.parts.iPart
            + obstacle_types[obstacle->type].min_gap;
    uint24_t max_gap = min_gap * MAX_GAP_COEFFICIENT;

    return randInt(min_gap, max_gap) + obstacle->width;
}

const obstacle_type_data_t obstacle_types[NUM_OBSTACLE_TYPES] = {
        {
            17,
            35,
            TOP_Y + 105,
            INT_TO_FIXED_POINT(4),
            72
        },
        {
            25,
            50,
            TOP_Y + 90,
            INT_TO_FIXED_POINT(7),
            72
        },
        {
            46,
            40,
            0,
            -1,
            90
        }
};

const obstacle_t dummy_obstacle = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1000
};