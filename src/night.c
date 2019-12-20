#include "gamestate.h"
#include "night.h"
#include "sprites.h"

void update_day_stage(game_t *game) {
    if(game->distance > game->distance_to_time_change) {
        game->day_stage++;
        if(game->day_stage >= NUM_DAY_STAGES)
            game->day_stage = DAY;

        switch(game->day_stage) {
            case DAY:
                game->distance_to_time_change += DAY_LENGTH;
                break;
            case DUSK:
                game->star_movement_frame = game->frame;
                game->moon.movement_frame = game->frame;
                game->moon.phase++;
                if(game->moon.phase > NUM_MOONS) game->moon.phase = 0;
                game->moon.x = LCD_WIDTH - 50;
                place_stars(game->stars);
            case DAWN:
                game->distance_to_time_change += INVERSION_DISTANCE;
                break;
            case NIGHT:
                game->distance_to_time_change += NIGHT_LENGTH;
                break;
        }
    }

    if(game->day_stage != DAY) {
        update_moon(&game->moon, game->frame);
        update_stars(game->stars, game->frame, &game->star_movement_frame);
    }
}

void update_stars(star_t *stars, uint24_t frame, uint24_t *next_frame) {
    uint8_t i, offset = 0;
    while(frame >= *next_frame) {
        offset++;
        *next_frame += STAR_SPEED_DIVISOR;
    }

    for(i = 0; i < MAX_NUM_STARS; i++) {
        update_star(&stars[i], offset);
    }
}

void update_star(star_t *star, uint8_t offset) {
    star->x -= offset;
    if(star->x + STAR_WIDTH < 0)
        star->x = LCD_WIDTH;
}

void update_moon(moon_t *moon, uint24_t frame) {
    uint8_t offset = 0;
    uint8_t width = MOON_WIDTH;
    while(frame >= moon->movement_frame) {
        offset++;
        moon->movement_frame += MOON_SPEED_DIVISOR;
    }
    moon->x -= offset;

    if(moon->phase == 3) width = 2 * MOON_WIDTH;
    if(moon->x + width < 0)
        moon->x = LCD_WIDTH;
}

void place_stars(star_t *stars) {
    stars[0].x = randInt(0, LCD_WIDTH / 2);
    stars[0].y = randInt(TOP_Y, MAX_STAR_Y);

    stars[1].x = randInt(LCD_WIDTH / 2, LCD_WIDTH);
    stars[1].y = randInt(TOP_Y, MAX_STAR_Y);
}
