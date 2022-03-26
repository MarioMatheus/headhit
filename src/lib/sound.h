#ifndef _LIB_SOUND_H_
#define _LIB_SOUND_H_

#include <gb/gb.h>

void enable_sound();
void disable_sound();
void set_mono();
void set_stereo();

void play_click_sound();
void play_bounce_sound(uint8_t left);
void play_jump_sound(uint8_t left);
void play_land_on_ground_sound();
void play_game_over_sound();

#endif
