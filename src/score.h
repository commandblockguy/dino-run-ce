#ifndef DINO_SCORE_H
#define DINO_SCORE_H

#include <stdint.h>
#include <fileioc.h>

#define SCORE_APPVAR_NAME "DinoHS"

uint24_t get_score(void);
void set_score(uint24_t score);

#endif //DINO_SCORE_H
