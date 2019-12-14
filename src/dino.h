#ifndef DINO_DINO_H
#define DINO_DINO_H

#include <stdint.h>
#include <stdbool.h>

#include "util.h"

typedef struct Dino {
    ufix_t y;
    ifix_t velocity_x;
    ifix_t velocity_y;
    bool alive;
    bool on_ground;
    bool ducking;
    bool dropping;
    bool jumping;
} dino_t;

/* Check if dino is on ground and if so start a jump */
void dino_start_jump(dino_t *dino);

/* Stop jump and start falling if not doing so already */
void dino_end_jump(dino_t *dino);

void update_dino(dino_t *dino);

#endif //DINO_DINO_H
