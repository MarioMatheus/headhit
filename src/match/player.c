#include "player.h"

#include "../lib/definitions.h"
#include "../lib/sound.h"

int8_t kick_animation_x_offsets[] = {-1, 0, 1, 1, 1, 0, -1, 0};
int8_t kick_animation_y_offsets[] = {0, 0, 0, -1, 0, 0, 0, 0};

int8_t headhit_animation_x_offsets[] = {-1, 0, 1, 2, 1, 0, -1, 0};
int8_t headhit_animation_y_offsets[] = {1, 0, 1, 1, 1, 0, 1, 0};

uint8_t get_player_sprite_index (uint8_t char_sprite) {
    if (char_sprite & 0xF0) {
        return PLAYER_SPRITE_INDEX + 2;
    }
    return PLAYER_SPRITE_INDEX;
}

void set_player_sprite_data (uint8_t char_sprite) {
    uint8_t sprite_index = get_player_sprite_index(char_sprite);

    set_sprite_tile(sprite_index + 1, PLAYER_SPRITE_TITESET_START_INDEX + (char_sprite & 0x03));
    set_sprite_tile(sprite_index, PLAYER_SPRITE_TITESET_START_INDEX + 4 + ((char_sprite & 0x0C) >> 2));

    if (char_sprite & 0xF0) {
        set_sprite_prop(sprite_index + 1, S_FLIPX);
        set_sprite_prop(sprite_index, S_FLIPX);
    }
}

void move_player_sprite (Player* player) {
    uint8_t x_head = player->x.b.h;
    uint8_t y_head = player->y.b.h - 8;

    int8_t x_head_offset = 0;
    int8_t y_head_offset = 0;
    int8_t x_offset = 0;
    int8_t y_offset = 0;

    uint8_t sprite_index = get_player_sprite_index(player->char_sprite);

    if (player->kick_animation > 0) {
        if (player->head_hit) {
            x_head_offset = headhit_animation_x_offsets[player->kick_animation - 1];
            y_head_offset = headhit_animation_y_offsets[player->kick_animation - 1];
        } else {
            x_offset = kick_animation_x_offsets[player->kick_animation - 1];
            y_offset = kick_animation_y_offsets[player->kick_animation - 1];
        }
        
        if (player->char_sprite & 0xF0) {
            x_offset *= -1;
            x_head_offset *= -1;
        }

        if (player->relative_frame % 2 == 0) {
            player->kick_animation++;
            if (player->kick_animation > 8) {
                player->kick_animation = 0;
                player->head_hit = FALSE;
            }
        }
    }

    if (!player->head_hit && player->movement != 0) {
        x_head += player->movement;
        y_head++;
    }

    move_sprite(sprite_index + 1, x_head + x_head_offset, y_head + y_head_offset);
    move_sprite(sprite_index, player->x.b.h + x_offset, player->y.b.h + y_offset);
}

void hide_player (Player* player) {
    uint8_t sprite_index = get_player_sprite_index(player->char_sprite);
    hide_sprite(sprite_index + 1);
    hide_sprite(sprite_index);
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

void increase_player_relative_frame (Player* player) {
    player->relative_frame++;
    if (player->relative_frame == 60) {
        player->relative_frame = 0;
    }
}

void add_score_to_player (Player* player) {
    if (player->goals < 99) {
        player->goals++;
    }
}

void put_player_on_the_green_carpet (Player* player, uint8_t char_sprite, uint8_t goals) {
    player->char_sprite = char_sprite;

    player->y.w = PLAYER_SPRITE_MIN_Y << 8;
    player->x.w = 56 << 8;
    if (char_sprite & 0xF0) {
        player->x.w *= 2;
    }

    player->gravity = 15;
    player->x_speed = 200;
    player->y_speed = 0;

    player->jump_force = 300;
    player->is_falling = FALSE;
    player->movement = 0;

    player->stadium_width = PLAYER_SPRITE_MAX_X;
    player->stadium_height = PLAYER_SPRITE_MIN_Y;

    player->j_a_tapped = FALSE;
    player->kick_cooldown = 0;
    player->kick_animation = 0;
    player->head_hit = FALSE;

    player->in_collision_with_ball = FALSE;

    player->relative_frame = 0;

    player->goals = goals;

    set_player_sprite_data(player->char_sprite);
    move_player_sprite(player);
}

void update_player_movement (Player* player, uint8_t current_joypad, uint8_t previous_joypad) {
    move_player_on_the_y_axis(player);

    // Jump
    if (player->y.h == player->stadium_height && current_joypad & J_B && !(previous_joypad & J_B)) {
        player->y_speed = player->jump_force;
        play_jump_sound(player->x.h < player->stadium_width / 2);
    }

    // Kick
    player->j_a_tapped = FALSE;

    if (player->kick_cooldown > 0) {
        player->kick_cooldown--;
    }

    if (player->kick_cooldown == 0 && current_joypad & J_A && !(previous_joypad & J_A)) {
        player->j_a_tapped = TRUE;
        player->head_hit = player->y_speed > 0;
        player->kick_cooldown = 30;
        player->kick_animation = 1;
    }

    // Gravity
    if (player->y_speed != 0 || player->y.h < player->stadium_height) {
        apply_gravity_in_player(player);
        if (player->y.h > player->stadium_height) {
            player->y.w = PLAYER_SPRITE_MIN_Y << 8;
            player->is_falling = FALSE;
            player->y_speed = 0;
        }
    }

    // Movement
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

    // Procedures
    move_player_sprite(player);
    increase_player_relative_frame(player);
}
