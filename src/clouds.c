#include <tice.h>
#include "clouds.h"
#include "config.h"

void add_cloud(cloud_t *new) {
    new->x = new->last->x + new->last->gap;
    new->y = randInt(MIN_CLOUD_Y, MAX_CLOUD_Y);

    new->gap = randInt(MIN_CLOUD_GAP, MAX_CLOUD_GAP);
}

void update_cloud(cloud_t *cloud, uint24_t distance) {
    if(((int24_t)(cloud->x - distance + DINO_OFFSET_X) / 5 + CLOUD_WIDTH) < 0) {
        add_cloud(cloud);
    }
}

void init_clouds(cloud_t *clouds) {
    uint8_t i;

    /* Initialize the first cloud using the dummy cloud as the previous one */
    clouds[0].last = &dummy_cloud;
    add_cloud(&clouds[0]);
    /* Add the correct value of last to the first cloud */
    clouds[0].last = &clouds[NUM_CLOUDS - 1];

    for(i = 1; i < NUM_CLOUDS; i++) {
        clouds[i].last = &clouds[i - 1];
        add_cloud(&clouds[i]);
    }
}

const cloud_t dummy_cloud = {
        0,
        0,
        0,
        0,
};
