#include <tice.h>
#include "clouds.h"
#include "config.h"
#include "gamestate.h"

void add_cloud(cloud_t *new) {
    new->x = new->last->x + new->last->gap;
    new->y = randInt(MIN_CLOUD_Y, MAX_CLOUD_Y);

    new->gap = randInt(MIN_CLOUD_GAP, MAX_CLOUD_GAP);
}

void update_clouds(void) {
    uint8_t i;
    uint8_t offset = 0;
    while(game.distance >= game.distance_to_cloud_movement) {
        offset++;
        game.distance_to_cloud_movement += CLOUD_PARALLAX_RATIO;
    }
    for(i = 0; i < NUM_CLOUDS; i++) {
        update_cloud(&game.clouds[i], offset);
    }
}

void update_cloud(cloud_t *cloud, uint8_t offset) {
    cloud->x -= offset;
    if(cloud->x + CLOUD_WIDTH < 0) {
        add_cloud(cloud);
    }
}

void init_clouds(void) {
    uint8_t i;

    /* Add the correct value of last to the first cloud */
    game.clouds[0].last = &game.clouds[NUM_CLOUDS - 1];
    add_cloud(&game.clouds[0]);

    for(i = 1; i < NUM_CLOUDS; i++) {
        game.clouds[i].last = &game.clouds[i - 1];
        add_cloud(&game.clouds[i]);
    }
}
