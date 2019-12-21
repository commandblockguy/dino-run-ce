#include <debug.h>
#include "sound.h"
#include "lib/steam_controller.h"
#include "gamestate.h"

#if USE_SOUND

void play_sound(sound_player_t *player, const sound_t *sound) {
    player->sound = sound;

    if(sound->num_tones > 1) {
        player->next_tone = 1;
        player->next_tone_time = (uint24_t)timer_2_Counter +
                                 sound->tones[1].time;
        dbg_sprintf(dbgout, "player %p\n", player);
        dbg_sprintf(dbgout, "more than one tone\n");
        dbg_sprintf(dbgout, "time %u\n", sound->tones[1].time);
    } else {
        player->next_tone = 0;
        player->next_tone_time = 0;
        dbg_sprintf(dbgout, "only one tone\n");
    }

    play_tone(&sound->tones[0]);
}

void update_sound_player(sound_player_t *player) {
    if(!player->next_tone || !player->next_tone_time) return;

    if((uint24_t)timer_2_Counter > player->next_tone_time) {
        uint24_t current_relative_time, new_relative_time;
        play_tone(&player->sound->tones[player->next_tone]);

        current_relative_time = player->sound->tones[player->next_tone].time;

        player->next_tone++;

        if(player->next_tone >= player->sound->num_tones) {
            player->sound = NULL;
            player->next_tone = 0;
            player->next_tone_time = 0;
            return;
        }

        new_relative_time = player->sound->tones[player->next_tone].time;
        player->next_tone_time += new_relative_time - current_relative_time;
    }
}

void play_tone(const tone_t *tone) {
    sc_PlayTone(game.usb.controller_dev, game.usb.controller_interface,
                SC_HAPTIC_RIGHT, tone->pulse_duration, tone->repeat_count);
}

const sound_t sounds[3] = {
    {
        1,
        {
            {
                0,
                sc_FrequencyToDuration(527),
                sc_TimeToRepeatCount(36, 527)
            }
        }
    },
    {
        2,
        {
            {
                0,
                sc_FrequencyToDuration(67),
                2
            },
            {
                68 * 32768 / 1000,
                sc_FrequencyToDuration(67),
                5
            }
        }
    },
    {
        2,
        {
            {
                0,
                sc_FrequencyToDuration(525),
                sc_TimeToRepeatCount(80, 525)
            },
            {
                90 * 32768 / 1000,
                sc_FrequencyToDuration(786),
                sc_TimeToRepeatCount(220,786)
            }
        }
    },
};

#endif
