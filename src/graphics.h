#ifndef DINO_GRAPHICS_H
#define DINO_GRAPHICS_H

#include "config.h"
#include "dino.h"
#include "gamestate.h"

#define HORIZON_SPRITE_BASE (GROUND_LEVEL + 1)

/* Number of frames between dino feet switches */
#define DINO_STEP_SPEED 5

#define PTERODACTYL_FLAP_SPEED 10

#define DISTANCE_METER_X 254
#define HIGH_SCORE_X (DISTANCE_METER_X - 100)
#define SCORE_TEXT_Y (TOP_Y + 5)

void init_graphics(void);

void draw(const game_t *game);

void draw_horizon(uint24_t distance);

void draw_dino(const dino_t *dino, uint24_t frame);

void draw_obstacle(const obstacle_t *obstacle, uint24_t distance, uint24_t frame);

void draw_cloud(const cloud_t *cloud, uint24_t distance);

void draw_distance_meter(uint24_t score);
void draw_high_score(uint24_t score);

void fps_counter(void);

#endif //DINO_GRAPHICS_H
