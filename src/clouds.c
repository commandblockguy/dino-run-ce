#include <tice.h>
#include "clouds.h"
#include "config.h"

void add_cloud(cloud_t *new) {
    new->x = new->last->x + new->last->gap;
    new->y = randInt(MIN_CLOUD_Y, MAX_CLOUD_Y);

    new->gap = randInt(MIN_CLOUD_GAP, MAX_CLOUD_GAP);
}

void update_clouds(cloud_t *clouds, uint24_t distance, uint24_t *next_distance) {
    uint8_t i;
    uint8_t offset = 0;
    while(distance >= *next_distance) {
        offset++;
        *next_distance += CLOUD_PARALLAX_RATIO;
    }
    for(i = 0; i < NUM_CLOUDS; i++) {
        update_cloud(&clouds[i], offset);
    }
}

void update_cloud(cloud_t *cloud, uint8_t offset) {
    cloud->x -= offset;
    if(cloud->x + CLOUD_WIDTH < 0) {
        add_cloud(cloud);
    }
}

void init_clouds(cloud_t *clouds) {
    uint8_t i;

    /* Add the correct value of last to the first cloud */
    clouds[0].last = &clouds[NUM_CLOUDS - 1];
    add_cloud(&clouds[0]);

    for(i = 1; i < NUM_CLOUDS; i++) {
        clouds[i].last = &clouds[i - 1];
        add_cloud(&clouds[i]);
    }
}
