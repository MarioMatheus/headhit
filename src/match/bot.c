#include "bot.h"

uint8_t dumb_bot(Player* self, Ball* ball) {
    if (ball->x.h > self->x.h - 3 && ball->x.h < self->x.h + 4 && ball->y.h > self->y.h - 8) {
        return J_A;
    }
    if (
        ball->x.h > self->x.h - 3
        && ball->x.h < self->x.h + 8
        && ball->y.h > self->y.h - 22
        && ball->y.h < self->y.h - 16
    ) {
        return J_B;
    }
    if (ball->x.h < self->x.h) {
        return J_LEFT;
    }
    if (ball->x.h > self->x.h) {
        return J_RIGHT;
    }
    return 0;
}

BotThinking* get_bot_thinking() {
    return dumb_bot;
}