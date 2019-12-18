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

#include "graphics.h"
#include "gamestate.h"
#include "util.h"
#include "physics.h"
#include "gfx/gfx.h"

void reset_timer(void) {
    timer_Control = TIMER1_DISABLE;
    timer_1_Counter = 0;
    timer_1_MatchValue_1 = FRAME_TIME;
    timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_NOINT | TIMER1_UP;
}

bool play(game_t *game) {
    while(true) {
        uint8_t i;
        kb_Scan();

        if(kb_IsDown(kb_KeyClear)) return false;

        game->dino.ducking = kb_IsDown(kb_KeyDown) && game->dino.on_ground;
        game->dino.dropping = kb_IsDown(kb_KeyDown) && !game->dino.on_ground;
        game->dino.jumping = kb_IsDown(kb_KeyUp);

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
                game->dino.alive = false;
            }
        }
        for(i = 0; i < NUM_CLOUDS; i++) {
            update_cloud(&game->clouds[i], game->distance);
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

void main(void) {
    uint8_t i;
    game_t game = {0};
    init_graphics();


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

        init_obstacles(game.obstacles);
        init_clouds(game.clouds);

        quit = !play(&game);
        //todo: high score
        if(quit) {
            /* Player pressed clear */
            break;
        } else {
            /* Player died */
            gfx_SetDrawScreen();
            gfx_RLETSprite(restart, (LCD_WIDTH - restart_width) / 2, (LCD_HEIGHT - restart_height) / 2);
            gfx_SetDrawBuffer();

            while(true) {
                kb_Scan();

                if(kb_IsDown(kb_KeyClear)) break;
                if(kb_IsDown(kb_KeyEnter)) break;
                if(kb_IsDown(kb_Key2nd)) break;
            }
            if(kb_IsDown(kb_KeyClear)) break;
        }
    }

    gfx_End();
}
