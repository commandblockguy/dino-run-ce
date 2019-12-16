#include <stddef.h>
#include <fontlibc.h>

#include "font.h"

static const uint8_t font_data[] = {
        /* `make font` if font.inc does not exist */
#include "font.inc"
};

const fontlib_font_t *font = (fontlib_font_t *)font_data;
