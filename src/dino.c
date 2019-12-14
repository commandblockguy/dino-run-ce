#include <debug.h>
#include "dino.h"
#include "util.h"
#include "config.h"

void dino_start_jump(dino_t *dino) {
    if(!dino->on_ground) return;
    if(dino->ducking) return;

    dino->velocity_y.combined = INT_TO_FIXED_POINT(INITIAL_JUMP_VELOCITY) - dino->velocity_x.combined / 10;
    dino->on_ground = false;
}

void dino_end_jump(dino_t *dino) {
    if(dino->y.combined >= INT_TO_FIXED_POINT(GROUND_LEVEL - MIN_JUMP_HEIGHT) && !dino->dropping) return;
    if(dino->velocity_y.combined < INT_TO_FIXED_POINT(DROP_VELOCITY)) {
        dino->velocity_y.combined = INT_TO_FIXED_POINT(DROP_VELOCITY);
    }
}

void update_dino(dino_t *dino) {
    if(dino->velocity_x.combined < INT_TO_FIXED_POINT(MAX_SPEED)) {
        dino->velocity_x.combined += FLOAT_TO_FIXED_POINT(ACCELERATION);
    }

    if(dino->jumping) dino_start_jump(dino);
    if(!dino->jumping) dino_end_jump(dino);

    if(dino->y.combined <= INT_TO_FIXED_POINT(GROUND_LEVEL - MAX_JUMP_HEIGHT)) {
        dino_end_jump(dino);
    }

    if(dino->dropping) {
        dino_end_jump(dino);

        /* Make sure that we start dropping immediately */
        if(dino->velocity_y.combined < INT_TO_FIXED_POINT(1))
            dino->velocity_y.combined = INT_TO_FIXED_POINT(1);

        dino->y.combined += dino->velocity_y.combined * SPEED_DROP_COEFFICIENT;
    } else {
        dino->y.combined += dino->velocity_y.combined;
    }

    if(!dino->on_ground) {
        dino->velocity_y.combined += FLOAT_TO_FIXED_POINT(GRAVITY);
    }

    if(dino->y.combined > INT_TO_FIXED_POINT(GROUND_LEVEL)) {
        dino->y.combined = INT_TO_FIXED_POINT(GROUND_LEVEL);
        dino->velocity_y.combined = 0;
        dino->on_ground = true;
    }
}