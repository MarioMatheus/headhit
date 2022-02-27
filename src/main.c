#include <gb/gb.h>

#include "vdata/background_tileset.h"
#include "vdata/sprites_tileset.h"
#include "menu/menu_state.h"
#include "lib/sound.h"

#include "vdata/game_match_tilemap.h"

void main () {
    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    set_bkg_data(0, 114, BackgroundTileset);
    set_sprite_data(0, 36, SpritesTileset);

    enable_sound();

    MenuState menu_state;
    init_menu_state(&menu_state);

    uint8_t tmp_flag = 0;

    while (1) {
        uint8_t current_joypad = joypad();

        if (!menu_state.is_match_ready) {
            update_menu_state(&menu_state, current_joypad);
        }

        if (menu_state.is_match_ready && !tmp_flag) {
            set_bkg_tiles(0, 0, GameMatchTilemapWidth, GameMatchTilemapHeight, GameMatchTilemap);
            tmp_flag = 1;
        }

        wait_vbl_done();
    }
}
