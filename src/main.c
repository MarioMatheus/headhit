#include <gb/gb.h>

#include "vdata/menu_tileset.h"
#include "vdata/sprites_tileset.h"
#include "menu/menu_state.h"
#include "lib/sound.h"

int main () {
    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    set_bkg_data(0, 96, MenuTileset);
    set_sprite_data(0, 36, SpritesTileset);

    enable_sound();

    MenuState menu_state;
    init_menu_state(&menu_state);


    while (1) {
        uint8_t current_joypad = joypad();
        update_menu_state(&menu_state, current_joypad);
        wait_vbl_done();
    }
}
