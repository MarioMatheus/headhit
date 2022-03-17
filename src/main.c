#include <gb/gb.h>
#include <stdbool.h>

#include "vdata/background_tileset.h"
#include "vdata/sprites_tileset.h"
#include "lib/sound.h"

#include "menu/menu_state.h"
#include "match/match_state.h"

void main () {
    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    set_bkg_data(0, 116, BackgroundTileset);
    set_sprite_data(0, 50, SpritesTileset);

    disable_sound();

    MenuState menu_state;
    // init_menu_state(&menu_state);
    menu_state.is_match_ready = TRUE;
    menu_state.match_mode = 0;

    bool match_initialized = FALSE;
    MatchState match_state;

    while (1) {
        uint8_t current_joypad = joypad();

        // if (!menu_state.is_match_ready) {
        //     update_menu_state(&menu_state, current_joypad);
        // }

        if (menu_state.is_match_ready && !match_initialized) {
            init_match_state(&match_state, menu_state.match_mode);
            match_initialized = 1;
        }

        if (menu_state.is_match_ready) {
            update_match_state(&match_state, current_joypad);
        }

        wait_vbl_done();
    }
}
