#ifndef DINO_GRAPHICS_H
#define DINO_GRAPHICS_H

#include "config.h"
#include "dino.h"
#include "gamestate.h"

#define DINO_OFFSET_X 13

#define HORIZON_SPRITE_BASE (GROUND_LEVEL + 1)

/* Number of frames between dino feet switches */
#define DINO_STEP_SPEED 5

#define PTERODACTYL_FLAP_SPEED 10

void init_graphics(void);

void draw(game_t *game);

void draw_horizon(uint24_t distance);

void draw_dino(dino_t *dino, uint24_t frame);

void draw_obstacle(obstacle_t *obstacle, uint24_t distance, uint24_t frame);

void fps_counter(void);

#endif //DINO_GRAPHICS_H
