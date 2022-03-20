#ifndef _BOT_H_
#define _BOT_H_

#include <gb/gb.h>

#include "ball.h"
#include "player.h"

typedef uint8_t BotThinking(Player* self, Ball* ball /*, Player* opponent*/);

BotThinking* get_bot_thinking(/*uint8_t kind*/);

#endif