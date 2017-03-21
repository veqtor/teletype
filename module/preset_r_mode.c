#include "preset_r_mode.h"

// this
#include "flash.h"
#include "fudge.h"
#include "keyboard_helper.h"

// libavr32
#include "font.h"
#include "region.h"
#include "util.h"

// asf
#include "conf_usb_host.h"  // needed in order to include "usb_protocol_hid.h"
#include "usb_protocol_hid.h"

uint8_t offset;

void set_preset_r_mode() {
    offset = 0;
}

void process_preset_r_knob(uint8_t knob, uint8_t mod_key) {
    preset_select = knob;
    r_edit_dirty = R_ALL;
}

void process_preset_r_keys(uint8_t k, uint8_t m, bool is_held_key) {
    if (match_no_mod(m, k, HID_DOWN)) {  // down
        if (offset < SCENE_TEXT_LINES - 8) {
            offset++;
            r_edit_dirty |= R_ALL;
        }
    }
    else if (match_no_mod(m, k, HID_UP)) {  // up
        if (offset) {
            offset--;
            r_edit_dirty |= R_ALL;
        }
    }
    else if (match_no_mod(m, k, HID_LEFT) ||
             match_no_mod(m, k, HID_OPEN_BRACKET)) {  // left or [
        if (preset_select) preset_select--;
        r_edit_dirty |= R_ALL;
    }

    else if (match_no_mod(m, k, HID_RIGHT) ||
             match_no_mod(m, k, HID_CLOSE_BRACKET)) {  // right or ]
        if (preset_select < SCENE_SLOTS - 1) preset_select++;
        r_edit_dirty |= R_ALL;
    }
    else if (match_no_mod(m, k, HID_ENTER) && !is_held_key) {  // enter
        flash_read(preset_select);
        tele_set_scene(preset_select);

        run_script(INIT_SCRIPT);

        set_mode(last_mode);
    }
}

void screen_refresh_preset_r() {
    if (!(r_edit_dirty & R_ALL)) { return; }

    char s[32];
    itoa(preset_select, s, 10);
    region_fill(&line[0], 1);
    font_string_region_clip_right(&line[0], s, 126, 0, 0xf, 1);
    font_string_region_clip(&line[0], f.s[preset_select].text[0], 2, 0, 0xf, 1);


    for (uint8_t y = 1; y < 8; y++) {
        region_fill(&line[y], 0);
        font_string_region_clip(&line[y], f.s[preset_select].text[offset + y],
                                2, 0, 0xa, 0);
    }

    r_edit_dirty &= ~R_ALL;
    screen_dirty = true;
};
