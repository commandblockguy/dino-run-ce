#include "night.h"

void update_day_stage(game_t *game) {
    if(game->distance > game->distance_to_time_change) {
        game->day_stage++;
        if(game->day_stage >= NUM_DAY_STAGES)
            game->day_stage = DAY;

        switch(game->day_stage) {
            case DAY:
                game->distance_to_time_change += DAY_LENGTH;
                break;
            case DAWN:
            case DUSK:
                game->distance_to_time_change += INVERSION_DISTANCE;
                break;
            case NIGHT:
                game->distance_to_time_change += NIGHT_LENGTH;
                break;
        }
    }
}
