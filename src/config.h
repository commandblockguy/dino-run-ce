#ifndef DINO_CONFIG_H
#define DINO_CONFIG_H

#include <tice.h>

#define GAME_HEIGHT 150
#define TOP_Y ((LCD_HEIGHT - GAME_HEIGHT) / 2)
#define BOTTOM_Y (GAME_HEIGHT + TOP_Y)
#define GROUND_LEVEL (BOTTOM_Y - 12)

#define FPS 60
#define ONE_SECOND 32768
#define FRAME_TIME (ONE_SECOND / FPS)

#ifndef SHOW_FPS
#define SHOW_FPS 0
#endif SHOW_FPS
#ifndef SHOW_BOXES
#define SHOW_BOXES 0
#endif
#ifndef USE_CHEATS
#define USE_CHEATS 0
#endif

#define GRAVITY 0.6
#define INITIAL_JUMP_VELOCITY -10
#define DROP_VELOCITY -5
#define SPEED_DROP_COEFFICIENT 3
#define MAX_JUMP_HEIGHT 63
#define MIN_JUMP_HEIGHT 30

#define INITIAL_SPEED 6
#define ACCELERATION 0.001
#define MAX_SPEED 13

#define DINO_OFFSET_X 13

#define SCORE_DIVISOR 40
#define SCORE_DIGITS 5
#define DISTANCE_METER_MAX 99999

#define LETTERBOX 1

#ifndef USE_USB
#define USE_USB 0
#endif
#define MAX_HID_DEVICES 4

#ifndef COMMIT
#define COMMIT (N/A)
#endif
#ifndef DIFF_STATUS
#define DIFF_STATUS (N/A)
#endif
#ifndef VERSION
#define VERSION 1.0
#endif


#endif //DINO_CONFIG_H
