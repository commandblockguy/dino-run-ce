#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphx.h>
#include <keypadc.h>
#include <debug.h>
#include <usbdrvce.h>

#include "graphics.h"
#include "gamestate.h"
#include "util.h"
#include "physics.h"
#include "usb.h"
#include "lib/steam_controller.h"
#include "hid/usb_hid_keys.h"
#include "gfx/gfx.h"
#include "score.h"

void reset_timer(void) {
    timer_Control = TIMER1_DISABLE;
    timer_1_Counter = 0;
    timer_1_MatchValue_1 = FRAME_TIME;
    timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_NOINT | TIMER1_UP;
}

bool play(game_t *game) {
    while(true) {
        uint8_t i;
#if USE_USB
        usb_HandleEvents();
#endif

        kb_Scan();

        if(kb_IsDown(kb_KeyClear)) return false;

        game->dino.ducking = kb_IsDown(kb_KeyDown) && game->dino.on_ground;
        game->dino.dropping = kb_IsDown(kb_KeyDown) && !game->dino.on_ground;
        game->dino.jumping = kb_IsDown(kb_KeyUp)
#if USE_USB
        || any_hid_mouse_held(game, HID_MOUSE_LEFT)
#endif
        ;

        if(kb_IsDown(kb_Key1)) set_dynamic_palette(true);
        if(kb_IsDown(kb_Key2)) set_dynamic_palette(false);
        if(kb_IsDown(kb_Key3)) invert_palette(true);
        if(kb_IsDown(kb_Key4)) invert_palette(false);

        game->distance += game->dino.velocity_x.parts.iPart;
        if(game->distance > game->distance_to_score) {
            game->score++;
            game->distance_to_score += SCORE_DIVISOR;
            if(game->score > DISTANCE_METER_MAX) {
                game->distance_overrun = true;
            }
        }

        update_dino(&game->dino);

        for(i = 0; i < NUM_OBSTACLES; i++) {
            update_obstacle(&game->obstacles[i], game->distance, game->dino.velocity_x);
            if(check_collision(&game->dino, game->distance, &game->obstacles[i])) {
                if(!kb_IsDown(kb_KeyGraph))
                game->dino.alive = false;
            }
        }
        for(i = 0; i < NUM_CLOUDS; i++) {
            update_cloud(&game->clouds[i], game->distance);
        }

        if(game->distance > game->distance_to_time_change) {
            game->day_stage++;
            if(game->day_stage >= NUM_DAY_STAGES)
                game->day_stage = DAY;

            switch(game->day_stage) {
                case DAY:
                    game->distance_to_time_change += DAY_LENGTH;
                    break;
                case DAWN:
                case DUSK:
                    game->distance_to_time_change += INVERSION_DISTANCE;
                    break;
                case NIGHT:
                    game->distance_to_time_change += NIGHT_LENGTH;
                    break;
            }
        }

        if(game->day_stage == DAWN) {
            invert_palette(true);
        } else if(game->day_stage == DUSK) {
            invert_palette(false);
        }

        draw(game);

        while(!(timer_IntStatus & TIMER1_MATCH1)) {
            kb_Scan();
            if(kb_IsDown(kb_KeyClear)) break;
        }
        reset_timer();

        game->frame++;

        if(!game->dino.alive) return true;
    }
}

bool game_over() {
    draw_game_over();

    while(true) {
        kb_Scan();

        if(kb_IsDown(kb_KeyClear)) return true;
        if(kb_IsDown(kb_KeyEnter)) return false;
        if(kb_IsDown(kb_Key2nd)) return false;
    }
}

void main(void) {
    game_t game;

#if USE_USB
    start_usb(&game);
#endif

    ti_CloseAll();

    init_graphics();

    while(true) {
        bool quit;

        memset(&game, 0, sizeof(game));
        reset_timer();
        game.dino.alive = true;
        game.dino.on_ground = true;
        game.dino.y.combined = INT_TO_FIXED_POINT(GROUND_LEVEL);
        game.dino.velocity_x.combined = INT_TO_FIXED_POINT(INITIAL_SPEED);
        game.distance_to_score = SCORE_DIVISOR;
        game.high_score = get_score();
        game.distance_to_time_change = NIGHT_CYCLE_INTERVAL;

        set_dynamic_palette(true);

        init_obstacles(game.obstacles);
        init_clouds(game.clouds);

        quit = !play(&game);
        set_score(game.score);
        if(quit) {
            /* Player pressed clear */
            break;
        } else {
            /* Player died */
            if(game_over()) break;
        }
    }

    exit:
    gfx_End();

#if USE_USB
    usb_Cleanup();
#endif
}
