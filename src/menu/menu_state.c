#include "menu_state.h"

#include "../vdata/main_menu_tilemap.h"
#include "../vdata/match_menu_tilemap.h"

#include "../lib/sound.h"
#include "../lib/definitions.h"

/**
 * Index sprite tiles to option selector rect
 *
 * Sprites from 0 to 19
 */
void set_option_selector_sprites_tiles (void) {
    for (uint8_t i = 0; i < 8; i++) {
        set_sprite_tile(i, 10);    
    }

    for (uint8_t i = 8; i < 20; i++) {
        set_sprite_tile(i, 11);
    }
}

/**
 * Move option selector rect to some coordinate with specific height.
 * Max height is 6
 */
void move_option_selector (uint8_t anchor_x, uint8_t anchor_y, uint8_t heigth) {
    uint8_t y_offset = heigth * 8 + 1;
    uint8_t max_heigth_diff = 6 - heigth;

    for (uint8_t i = 0; i < 4; i++) {
        move_sprite(i, anchor_x + 8 * i, anchor_y);
        move_sprite(i + 4, anchor_x + 8 * i, anchor_y + y_offset);
    }

    for (uint8_t i = 8; i < 14; i++) {
        move_sprite(i, anchor_x - 1, anchor_y + 8 * (i-7));
        move_sprite(i + 6, anchor_x + 32, anchor_y + 8 * (i-7));
    }
    
    for (uint8_t i = 0; i < max_heigth_diff; i++) {
        hide_sprite(i + heigth + 8);
        hide_sprite(i + heigth + 8 + heigth + max_heigth_diff);
    }
}

bool handle_match_menu_char_selection (MenuState* menu_state, uint8_t current_joypad) {
    uint8_t index = 0;
    uint8_t value = 0;

    bool has_changes = FALSE;

    if (current_joypad & J_UP && !(menu_state->previous_joypad & J_UP)) {
        if (menu_state->option_index == CHAR_2_OPTION_INDEX) {
            index = 2;
        }
        has_changes = TRUE;
    }

    if (current_joypad & J_DOWN && !(menu_state->previous_joypad & J_DOWN)) {
        index = 1;
        if (menu_state->option_index == CHAR_2_OPTION_INDEX) {
            index = 3;
        }
        has_changes = TRUE;
    }

    if (has_changes) {
        value = menu_state->chars[index] + 1;
        if (value == 4) {
            value = 0;
        }

        menu_state->chars[index] = value;
    }

    return has_changes;
}

bool handle_match_menu_options_navigation (MenuState* menu_state, uint8_t current_joypad) {
    uint8_t previous_option_index = menu_state->option_index;

    if (current_joypad & J_RIGHT && !(menu_state->previous_joypad & J_RIGHT)) {
        menu_state->option_index = CHAR_2_OPTION_INDEX;
        if (previous_option_index == MATCH_MAIN_OPTION_INDEX) {
            menu_state->option_index = MATCH_MODE_OPTION_INDEX;
        }
    }

    if (current_joypad & J_LEFT && !(menu_state->previous_joypad & J_LEFT)) {
        menu_state->option_index = MATCH_MODE_OPTION_INDEX;
        if (previous_option_index != CHAR_2_OPTION_INDEX) {
            menu_state->option_index = CHAR_1_OPTION_INDEX;
        }
    }

    if (current_joypad & J_UP && !(menu_state->previous_joypad & J_UP) && menu_state->option_index == PLAY_OPTION_INDEX) {
        menu_state->option_index = MATCH_MODE_OPTION_INDEX;
    }

    if (current_joypad & J_DOWN && !(menu_state->previous_joypad & J_DOWN) && menu_state->option_index == MATCH_MODE_OPTION_INDEX) {
        menu_state->option_index = PLAY_OPTION_INDEX;
    }

    return previous_option_index != menu_state->option_index;
}

uint8_t* get_selector_params_from_match_menu_option (uint8_t option_index) {
    uint8_t params[3] = {CHAR_1_OPTION_X, CHAR_1_OPTION_Y, CHAR_1_OPTION_H};

    if (option_index == MATCH_MODE_OPTION_INDEX) {
        params[0] = MATCH_MODE_OPTION_X;
        params[1] = MATCH_MODE_OPTION_Y;
        params[2] = MATCH_MODE_OPTION_H;
    }

    if (option_index == PLAY_OPTION_INDEX) {
        params[0] = PLAY_OPTION_X;
        params[1] = PLAY_OPTION_Y;
        params[2] = PLAY_OPTION_H;
    }

    if (option_index == CHAR_2_OPTION_INDEX) {
        params[0] = CHAR_2_OPTION_X;
        params[1] = CHAR_2_OPTION_Y;
        params[2] = CHAR_2_OPTION_H;
    }

    return params;
}

void set_char_sprites (uint8_t* chars) {
    set_sprite_tile(CHAR_SPRITE_TILES_START_INDEX, CHAR_SPRITE_TILESET_START + chars[0]);
    set_sprite_tile(CHAR_SPRITE_TILES_START_INDEX + 1, CHAR_SPRITE_TILESET_START + chars[1] + 4);
    set_sprite_tile(CHAR_SPRITE_TILES_START_INDEX + 2, CHAR_SPRITE_TILESET_START + chars[2]);
    set_sprite_tile(CHAR_SPRITE_TILES_START_INDEX + 3, CHAR_SPRITE_TILESET_START + chars[3] + 4);

    set_sprite_prop(CHAR_SPRITE_TILES_START_INDEX + 2, S_FLIPX);
    set_sprite_prop(CHAR_SPRITE_TILES_START_INDEX + 3, S_FLIPX);
}

void hide_char_sprites (void) {
    for (uint8_t i = 0; i < 4; i++) {
        hide_sprite(CHAR_SPRITE_TILES_START_INDEX + i);
    }
}

void show_char_sprites (void) {
    move_sprite(CHAR_SPRITE_TILES_START_INDEX, CHAR_1_SPRITE_X, CHAR_1_SPRITE_Y);
    move_sprite(CHAR_SPRITE_TILES_START_INDEX + 1, CHAR_1_SPRITE_X, CHAR_1_SPRITE_Y + 8);
    move_sprite(CHAR_SPRITE_TILES_START_INDEX + 2, CHAR_2_SPRITE_X, CHAR_2_SPRITE_Y);
    move_sprite(CHAR_SPRITE_TILES_START_INDEX + 3, CHAR_2_SPRITE_X, CHAR_2_SPRITE_Y + 8);
}

void set_match_mode_sprites (uint8_t match_mode) {
    uint8_t number = 1;
    uint8_t description = MATCH_MODE_DESCRIPTION_MIN_TILESET_START;

    if (match_mode == MATCH_MODE_3_MIN) {
        number = 3;
    }

    if (match_mode == MATCH_MODE_3_GOALS) {
        number = 3;
        description = MATCH_MODE_DESCRIPTION_GOALS_TILESET_START;
    }

    if (match_mode == MATCH_MODE_7_GOALS) {
        number = 7;
        description = MATCH_MODE_DESCRIPTION_GOALS_TILESET_START;
    }

    set_sprite_tile(NUMBER_SPRITE_TILES_START_INDEX, number);
    set_sprite_tile(NUMBER_SPRITE_TILES_START_INDEX + 1, description);
    set_sprite_tile(NUMBER_SPRITE_TILES_START_INDEX + 2, description + 1);
}

void show_match_mode_sprites () {
    move_sprite(NUMBER_SPRITE_TILES_START_INDEX, MATCH_MODE_NUMBER_SPRITE_X, MATCH_MODE_DESCRIPTION_SPRITE_Y);
    move_sprite(NUMBER_SPRITE_TILES_START_INDEX + 1, MATCH_MODE_DESCRIPTION_SPRITE_1_X, MATCH_MODE_DESCRIPTION_SPRITE_Y);
    move_sprite(NUMBER_SPRITE_TILES_START_INDEX + 2, MATCH_MODE_DESCRIPTION_SPRITE_2_X, MATCH_MODE_DESCRIPTION_SPRITE_Y);
}

void hide_match_mode_sprites () {
    hide_sprite(NUMBER_SPRITE_TILES_START_INDEX);
    hide_sprite(NUMBER_SPRITE_TILES_START_INDEX + 1);
    hide_sprite(NUMBER_SPRITE_TILES_START_INDEX + 2);
}

void set_player_side_sprite () {
    set_sprite_tile(PLAYER_SIDE_SPRITE_INDEX, PLAYER_SIDE_SPRITE_TILESET_INDEX);
}

void hide_player_side_sprite () {
    hide_sprite(PLAYER_SIDE_SPRITE_INDEX);
}

void move_player_side_sprite (uint8_t player_side) {
    if (player_side == PLAYER_SIDE_LEFT) {
        move_sprite(PLAYER_SIDE_SPRITE_INDEX, 44, 104);
    } else {
        move_sprite(PLAYER_SIDE_SPRITE_INDEX, 124, 104);
    }
}

void init_menu_state (MenuState* menu_state) {
    menu_state->menu_index = MAIN_MENU_INDEX;
    menu_state->next_menu_index = MAIN_MENU_INDEX;
    menu_state->previous_menu_index = MATCH_MENU_INDEX;
    menu_state->option_index = MATCH_MAIN_OPTION_INDEX;

    menu_state->match_mode = MATCH_MODE_1_MIN;
    menu_state->chars[0] = 0;
    menu_state->chars[1] = 0;
    menu_state->chars[2] = 1;
    menu_state->chars[3] = 1;
    menu_state->player_side = PLAYER_SIDE_LEFT;

    menu_state->previous_joypad = 0;
    menu_state->is_match_ready = FALSE;

    set_option_selector_sprites_tiles();
    set_char_sprites(menu_state->chars);
    set_match_mode_sprites(menu_state->match_mode);
    set_player_side_sprite();
    set_mono();
}

void update_main_menu_state (MenuState* menu_state, uint8_t current_joypad) {
    if (menu_state->previous_menu_index != MAIN_MENU_INDEX) {
        menu_state->option_index = MATCH_MAIN_OPTION_INDEX;
        set_bkg_tiles(0, 0, MainMenuTilemapWidth, MainMenuTilemapHeight, MainMenuTilemap);
        move_option_selector(MATCH_MAIN_OPTION_X, MATCH_MAIN_OPTION_Y, MATCH_MAIN_OPTION_H);
        hide_char_sprites();
        hide_match_mode_sprites();
        hide_player_side_sprite();
    }
    if (current_joypad & J_A && !(menu_state->previous_joypad & J_A)) {
        menu_state->next_menu_index = MATCH_MENU_INDEX;
        play_click_sound();
    }
}

void update_match_menu_state (MenuState* menu_state, uint8_t current_joypad) {
    if (menu_state->previous_menu_index != menu_state->menu_index) {
        set_bkg_tiles(0, 0, MatchMenuTilemapWidth, MatchMenuTilemapHeight, MatchMenuTilemap);
        move_option_selector(CHAR_1_OPTION_X, CHAR_1_OPTION_Y, CHAR_1_OPTION_H);
        show_char_sprites();
        show_match_mode_sprites();
        move_player_side_sprite(menu_state->player_side);
    }

    if (current_joypad & J_B && !(menu_state->previous_joypad & J_B)) {
        menu_state->next_menu_index = MAIN_MENU_INDEX;
        play_click_sound();
    }

    if (current_joypad & J_A && !(menu_state->previous_joypad & J_A)) {
        if (menu_state->option_index == MATCH_MODE_OPTION_INDEX) {

            menu_state->match_mode = menu_state->match_mode + 1;
            if (menu_state->match_mode == 4) {
                menu_state->match_mode = 0;
            }
            set_match_mode_sprites(menu_state->match_mode);
        }
        if (menu_state->option_index == PLAY_OPTION_INDEX) {
            hide_char_sprites();
            hide_match_mode_sprites();
            hide_player_side_sprite();
            move_option_selector(0, 0, 0);
            menu_state->is_match_ready = 1;
        }

        if (menu_state->option_index == CHAR_1_OPTION_INDEX || menu_state->option_index == CHAR_2_OPTION_INDEX) {
            menu_state->player_side = menu_state->option_index == CHAR_2_OPTION_INDEX;
            move_player_side_sprite(menu_state->player_side);
        }

        play_click_sound();
    }


    if (menu_state->option_index == CHAR_1_OPTION_INDEX || menu_state->option_index == CHAR_2_OPTION_INDEX) {
        bool has_changes = handle_match_menu_char_selection(menu_state, current_joypad);
        if (has_changes) {
            set_char_sprites(menu_state->chars);
            play_jump_sound(0);
        }
    }

    bool has_changes = handle_match_menu_options_navigation(menu_state, current_joypad);
    if (has_changes) {
        uint8_t* params = get_selector_params_from_match_menu_option(menu_state->option_index);
        move_option_selector(params[0], params[1], params[2]);
    }
}

void update_menu_state (MenuState* menu_state, uint8_t current_joypad) {
    if (menu_state->menu_index == MAIN_MENU_INDEX) {
        update_main_menu_state(menu_state, current_joypad);
    }

    if (menu_state->menu_index == MATCH_MENU_INDEX) {
        update_match_menu_state(menu_state, current_joypad);
    }

    menu_state->previous_joypad = current_joypad;
    menu_state->previous_menu_index = menu_state->menu_index;
    menu_state->menu_index = menu_state->next_menu_index;
}
