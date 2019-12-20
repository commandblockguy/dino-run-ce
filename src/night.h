#ifndef DINO_NIGHT_H
#define DINO_NIGHT_H

#include <stdint.h>

enum {
    DAY,
    DUSK,
    NIGHT,
    DAWN,
    NUM_DAY_STAGES
};

#define NIGHT_CYCLE_INTERVAL 28000
#define INVERSION_FRAMES 32
#define INVERSION_DISTANCE (INVERSION_FRAMES * MAX_SPEED)
#define NIGHT_LENGTH (8000 - INVERSION_DISTANCE)
#define DAY_LENGTH (20000 - INVERSION_DISTANCE)

#define MOON_SPEED_DIVISOR 4
#define STAR_SPEED_DIVISOR 3

#define MAX_NUM_STARS 2

#define STAR_WIDTH 9
#define MOON_WIDTH 20

#define MAX_STAR_Y (TOP_Y + 70)

typedef struct {
    int24_t x;
    uint8_t y;
    uint8_t type;
} star_t;

typedef struct {
    uint8_t phase;
    int24_t x;
    uint24_t movement_frame;
} moon_t;

void update_day_stage(void);

void update_moon(void);

void update_stars(void);

void update_star(star_t *star, uint8_t offset);

void place_stars(void);

#endif //DINO_NIGHT_H
