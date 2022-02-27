#include "sound.h"

void enable_sound() {
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;
}

void play_click_sound() {
    NR10_REG = 0x00;
    NR11_REG = 0x81;
    NR12_REG = 0x43;
    NR13_REG = 0x73;
    NR14_REG = 0x86;
}

void play_jump_sound() {
    NR10_REG = 0x15;
    NR11_REG = 0x96;
    NR12_REG = 0x73;
    NR13_REG = 0xBB;
    NR14_REG = 0x85;
}

void play_land_on_ground_sound() {
    NR10_REG = 0x79;
    NR11_REG = 0x8D;
    NR12_REG = 0x63;
    NR13_REG = 0xC8;
    NR14_REG = 0x80;
}

void play_game_over_sound() {
    NR10_REG = 0x4F;
    NR11_REG = 0x96;
    NR12_REG = 0xB7;
    NR13_REG = 0xBB;
    NR14_REG = 0x85;
}
