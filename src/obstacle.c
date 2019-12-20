#include <tice.h>
#include <debug.h>
#include <keypadc.h>
#include "obstacle.h"
#include "config.h"
#include "util.h"
#include "physics.h"

#if USE_USB
#include "usb.h"
#include "hid/usb_hid_keys.h"
#endif

static uint24_t get_gap(obstacle_t *obstacle);

void add_obstacle(obstacle_t *new) {
    obstacle_type_index_t max_possible_type;
    const obstacle_type_data_t *type_data;

    if(game.dino.velocity_x.combined < FLOAT_TO_FIXED_POINT(PTERODACTYL_MIN_SPEED)) {
        max_possible_type = CACTUS_LARGE;
    } else {
        max_possible_type = PTERODACTYL;
    }

    new->type = randInt(0, max_possible_type);
    type_data = &obstacle_types[new->type];

    if(game.dino.velocity_x.combined > type_data->multiple_speed.combined) {
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
    new->x = new->last->x + new->last->gap;

    new->gap = get_gap(new);
}

void update_obstacles(void) {
    uint8_t i;
    for(i = 0; i < NUM_OBSTACLES; i++) {
        update_obstacle(&game.obstacles[i]);

        if(check_collision(&game.obstacles[i])) {
#if USE_CHEATS
            if(!kb_IsDown(kb_KeyGraph)
#if USE_USB
               && !any_hid_held(KEY_F5)
#endif
            )
#endif
            game.dino.alive = false;
        }
    }
}

void update_obstacle(obstacle_t *obstacle) {
    if(obstacle->x < game.distance - DINO_OFFSET_X + LCD_WIDTH) {
        obstacle->x_offset.combined += obstacle->velocity.combined;
    }

    if(obstacle->x_offset.parts.iPart) {
        obstacle->x += obstacle->x_offset.parts.iPart;
        obstacle->x_offset.parts.iPart = 0;
    }

    if((int24_t)(obstacle->x + obstacle->width) < (int24_t)(game.distance - DINO_OFFSET_X)) {
        add_obstacle(obstacle);
    }
}

void init_obstacles(void) {
    uint8_t i;
    const ifix_t dino_speed = {INT_TO_FIXED_POINT(INITIAL_SPEED)};
    /* Initialize the first obstacle with the last obstacle */
    game.obstacles[NUM_OBSTACLES - 1].gap = 1000;
    game.obstacles[0].last = &game.obstacles[NUM_OBSTACLES - 1];
    add_obstacle(&game.obstacles[0]);

    for(i = 1; i < NUM_OBSTACLES; i++) {
        game.obstacles[i].last = &game.obstacles[i - 1];
        add_obstacle(&game.obstacles[i]);
    }
}

static uint24_t get_gap(obstacle_t *obstacle) {
    uint24_t min_gap = obstacle->width * game.dino.velocity_x.parts.iPart
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
            72,
            3
        },
        {
            25,
            50,
            TOP_Y + 90,
            INT_TO_FIXED_POINT(7),
            72,
            3
        },
        {
            46,
            27,
            0,
            -1,
            90,
            5
        }
};
