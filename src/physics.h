#ifndef DINO_PHYSICS_H
#define DINO_PHYSICS_H

#include "gamestate.h"

void update_physics(game_t *game);

bool check_collision(const dino_t *dino, const obstacle_t *obstacle);

#endif //DINO_PHYSICS_H
