#ifndef _BOT_H_
#define _BOT_H_

#include <gb/gb.h>
#include <rand.h>

#include "ball.h"
#include "player.h"


typedef struct {
    // uint8_t kind;
    Player* player;

    uint8_t finger_force;
    uint8_t finger_cooldown;
    uint8_t last_command;
} Bot;

void init_bot (Bot* bot, Player* player/*, uint8_t kind*/);
uint8_t get_bot_command (Bot* bot, Ball* ball);

#endif