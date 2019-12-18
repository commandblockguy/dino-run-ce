#include <stddef.h>
#include <graphx.h>
#include <fontlibc.h>
#include <debug.h>

#include "graphics.h"
#include "gfx/gfx.h"
#include "font/font.h"
#include "sprites.h"
#include "gamestate.h"
#include "util.h"
#include "clouds.h"
#include "physics.h"

void init_graphics(void) {
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetPalette(gfx_pal, sizeof_gfx_pal, 0);
    fontlib_SetFont(font, 0);
    fontlib_SetWindowFullScreen();
}

void draw(const game_t *game) {
    uint8_t i;
    gfx_FillScreen(0xFF);

    gfx_SetColor(0);
    gfx_FillRectangle(0, 0, LCD_WIDTH, TOP_Y);
    gfx_FillRectangle(0, BOTTOM_Y, LCD_WIDTH, LCD_WIDTH - BOTTOM_Y);

    draw_horizon(game->distance);

    for(i = 0; i < NUM_OBSTACLES; i++) {
        draw_obstacle(&game->obstacles[i], game->distance, game->frame);
    }

    for(i = 0; i < NUM_CLOUDS; i++) {
        draw_cloud(&game->clouds[i], game->distance);
    }

    draw_dino(&game->dino, game->frame);

    draw_high_score(game->high_score);
    if(game->distance_overrun) {
        draw_distance_meter(0);
    } else {
        draw_distance_meter(game->distance);
    }

#if SHOW_FPS
    fps_counter();
#endif
    gfx_SwapDraw();
}

void draw_horizon(uint24_t distance) {
    uint24_t total_offset = distance % (HORIZON_SEGMENT_WIDTH * NUM_HORIZONS);
    uint24_t segment_offset = total_offset % HORIZON_SEGMENT_WIDTH;

    uint8_t segment;
    int24_t x;
    uint8_t y;

    segment = total_offset / HORIZON_SEGMENT_WIDTH;
    x = -segment_offset;
    y = HORIZON_SPRITE_BASE - horizons[segment]->height;
    gfx_RLETSprite(horizons[segment], x, y);

    segment = (segment + 1) % NUM_HORIZONS;
    x += HORIZON_SEGMENT_WIDTH;
    y = HORIZON_SPRITE_BASE - horizons[segment]->height;
    gfx_RLETSprite(horizons[segment], x, y);

    if(x + HORIZON_SEGMENT_WIDTH < LCD_WIDTH) {
        segment = (segment + 1) % NUM_HORIZONS;
        x += HORIZON_SEGMENT_WIDTH;
        y = HORIZON_SPRITE_BASE - horizons[segment]->height;

        gfx_RLETSprite(horizons[segment], x, y);
    }
}

void draw_dino(const dino_t *dino, uint24_t frame) {
#if SHOW_BOXES
    uint8_t i;
#endif
    gfx_rletsprite_t *sprite;
    uint8_t y;

    if(!dino->on_ground) {
        sprite = dino_midair;
    } else {
        if((frame & (2 * DINO_STEP_SPEED)) > DINO_STEP_SPEED) {
            /* Dino is on left foot */
            sprite = dino->ducking ? dino_duck_left : dino_left;
        } else {
            /* Dino is on right foot */
            sprite = dino->ducking ? dino_duck_right : dino_right;
        }
    }

    y = dino->y.parts.iPart + 2 - sprite->height;
    gfx_RLETSprite(sprite, DINO_OFFSET_X, y);

#if SHOW_BOXES
    gfx_SetColor(31);

    if(dino->ducking) {
        gfx_Rectangle(DINO_OFFSET_X + dino_box_ducking.x1, y + dino_box_ducking.y1,
                      dino_box_ducking.x2 - dino_box_ducking.x1,
                      dino_box_ducking.y2 - dino_box_ducking.y1);
    } else {
        for(i = 0; i < 5; i++) {
            const aabb_t *box = &dino_fine_boxes[i];
            gfx_Rectangle(DINO_OFFSET_X + box->x1, y + box->y1, box->x2 - box->x1, box->y2 - box->y1);
        }
    }
#endif
}

void draw_obstacle(const obstacle_t *obstacle, uint24_t distance, uint24_t frame) {
#if SHOW_BOXES
    uint8_t i;
#endif
    int24_t x;
    uint8_t y;
    gfx_rletsprite_t *sprite;

    x = obstacle->x - distance + DINO_OFFSET_X;

    if(x > LCD_WIDTH) return;

    y = obstacle->y;

    switch(obstacle->type) {
        case CACTUS_SMALL:
            sprite = small_cacti[obstacle->size - 1];
            break;
        case CACTUS_LARGE:
            sprite = large_cacti[obstacle->size - 1];
            break;
        case PTERODACTYL:
            if((frame & (2 * PTERODACTYL_FLAP_SPEED)) > PTERODACTYL_FLAP_SPEED) {
                sprite = bird_0;
                y += PTERODACTYL_0_Y_OFFSET;
            }
            else {
                sprite = bird_1;
            }
            break;
        default:
            return;
    }

    gfx_RLETSprite(sprite, x, y);

#if SHOW_BOXES
    gfx_SetColor(31);

    for(i = 0; i < obstacle_types[obstacle->type].num_boxes; i++) {
        const aabb_t *box = &obstacle_boxes[obstacle->type][i];
        int j;
        for(j = 0; j < obstacle->size; j++) {
            gfx_Rectangle(x + j * obstacle_types[obstacle->type].width + box->x1,
                          obstacle->y + box->y1, box->x2 - box->x1,
                          box->y2 - box->y1);
        }
    }

#endif
}

void draw_cloud(const cloud_t *cloud, uint24_t distance) {
    gfx_RLETSprite(cloud_sprite, (int24_t)(cloud->x - distance) / 5, cloud->y);
}

void draw_distance_meter(uint24_t distance) {
    fontlib_SetCursorPosition(DISTANCE_METER_X, SCORE_TEXT_Y);
    fontlib_SetForegroundColor(1); //todo: fix palette
    fontlib_DrawUInt(distance / SCORE_DIVISOR, SCORE_DIGITS);
}

void draw_high_score(uint24_t score) {
    fontlib_SetCursorPosition(HIGH_SCORE_X, SCORE_TEXT_Y);
    fontlib_SetForegroundColor(7); //todo: fix palette
    fontlib_DrawString("HI:");
    fontlib_DrawUInt(score, SCORE_DIGITS);
}

void fps_counter(void) {
    gfx_SetTextXY(0, 0);
    gfx_SetTextFGColor(3); //todo: fix
    gfx_PrintInt(timer_1_Counter, 3);
    gfx_SetTextXY(0, 12);
    gfx_PrintInt(ONE_SECOND / (uint24_t) timer_1_Counter, 3);
}
