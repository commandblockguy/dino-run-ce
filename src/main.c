#include <stdbool.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <string.h>
#include <graphx.h>
#include <keypadc.h>
#include <debug.h>

#include "graphics.h"
#include "gamestate.h"
#include "util.h"
#include "score.h"
#include "night.h"
#include "sound.h"

#if USE_USB
#include <usbdrvce.h>
#include "usb.h"
#include "lib/steam_controller.h"
#include "hid/usb_hid_keys.h"
#endif

game_t game;

void reset_timer(void) {
    timer_Control &= ~TIMER1_ENABLE;
    timer_1_Counter = 0;
    timer_1_MatchValue_1 = FRAME_TIME;
    timer_Control |= TIMER1_ENABLE;
}

bool play(void) {
    while(true) {
        uint8_t i;
        bool jump_pressed, duck_pressed;
#if USE_USB
        usb_HandleEvents();
#endif
#if USE_SOUND
        update_sound_player(&game.sound_player);
#endif

        kb_Scan();

        if(kb_IsDown(kb_KeyClear)) return false;

#if USE_USB
        duck_pressed = kb_IsDown(kb_KeyDown) ||
                       any_hid_held(KEY_DOWN) ||
                       any_hid_mouse_held(HID_MOUSE_RIGHT);
        jump_pressed = kb_IsDown(kb_KeyUp) ||
                       any_hid_held(KEY_UP) ||
                       any_hid_held(KEY_SPACE) ||
                       any_hid_mouse_held(HID_MOUSE_LEFT);
#else
        duck_pressed = kb_IsDown(kb_KeyDown);
        jump_pressed = kb_IsDown(kb_KeyUp);
#endif

        game.dino.ducking = duck_pressed && game.dino.on_ground;
        game.dino.dropping = duck_pressed && !game.dino.on_ground;
        game.dino.jumping = jump_pressed;

        game.distance += game.dino.velocity_x.parts.iPart;
        if(game.distance > game.distance_to_score) {
            game.score++;
            game.distance_to_score += SCORE_DIVISOR;
            if(game.score > DISTANCE_METER_MAX) {
                game.distance_overrun = true;
            }
        }
        if(game.score >= game.next_beep_score) {
            game.next_beep_score += 100;
#if USE_SOUND
            play_sound(&game.sound_player, &sounds[SOUND_ACHIEVEMENT]);
#endif
        }

        update_dino();

        update_obstacles();

        update_clouds();

        update_day_stage();

        draw();

        while(!(timer_IntStatus & TIMER1_MATCH1)) {
            timer_IntAcknowledge = TIMER1_MATCH1;
            kb_Scan();
            if(kb_IsDown(kb_KeyClear)) break;
#if USE_SOUND
            update_sound_player(&game.sound_player);
#endif
#if USE_USB
            usb_HandleEvents();
            if(any_hid_held(KEY_ESC)) break;
#endif
        }
        reset_timer();

        game.frame++;

        if(!game.dino.alive) return true;
    }
}

bool game_over(void) {
    draw_game_over();
#if USE_SOUND
    play_sound(&game.sound_player, &sounds[SOUND_DEATH]);
#endif

    while(true) {
        kb_Scan();

        if(kb_IsDown(kb_KeyClear)) return true;
        if(kb_IsDown(kb_KeyEnter)) return false;
        if(kb_IsDown(kb_Key2nd)) return false;
#if USE_SOUND
      update_sound_player(&game.sound_player);
#endif
#if USE_USB
        usb_HandleEvents();
        if(any_hid_held(KEY_ESC)) return true;
        if(any_hid_held(KEY_ENTER)) return false;
        if(any_hid_mouse_held(HID_MOUSE_LEFT)) return false;
#endif
    }
}

void main(void) {
#if USE_USB
    start_usb(&game);
#endif

    dbg_sprintf(dbgout, "Dino Run CE starting\n");
    dbg_sprintf(dbgout, "Version %s\n", xstr(VERSION));
    dbg_sprintf(dbgout, "Git commit %s, changed: %s\n",
                xstr(COMMIT), xstr(DIFF_STATUS));
    dbg_sprintf(dbgout, "Built %s %s\n", __TIME__, __DATE__);

    ti_CloseAll();

    timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_NOINT | TIMER1_UP |
                    TIMER2_ENABLE | TIMER2_32K | TIMER2_NOINT | TIMER2_UP;

    init_graphics();

    while(true) {
        bool quit;

        memset(&game, 0, sizeof(game)
#if USE_USB
            - sizeof(game.usb)
#endif
        );
        reset_timer();
        game.dino.alive = true;
        game.dino.on_ground = true;
        game.dino.y.combined = INT_TO_FIXED_POINT(GROUND_LEVEL);
        game.dino.velocity_x.combined = INT_TO_FIXED_POINT(INITIAL_SPEED);
        game.distance_to_score = SCORE_DIVISOR;
        game.high_score = get_score();
        game.distance_to_time_change = NIGHT_CYCLE_INTERVAL;
        game.next_beep_score = 100;

        set_dynamic_palette(true);

        init_obstacles();
        init_clouds();
#if USE_SOUND
        play_sound(&game.sound_player, &sounds[SOUND_JUMP]);
#endif

        quit = !play();
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
