#include "pattern_mode.h"

// this
#include "fudge.h"
#include "keyboard_helper.h"

// teletype
#include "teletype.h"

// libavr32
#include "font.h"
#include "region.h"
#include "util.h"

// asf
#include "conf_usb_host.h"  // needed in order to include "usb_protocol_hid.h"
#include "usb_protocol_hid.h"

int16_t copy_buffer;
uint8_t pattern;  // which pattern are we editting
uint8_t base;     // base + offset determine what we are editting
uint8_t offset;

void process_pattern_keys(uint8_t k, uint8_t m, bool is_held_key) {
    if (match_no_mod(m, k, HID_DOWN)) {  // down
        base++;
        if (base == 8) {
            base = 7;
            if (offset < 56) { offset++; }
        }
        r_edit_dirty |= R_ALL;
    }
    else if (match_alt(m, k, HID_DOWN)) {  // alt + down
        if (offset < 48)
            offset += 8;
        else {
            offset = 56;
            base = 7;
        }
        r_edit_dirty |= R_ALL;
    }
    else if (match_no_mod(m, k, HID_UP)) {  // up
        if (base)
            base--;
        else if (offset)
            offset--;
        r_edit_dirty |= R_ALL;
    }
    else if (match_alt(m, k, HID_UP)) {  // alt + up
        if (offset > 8) { offset -= 8; }
        else {
            offset = 0;
            base = 0;
        }
        r_edit_dirty |= R_ALL;
    }
    else if (match_no_mod(m, k, HID_LEFT)) {  // left
        if (pattern > 0) pattern--;
        r_edit_dirty |= R_ALL;
    }
    else if (match_alt(m, k, HID_LEFT)) {  // alt + left
        base = 0;
        offset = 0;
        r_edit_dirty |= R_ALL;
    }
    else if (match_no_mod(m, k, HID_RIGHT)) {  // right
        if (pattern < 3) pattern++;
        r_edit_dirty |= R_ALL;
    }
    else if (match_alt(m, k, HID_RIGHT)) {  // alt + right
        base = 7;
        offset = 56;
    }
    else if (match_no_mod(m, k, HID_OPEN_BRACKET)) {  // [
        int16_t v = ss_get_pattern_val(&scene_state, pattern, base + offset);
        if (v > INT16_MIN) {  // -32767
            ss_set_pattern_val(&scene_state, pattern, base + offset, v - 1);
            r_edit_dirty |= R_ALL;
        }
    }
    else if (match_no_mod(m, k, HID_CLOSE_BRACKET)) {  // ]
        int16_t v = ss_get_pattern_val(&scene_state, pattern, base + offset);
        if (v < INT16_MAX) {  // 32766
            ss_set_pattern_val(&scene_state, pattern, base + offset, v + 1);
            r_edit_dirty |= R_ALL;
        }
    }
    else if (match_no_mod(m, k, HID_BACKSPACE)) {  // backspace
        int16_t v =
            ss_get_pattern_val(&scene_state, pattern, base + offset) / 10;
        ss_set_pattern_val(&scene_state, pattern, base + offset, v);
        r_edit_dirty |= R_ALL;
    }
    else if (match_shift(m, k, HID_BACKSPACE)) {  // shift + backspace
        for (size_t i = base + offset; i < 63; i++) {
            int16_t v = ss_get_pattern_val(&scene_state, pattern, i + 1);
            ss_set_pattern_val(&scene_state, pattern, i, v);
        }

        uint16_t l = ss_get_pattern_len(&scene_state, pattern);
        if (l > base + offset) ss_set_pattern_len(&scene_state, pattern, l - 1);
        r_edit_dirty |= R_ALL;
    }
    else if (match_no_mod(m, k, HID_ENTER)) {  // enter
        uint16_t l = ss_get_pattern_len(&scene_state, pattern);
        if (base + offset == l && l < 64)
            ss_set_pattern_len(&scene_state, pattern, l + 1);
        base++;
        if (base == 8) {
            base = 7;
            if (offset < 56) { offset++; }
        }
        r_edit_dirty |= R_ALL;
    }
    else if (match_shift(m, k, HID_ENTER)) {  // shift + enter
        for (int i = 63; i > base + offset; i--) {
            int16_t v = ss_get_pattern_val(&scene_state, pattern, i - 1);
            ss_set_pattern_val(&scene_state, pattern, i, v);
        }
        uint16_t l = ss_get_pattern_len(&scene_state, pattern);
        if (l < 64) { ss_set_pattern_len(&scene_state, pattern, l + 1); }
        r_edit_dirty |= R_ALL;
    }
    else if (match_alt(m, k, HID_X)) {  // alt + x = cut
        copy_buffer = ss_get_pattern_val(&scene_state, pattern, base + offset);
        for (int i = base + offset; i < 63; i++) {
            int16_t v = ss_get_pattern_val(&scene_state, pattern, i + 1);
            ss_set_pattern_val(&scene_state, pattern, i, v);
        }

        uint16_t l = ss_get_pattern_len(&scene_state, pattern);
        if (l > base + offset) {
            ss_set_pattern_len(&scene_state, pattern, l - 1);
        }
        r_edit_dirty |= R_ALL;
    }
    else if (match_alt(m, k, HID_C)) {  // alt + c = copy
        copy_buffer = ss_get_pattern_val(&scene_state, pattern, base + offset);
    }
    else if (match_alt(m, k, HID_V)) {  // alt + v = paste
        ss_set_pattern_val(&scene_state, pattern, base + offset, copy_buffer);
        r_edit_dirty |= R_ALL;
    }
    else if (match_shift_alt(m, k, HID_V)) {  // shift + alt + v = insert paste
        for (int i = 63; i > base + offset; i--) {
            int16_t v = ss_get_pattern_val(&scene_state, pattern, i - 1);
            ss_set_pattern_val(&scene_state, pattern, i, v);
        }
        uint16_t l = ss_get_pattern_len(&scene_state, pattern);
        if (l >= base + offset && l < 63) {
            ss_set_pattern_len(&scene_state, pattern, l + 1);
        }
        ss_set_pattern_val(&scene_state, pattern, base + offset, copy_buffer);
        r_edit_dirty |= R_ALL;
    }
    else if (match_shift(m, k, HID_L)) {  // shift + l
        ss_set_pattern_len(&scene_state, pattern, base + offset + 1);
        r_edit_dirty |= R_ALL;
    }
    else if (match_alt(m, k, HID_L)) {  // alt + l
        uint16_t l = ss_get_pattern_len(&scene_state, pattern);
        if (l) {
            offset = ((l - 1) >> 3) << 3;
            base = (l - 1) & 0x7;
            int8_t delta = base - 3;
            if ((offset + delta > 0) && (offset + delta < 56)) {
                offset += delta;
                base = 3;
            }
        }
        else {
            offset = 0;
            base = 0;
        }
        r_edit_dirty |= R_ALL;
    }
    else if (match_shift(m, k, HID_S)) {  // shift + s
        ss_set_pattern_start(&scene_state, pattern, offset + base);
        r_edit_dirty |= R_ALL;
    }
    else if (match_alt(m, k, HID_S)) {  // alt + s
        int16_t start = ss_get_pattern_start(&scene_state, pattern);
        if (start) {
            offset = (start >> 3) << 3;
            base = start & 0x7;
            int8_t delta = base - 3;
            if ((offset + delta > 0) && (offset + delta < 56)) {
                offset += delta;
                base = 3;
            }
        }
        else {
            offset = 0;
            base = 0;
        }
        r_edit_dirty |= R_ALL;
    }
    else if (match_shift(m, k, HID_E)) {  // shift + e
        ss_set_pattern_end(&scene_state, pattern, offset + base);
        r_edit_dirty |= R_ALL;
    }
    else if (match_alt(m, k, HID_E)) {  // alt + e
        int16_t end = ss_get_pattern_end(&scene_state, pattern);
        if (end) {
            offset = (end >> 3) << 3;
            base = end & 0x7;
            int8_t delta = base - 3;
            if ((offset + delta > 0) && (offset + delta < 56)) {
                offset += delta;
                base = 3;
            }
        }
        else {
            offset = 0;
            base = 0;
        }
        r_edit_dirty |= R_ALL;
    }
    else if (match_no_mod(m, k, HID_UNDERSCORE)) {  // -
        int16_t v = ss_get_pattern_val(&scene_state, pattern, base + offset);
        ss_set_pattern_val(&scene_state, pattern, base + offset, -v);
        r_edit_dirty |= R_ALL;
    }
    else if (match_no_mod(m, k, HID_SPACEBAR)) {  // space
        if (ss_get_pattern_val(&scene_state, pattern, base + offset))
            ss_set_pattern_val(&scene_state, pattern, base + offset, 0);
        else
            ss_set_pattern_val(&scene_state, pattern, base + offset, 1);
        r_edit_dirty |= R_ALL;
    }
    else if ((m == HID_MODIFIER_NONE) && (k >= HID_1 && k <= HID_0)) {
        uint8_t n = (k - HID_1 + 1) % 10;  // convert HID numbers to decimal,
                                           // taking care of HID_0
        int16_t v = ss_get_pattern_val(&scene_state, pattern, base + offset);
        if (v && v < 3276 && v > -3276) {
            v = v * 10;
            if (v > 0)
                ss_set_pattern_val(&scene_state, pattern, base + offset, v + n);
            else
                ss_set_pattern_val(&scene_state, pattern, base + offset, v - n);
        }
        else
            ss_set_pattern_val(&scene_state, pattern, base + offset, n);
        r_edit_dirty |= R_ALL;
    }
}

void process_pattern_knob(uint16_t knob, uint8_t m) {
    if (m == HID_MODIFIER_LEFT_CTRL || m == HID_MODIFIER_RIGHT_CTRL) {
        if (m == HID_MODIFIER_LEFT_SHIFT || m == HID_MODIFIER_RIGHT_SHIFT) {
            ss_set_pattern_val(&scene_state, pattern, base + offset, knob >> 2);
        }
        else {
            ss_set_pattern_val(&scene_state, pattern, base + offset, knob >> 7);
        }
        r_edit_dirty |= R_ALL;
    }
}

void screen_refresh_pattern() {
    if (!(r_edit_dirty & R_ALL)) { return; }

    char s[32];
    for (uint8_t y = 0; y < 8; y++) {
        region_fill(&line[y], 0);
        itoa(y + offset, s, 10);
        font_string_region_clip_right(&line[y], s, 4, 0, 0x1, 0);

        for (uint8_t x = 0; x < 4; x++) {
            uint8_t a = 1;
            if (ss_get_pattern_len(&scene_state, x) > y + offset) a = 6;

            itoa(ss_get_pattern_val(&scene_state, x, y + offset), s, 10);
            font_string_region_clip_right(&line[y], s, (x + 1) * 30 + 4, 0, a,
                                          0);

            if (y + offset >= ss_get_pattern_start(&scene_state, x)) {
                if (y + offset <= ss_get_pattern_end(&scene_state, x)) {
                    for (uint8_t i = 0; i < 8; i += 2) {
                        line[y].data[i * 128 + (x + 1) * 30 + 6] = 1;
                    }
                }
            }

            if (y + offset == ss_get_pattern_idx(&scene_state, x)) {
                line[y].data[2 * 128 + (x + 1) * 30 + 6] = 11;
                line[y].data[3 * 128 + (x + 1) * 30 + 6] = 11;
                line[y].data[4 * 128 + (x + 1) * 30 + 6] = 11;
            }
        }
    }

    itoa(ss_get_pattern_val(&scene_state, pattern, base + offset), s, 10);
    font_string_region_clip_right(&line[base], s, (pattern + 1) * 30 + 4, 0,
                                  0xf, 0);

    for (uint8_t y = 0; y < 64; y += 2) {
        line[y >> 3].data[(y & 0x7) * 128 + 8] = 1;
    }

    for (uint8_t y = 0; y < 8; y++) {
        line[(offset + y) >> 3].data[((offset + y) & 0x7) * 128 + 8] = 6;
    }

    r_edit_dirty &= ~R_ALL;

    screen_dirty = true;
}
