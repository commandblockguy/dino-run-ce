#ifndef DINO_CLOUDS_H
#define DINO_CLOUDS_H

#include <stdint.h>

/* For each CLOUD_PARALAX_RATIO pixels that the ground moves, the clouds move one pixel */
#define CLOUD_PARALAX_RATIO 5

#define NUM_CLOUDS 3

typedef struct Cloud {
    uint24_t base_distance;
    uint8_t y;
} cloud_t;

#endif //DINO_CLOUDS_H
