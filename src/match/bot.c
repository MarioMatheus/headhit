#include "bot.h"

uint8_t dumb_bot(Player* self, Ball* ball) {
    if (
        ball->x.h > self->x.h - 10
        && ball->x.h + 8 < self->x.h + 4
        && (
            self->y_speed == 0 && ball->y.h > self->y.h - 4
            || self->y_speed > 0 && ball->y.h > self->y.h - 14 && ball->y.h < self->y.h - 6
        )
    ) {
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
    
    bool hovering_over_head = (
        ball->x.h > self->x.h - 14
        && ball->x.h + 8 < self->x.h + 4
        && ball->y.h < self->y.h - 14
        && ball->x_speed < 100
    );

    if (!hovering_over_head && ball->x.h < self->x.h - 8) {
        return J_LEFT;
    }

    if (!hovering_over_head && ball->x.h > self->x.h - 8) {
        return J_RIGHT;
    }

    return 0;
}

BotThinking* get_bot_thinking() {
    return dumb_bot;
}