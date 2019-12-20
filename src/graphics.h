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
#define SCORE_TEXT_Y (TOP_Y + 10)

#define GAME_OVER_TEXT_Y (TOP_Y + 42)
#define GAME_OVER_TEXT_WIDTH 11
#define GAME_OVER_TEXT_SPACING 13
#define GAME_OVER_SPACE_WIDTH 10
#define GAME_OVER_TOTAL_WIDTH 191
#define RESTART_BUTTON_Y (TOP_Y + 75)

#define MOON_Y (TOP_Y + 30)

#define DYNAMIC_PALETTE_SIZE 4
#define DYNAMIC_PALETTE_START 1

#define WHITE 9

void init_graphics(void);

void draw(void);

void draw_horizon(uint24_t distance);

void draw_dino(void);

void draw_obstacle(const obstacle_t *obstacle);

void draw_cloud(const cloud_t *cloud);

void draw_moon(void);

void draw_star(const star_t *star);

void draw_distance_meter(uint24_t score);
void draw_high_score(uint24_t score);

void fps_counter(void);

void draw_game_over(void);

void set_dynamic_palette(bool day);

void invert_palette(bool day);

#endif //DINO_GRAPHICS_H
