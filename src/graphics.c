#include <stddef.h>
#include <string.h>
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
#include "gfx/night.h"

void init_graphics(void) {
    gfx_Begin();
    gfx_SetPalette(gfx_pal, sizeof_gfx_pal, 0);
    gfx_palette[WHITE] = 0xFFFF;
    gfx_FillScreen(BG_COLOR);

    fontlib_SetFont(font, 0);
    fontlib_SetWindowFullScreen();

#if LETTERBOX
    gfx_SetColor(BLACK);
    gfx_FillRectangle_NoClip(0, 0, LCD_WIDTH, TOP_Y);
    gfx_FillRectangle_NoClip(0, BOTTOM_Y, LCD_WIDTH, LCD_WIDTH - BOTTOM_Y);
    gfx_SetDrawBuffer();
    gfx_FillRectangle_NoClip(0, 0, LCD_WIDTH, TOP_Y);
    gfx_FillRectangle_NoClip(0, BOTTOM_Y, LCD_WIDTH, LCD_WIDTH - BOTTOM_Y);
#endif

    gfx_SetDrawBuffer();
}

void draw(void) {
    uint8_t i;

    if(game.day_stage == DAWN) {
        invert_palette(true);
    } else if(game.day_stage == DUSK) {
        invert_palette(false);
    }

    gfx_SetColor(BG_COLOR);
    gfx_FillRectangle_NoClip(0, TOP_Y, LCD_WIDTH, BOTTOM_Y - TOP_Y );

    draw_horizon(game.distance);

    if(game.day_stage != DAY) {
        if((gfx_palette[STAR_COLOR] & 0x1F) > (gfx_palette[BG_COLOR] & 0x1F)) {
            for(i = 0; i < MAX_NUM_STARS; i++) {
                draw_star(&game.stars[i]);
            }
        }

        if((gfx_palette[CLOUD_COLOR] & 0x1F) > (gfx_palette[BG_COLOR] & 0x1F))
            draw_moon();
    }

    for(i = 0; i < NUM_CLOUDS; i++) {
        draw_cloud(&game.clouds[i]);
    }

    for(i = 0; i < NUM_OBSTACLES; i++) {
        draw_obstacle(&game.obstacles[i]);
    }

    draw_dino();

    draw_high_score(game.high_score);
    if(game.distance_overrun) {
        draw_distance_meter(0);
    } else {
        draw_distance_meter(game.score);
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

    segment++;
    if(segment >= NUM_HORIZONS) segment = 0;
    x += HORIZON_SEGMENT_WIDTH;
    y = HORIZON_SPRITE_BASE - horizons[segment]->height;
    gfx_RLETSprite(horizons[segment], x, y);

    if(x + HORIZON_SEGMENT_WIDTH < LCD_WIDTH) {
        segment++;
        if(segment >= NUM_HORIZONS) segment = 0;
        x += HORIZON_SEGMENT_WIDTH;
        y = HORIZON_SPRITE_BASE - horizons[segment]->height;

        gfx_RLETSprite(horizons[segment], x, y);
    }
}

void draw_dino() {
#if SHOW_BOXES
    uint8_t i;
#endif
    gfx_rletsprite_t *sprite;
    uint8_t base_y = game.dino.y.parts.iPart;
    uint8_t y;
    uint24_t x = DINO_OFFSET_X;

    if(!game.dino.alive) {
        x = DINO_OFFSET_X + 2;
        sprite = dino_dead;
        base_y -= 2;
    } else if(!game.dino.on_ground) {
        sprite = dino_midair;
    } else if((game.frame & (2 * DINO_STEP_SPEED)) > DINO_STEP_SPEED) {
        /* Dino is on left foot */
        sprite = game.dino.ducking ? dino_duck_left : dino_left;
    } else {
        /* Dino is on right foot */
        sprite = game.dino.ducking ? dino_duck_right : dino_right;
    }

    y = base_y + 2 - sprite->height;
    gfx_RLETSprite_NoClip(sprite, x, y);

#if SHOW_BOXES
    gfx_SetColor(DINO_BOX_COLOR);

    if(game.dino.ducking) {
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

void draw_obstacle(const obstacle_t *obstacle) {
#if SHOW_BOXES
    uint8_t i;
#endif
    int24_t x;
    uint8_t y;
    gfx_rletsprite_t *sprite;

    x = obstacle->x - game.distance + DINO_OFFSET_X;

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
            if((game.frame & (2 * PTERODACTYL_FLAP_SPEED)) > PTERODACTYL_FLAP_SPEED) {
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
    gfx_SetColor(OBSTACLE_BOX_COLOR);

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

void draw_cloud(const cloud_t *cloud) {
    gfx_RLETSprite(cloud_sprite, cloud->x, cloud->y);
}

void draw_moon(void) {
    if(game.moon.phase == NUM_MOONS - 1) return;
    gfx_RLETSprite(moons[game.moon.phase], game.moon.x, MOON_Y);
}

void draw_star(const star_t *star) {
    gfx_RLETSprite(stars[star->type], star->x, star->y);
}

void draw_distance_meter(uint24_t score) {
    fontlib_SetCursorPosition(DISTANCE_METER_X, SCORE_TEXT_Y);
    fontlib_SetColors(FG_COLOR, BG_COLOR);
    fontlib_DrawUInt(score, SCORE_DIGITS);
}

void draw_high_score(uint24_t score) {
    fontlib_SetCursorPosition(HIGH_SCORE_X, SCORE_TEXT_Y);
    fontlib_SetColors(HS_COLOR, BG_COLOR);
    fontlib_DrawString("HI:");
    fontlib_DrawUInt(score, SCORE_DIGITS);
}

void fps_counter(void) {
    gfx_SetColor(BLACK);
    gfx_FillRectangle_NoClip(0, 0, 25, 20);

    gfx_SetTextXY(0, 0);
    gfx_SetTextFGColor(WHITE);
    gfx_PrintInt(timer_1_Counter, 3);
    gfx_SetTextXY(0, 12);
    gfx_PrintInt(ONE_SECOND / (uint24_t) timer_1_Counter, 3);
}

void draw_game_over(void) {
    const char game_over_text[] = "GAMEOVER";
    const uint24_t base_x = (LCD_WIDTH - GAME_OVER_TOTAL_WIDTH) / 2;
    uint8_t i;
    gfx_SetDrawScreen();

    fontlib_SetColors(FG_COLOR, BG_COLOR);

    for(i = 0; i < strlen(game_over_text); i++) {
        uint24_t x = base_x + (GAME_OVER_TEXT_WIDTH + GAME_OVER_TEXT_SPACING) * i;
        if(i >= 4) x += GAME_OVER_SPACE_WIDTH;
        fontlib_SetCursorPosition(x, GAME_OVER_TEXT_Y);
        fontlib_DrawGlyph(game_over_text[i]);
    }

    gfx_RLETSprite(restart, (LCD_WIDTH - restart_width) / 2, RESTART_BUTTON_Y);

    gfx_SetDrawBuffer();
}

void set_dynamic_palette(bool day) {
    uint16_t *palette;
    if(day) {
        palette = &gfx_pal[DYNAMIC_PALETTE_START];
    } else {
        palette = &night_pal[DYNAMIC_PALETTE_START];
    }
    gfx_SetPalette(palette, DYNAMIC_PALETTE_SIZE * 2, DYNAMIC_PALETTE_START);
}

void invert_palette(bool day) {
    uint8_t i;
    uint16_t *target_palette;

    if(day) {
        target_palette = gfx_pal;
    } else {
        target_palette = night_pal;
    }

    for(i = DYNAMIC_PALETTE_START; i < DYNAMIC_PALETTE_SIZE + DYNAMIC_PALETTE_START; i++) {
        uint8_t current = gfx_palette[i] & 0x1F;
        uint8_t target = target_palette[i] & 0x1F;
        uint8_t new;

        if(target < current) new = current - 1;
        else if(target > current) new = current + 1;
        else new = current;

        gfx_palette[i] = (new == 0x1F) << 15 | (new & 0x1F) << 10 | (new & 0x1F) << 5 | (new & 0x1F);
    }
}
