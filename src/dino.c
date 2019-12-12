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
    if(dino->y >= GROUND_LEVEL - MIN_JUMP_HEIGHT && !dino->dropping) return;
    if(dino->velocity_y.combined < INT_TO_FIXED_POINT(DROP_VELOCITY)) {
        dino->velocity_y.combined = INT_TO_FIXED_POINT(DROP_VELOCITY);
    }
}