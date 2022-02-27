#include <gb/gb.h>

#include "vdata/background_tileset.h"
#include "vdata/sprites_tileset.h"
#include "lib/sound.h"

#include "menu/menu_state.h"
#include "match/match_state.h"

void main () {
    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    set_bkg_data(0, 114, BackgroundTileset);
    set_sprite_data(0, 36, SpritesTileset);

    enable_sound();

    MenuState menu_state;
    init_menu_state(&menu_state);

    uint8_t in_match = 0;
    MatchState match_state;

    while (1) {
        uint8_t current_joypad = joypad();

        if (!menu_state.is_match_ready) {
            update_menu_state(&menu_state, current_joypad);
        }

        if (menu_state.is_match_ready && !in_match) {
            init_match_state(&match_state, menu_state.match_mode);
            in_match = 1;
        }

        wait_vbl_done();
    }
}
