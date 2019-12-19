#include "score.h"

uint24_t get_score(void) {
    ti_var_t slot;
    uint24_t score;

    slot = ti_Open(SCORE_APPVAR_NAME, "r");
    if(!slot) return 0;

    ti_Read(&score, sizeof(score), 1, slot);

    ti_Close(slot);
    if(score > 99999) return 99999;
    return score;
}

void set_score(uint24_t score) {
    ti_var_t slot;
    uint24_t old_score = get_score();

    if(score > 99999) score = 99999;

    if(score <= old_score) return;

    slot = ti_Open(SCORE_APPVAR_NAME, "w");
    if(!slot) return;

    ti_Write(&score, sizeof(score), 1, slot);

    ti_SetArchiveStatus(true, slot);
    ti_Close(slot);
}
