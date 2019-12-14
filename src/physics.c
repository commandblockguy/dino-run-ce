#include <debug.h>
#include "physics.h"
#include "config.h"
#include "util.h"

void update_physics(game_t *game) {
    uint8_t i;
    game->distance += game->dino.velocity_x.parts.iPart;
    update_dino(&game->dino);

    for(i = 0; i < NUM_OBSTACLES; i++) {
        obstacle_t *obstacle = &game->obstacles[i];

        obstacle->x_offset.combined += obstacle->velocity.combined;

        if(obstacle->x_offset.parts.iPart) {
            obstacle->x += obstacle->x_offset.parts.iPart;
            obstacle->x_offset.parts.iPart = 0;
        }

        if((int24_t)(obstacle->x + obstacle->width) < (int24_t)(game->distance - DINO_OFFSET_X)) {
            obstacle_t *last;

            if(i > 0) {
                last = &game->obstacles[i] - 1;
            } else {
                last = &game->obstacles[NUM_OBSTACLES - 1];
            }

            add_obstacle(obstacle, last, game->dino.velocity_x);
        }
    }
}

bool check_collision(const dino_t *dino, const obstacle_t *obstacle) {

}
