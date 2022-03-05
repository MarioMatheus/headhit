#include "player.h"

#include "../lib/definitions.h"

#include <stdio.h>


void set_player_sprite_data (uint8_t char_sprite) {
    
    set_sprite_tile(PLAYER_SPRITE_INDEX + 1, PLAYER_SPRITE_TITESET_START_INDEX + (char_sprite & 0x03));
    set_sprite_tile(PLAYER_SPRITE_INDEX, PLAYER_SPRITE_TITESET_START_INDEX + 4 + (char_sprite & 0x0C));

    if (char_sprite & 0xF0) {
        set_sprite_prop(PLAYER_SPRITE_INDEX + 1, S_FLIPX);
        set_sprite_prop(PLAYER_SPRITE_INDEX, S_FLIPX);
    }

}

void move_player_sprite (Player* player) {
    uint8_t x_head = player->x.b.h;
    uint8_t y_head = player->y.b.h - 8;

    if (player->movement != 0) {
        x_head += player->movement;
        y_head++;
    }

    move_sprite(PLAYER_SPRITE_INDEX + 1, x_head, y_head);
    move_sprite(PLAYER_SPRITE_INDEX, player->x.b.h, player->y.b.h);
}

void hide_player () {
    hide_sprite(PLAYER_SPRITE_INDEX + 1);
    hide_sprite(PLAYER_SPRITE_INDEX);
}

void move_player_on_the_y_axis (Player* player) {
    if (player->is_falling) {
        player->y.w += player->y_speed;
    } else {
        player->y.w -= player->y_speed;
    }
}

void apply_gravity_in_player (Player* player) {
    if (player->is_falling) {
        player->y_speed += player->gravity;
    } else {
        player->y_speed -= player->gravity;
    }

    if (!player->is_falling && player->gravity >= player->y_speed) {
        player->is_falling = TRUE;
    }
}

void put_player_on_the_green_carpet (Player* player, uint8_t char_sprite) {
    player->char_sprite = char_sprite;

    player->x.w = 56 << 8;
    player->y.w = PLAYER_SPRITE_MIN_Y << 8;

    player->gravity = 15;
    player->x_speed = 200;
    player->y_speed = 0;

    player->jump_force = 300;
    player->is_falling = FALSE;
    player->movement = 0;

    player->stadium_width = PLAYER_SPRITE_MAX_X;
    player->stadium_height = PLAYER_SPRITE_MIN_Y;

    set_player_sprite_data(player->char_sprite);
    move_player_sprite(player);
}

void update_player_movement (Player* player, uint8_t current_joypad, uint8_t previous_joypad) {
    move_player_on_the_y_axis(player);

    if (player->y.h == player->stadium_height && current_joypad & J_B && !(previous_joypad & J_B)) {
        player->y_speed = player->jump_force;
    }

    if (player->y_speed != 0 || player->y.h < player->stadium_height) {
        apply_gravity_in_player(player);
        if (player->y.h > player->stadium_height) {
            player->y.w = PLAYER_SPRITE_MIN_Y << 8;
            player->is_falling = FALSE;
            player->y_speed = 0;
        }
    }

    if (current_joypad & J_RIGHT) {
        player->movement = 1;
        if (((player->x.w + player->x_speed) >> 8) > player->stadium_width) {
            player->x.w = player->stadium_width << 8;
        } else {
            player->x.w += player->x_speed;
        }
    } else if (current_joypad & J_LEFT) {
        player->movement = -1;
        if (((player->x.w - player->x_speed) >> 8) < 8) {
            player->x.w = 8 << 8;
        } else {
            player->x.w -= player->x_speed;
        }
    } else {
        player->movement = 0;
    }

    move_player_sprite(player);
}
