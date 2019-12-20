#ifndef DINO_UTIL_H
#define DINO_UTIL_H

#include <stdint.h>

typedef union {
    int24_t combined;
    struct {
        int16_t fPart;
        int8_t iPart;
    } parts;
} ifix_t;

typedef union {
    uint24_t combined;
    struct {
        uint16_t fPart;
        uint8_t iPart;
    } parts;
} ufix_t;

#define INT_TO_FIXED_POINT(i) (((i) << 16))
#define FLOAT_TO_FIXED_POINT(i) (((i) * (1 << 16)))

#define str(a) #a
#define xstr(a) str(a)

#endif //DINO_UTIL_H
