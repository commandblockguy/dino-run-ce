#ifndef DINO_UTIL_H
#define DINO_UTIL_H

#include <stdint.h>

typedef union {
    int24_t combined;
    struct {
        int16_t fPart;
        uint8_t iPart;
    } parts;
} fixed_point_t;

#define INT_TO_FIXED_POINT(i) ((int24_t)((i) << 16))
#define FLOAT_TO_FIXED_POINT(i) ((int24_t)((i) * (1 << 16)))

#endif //DINO_UTIL_H
