#include <debug.h>
#include "obstacle.h"
#include "physics.h"
#include "clouds.h"
#include "config.h"
#include "util.h"

void update_physics(game_t *game) {
    uint8_t i;
    game->distance += game->dino.velocity_x.parts.iPart;
    update_dino(&game->dino);

    for(i = 0; i < NUM_OBSTACLES; i++) {
        update_obstacle(&game->obstacles[i], game->distance, game->dino.velocity_x);
    }
    for(i = 0; i < NUM_CLOUDS; i++) {
        update_cloud(&game->clouds[i], game->distance);
    }
}

bool check_collision(const dino_t *dino, const obstacle_t *obstacle) {

}
