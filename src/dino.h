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

#define DINO_HEIGHT 47
#define DINO_WIDTH 44
#define DINO_WIDTH_DUCK 59
#define DINO_HEIGHT_DUCK 25

/* Check if dino is on ground and if so start a jump */
void dino_start_jump(void);

/* Stop jump and start falling if not doing so already */
void dino_end_jump(void);

void update_dino(void);

#endif //DINO_DINO_H
