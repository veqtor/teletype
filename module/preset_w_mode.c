#include "preset_w_mode.h"

#include <string.h>

// this
#include "flash.h"
#include "fudge.h"
#include "keyboard_helper.h"
#include "line_editor.h"

// libavr32
#include "font.h"
#include "kbd.h"
#include "region.h"
#include "util.h"

// asf
#include "conf_usb_host.h"  // needed in order to include "usb_protocol_hid.h"
#include "usb_protocol_hid.h"

uint8_t edit_line;
uint8_t edit_offset;
line_editor_t le;

void set_preset_w_mode() {
    edit_line = 0;
    edit_offset = 0;
    line_editor_set(&le, scene_text[0]);
}

void process_preset_w_keys(uint8_t k, uint8_t m, bool is_held_key) {
    if (match_no_mod(m, k, HID_DOWN) || match_ctrl(m, k, HID_N)) {  // down
        if ((edit_offset + edit_line) < 31) {
            if (edit_line == 5)
                edit_offset++;
            else
                edit_line++;
            line_editor_set(&le, scene_text[edit_line + edit_offset]);
            r_edit_dirty |= R_ALL;
        }
    }
    else if (match_no_mod(m, k, HID_UP) || match_ctrl(m, k, HID_P)) {  // up
        if (edit_line + edit_offset) {
            if (edit_line)
                edit_line--;
            else
                edit_offset--;
            line_editor_set(&le, scene_text[edit_line + edit_offset]);
            r_edit_dirty |= R_ALL;
        }
    }
    else if (match_no_mod(m, k, HID_OPEN_BRACKET)) {  // [
        if (preset_select) preset_select--;
        r_edit_dirty |= R_ALL;
    }
    else if (match_no_mod(m, k, HID_CLOSE_BRACKET)) {  // ]
        if (preset_select < SCENE_SLOTS - 1) preset_select++;
        r_edit_dirty |= R_ALL;
    }
    else if (match_no_mod(m, k, HID_ENTER)) {  // enter
        strcpy(scene_text[edit_line + edit_offset], line_editor_get(&le));
        if (edit_line + edit_offset < 31) {
            if (edit_line == 5)
                edit_offset++;
            else
                edit_line++;
        }
        line_editor_set(&le, scene_text[edit_line + edit_offset]);
        r_edit_dirty |= R_ALL;
    }
    else if (match_alt(m, k, HID_ENTER)) {  // alt+enter
        if (!is_held_key) {
            strcpy(scene_text[edit_line + edit_offset], line_editor_get(&le));
            flash_write(preset_select);
            set_last_mode();
        }
    }
    else {  // pass to line editor
        bool processed = line_editor_process_keys(&le, k, m, is_held_key);
        if (processed) r_edit_dirty |= R_INPUT;
    }
}


bool screen_refresh_preset_w() {
    if (!(r_edit_dirty & R_ALL)) { return false; }

    char header[6] = ">>> ";
    itoa(preset_select, header + 4, 10);
    region_fill(&line[0], 1);
    font_string_region_clip_right(&line[0], header, 126, 0, 0xf, 1);
    font_string_region_clip(&line[0], "WRITE", 2, 0, 0xf, 1);

    for (uint8_t y = 1; y < 7; y++) {
        uint8_t a = edit_line == (y - 1);
        region_fill(&line[y], a);
        font_string_region_clip(&line[y], scene_text[edit_offset + y - 1], 2, 0,
                                0xa + a * 5, a);
    }

    line_editor_draw(&le, '+', &line[7]);

    r_edit_dirty &= ~R_ALL;
    return true;
}
