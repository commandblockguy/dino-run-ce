#include <debug.h>
#include "dino.h"
#include "util.h"
#include "config.h"
#include "gamestate.h"

void dino_start_jump(void) {
    if(!game.dino.on_ground) return;
    if(game.dino.ducking) return;

    game.dino.velocity_y.combined = INT_TO_FIXED_POINT(INITIAL_JUMP_VELOCITY) - game.dino.velocity_x.combined / 10;
    game.dino.on_ground = false;

#if USE_SOUND
    play_sound(&game.sound_player, &sounds[SOUND_JUMP]);
#endif
}

void dino_end_jump(void) {
    if(game.dino.y.combined >= INT_TO_FIXED_POINT(GROUND_LEVEL - MIN_JUMP_HEIGHT) && !game.dino.dropping) return;
    if(game.dino.velocity_y.combined < INT_TO_FIXED_POINT(DROP_VELOCITY)) {
        game.dino.velocity_y.combined = INT_TO_FIXED_POINT(DROP_VELOCITY);
    }
}

void update_dino(void) {
    if(game.dino.velocity_x.combined < INT_TO_FIXED_POINT(MAX_SPEED)) {
        game.dino.velocity_x.combined += FLOAT_TO_FIXED_POINT(ACCELERATION);
    }

    if(game.dino.jumping) dino_start_jump();
    if(!game.dino.jumping) dino_end_jump();

    if(game.dino.y.combined <= INT_TO_FIXED_POINT(GROUND_LEVEL - MAX_JUMP_HEIGHT)) {
        dino_end_jump();
    }

    if(game.dino.dropping) {
        dino_end_jump();

        /* Make sure that we start dropping immediately */
        if(game.dino.velocity_y.combined < INT_TO_FIXED_POINT(1))
            game.dino.velocity_y.combined = INT_TO_FIXED_POINT(1);

        game.dino.y.combined += game.dino.velocity_y.combined * SPEED_DROP_COEFFICIENT;
    } else {
        game.dino.y.combined += game.dino.velocity_y.combined;
    }

    if(!game.dino.on_ground) {
        game.dino.velocity_y.combined += FLOAT_TO_FIXED_POINT(GRAVITY);
    }

    if(game.dino.y.combined > INT_TO_FIXED_POINT(GROUND_LEVEL)) {
        game.dino.y.combined = INT_TO_FIXED_POINT(GROUND_LEVEL);
        game.dino.velocity_y.combined = 0;
        game.dino.on_ground = true;
    }
}