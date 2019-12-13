#include <debug.h>
#include "physics.h"
#include "config.h"
#include "util.h"

void update_physics(game_t *game) {
    game->distance += game->dino.velocity_x.parts.iPart;
    if(game->dino.velocity_x.combined < INT_TO_FIXED_POINT(MAX_SPEED)) {
        game->dino.velocity_x.combined += FLOAT_TO_FIXED_POINT(ACCELERATION);
    }

    if(game->dino.jumping) dino_start_jump(&game->dino);
    if(!game->dino.jumping) dino_end_jump(&game->dino);

    if(game->dino.y.combined <= INT_TO_FIXED_POINT(GROUND_LEVEL - MAX_JUMP_HEIGHT)) {
        dino_end_jump(&game->dino);
    }

    if(game->dino.dropping) {
        dino_end_jump(&game->dino);

        /* Make sure that we start dropping immediately */
        if(game->dino.velocity_y.combined < INT_TO_FIXED_POINT(1))
            game->dino.velocity_y.combined = INT_TO_FIXED_POINT(1);

        game->dino.y.combined += game->dino.velocity_y.combined * SPEED_DROP_COEFFICIENT;
    } else {
        game->dino.y.combined += game->dino.velocity_y.combined;
    }

    if(!game->dino.on_ground) {
        game->dino.velocity_y.combined += FLOAT_TO_FIXED_POINT(GRAVITY);
    }

    if(game->dino.y.combined > INT_TO_FIXED_POINT(GROUND_LEVEL)) {
        game->dino.y.combined = INT_TO_FIXED_POINT(GROUND_LEVEL);
        game->dino.velocity_y.combined = 0;
        game->dino.on_ground = true;
    }
}

bool check_collision(dino_t *dino, obstacle_t *obstacle) {

}
