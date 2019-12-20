#ifndef DINO_CLOUDS_H
#define DINO_CLOUDS_H

#include <stdint.h>

/* For each CLOUD_PARALLAX_RATIO pixels that the ground moves, the clouds move one pixel */
#define CLOUD_PARALLAX_RATIO 5

#define NUM_CLOUDS 4
#define MIN_CLOUD_GAP 100
#define MAX_CLOUD_GAP 400
#define MIN_CLOUD_Y (30 + TOP_Y)
#define MAX_CLOUD_Y (71 + TOP_Y)
#define CLOUD_WIDTH 46

typedef struct Cloud {
    int24_t x;
    uint8_t y;
    uint24_t gap;
    const struct Cloud *last;
} cloud_t;

void add_cloud(cloud_t *new);

void update_clouds(void);

void update_cloud(cloud_t *cloud, uint8_t offset);

void init_clouds(void);

#endif //DINO_CLOUDS_H
