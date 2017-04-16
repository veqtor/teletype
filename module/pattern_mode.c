#include "pattern_mode.h"

// this
#include "globals.h"
#include "keyboard_helper.h"

// teletype
#include "teletype.h"
#include "teletype_io.h"

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

bool dirty;

// teletype_io.h
void tele_pattern_updated() {
    dirty = true;
}

void set_pattern_mode() {
    dirty = true;
}

void process_pattern_keys(uint8_t k, uint8_t m, bool is_held_key) {
    // <down>: move down
    if (match_no_mod(m, k, HID_DOWN)) {
        base++;
        if (base == 8) {
            base = 7;
            if (offset < 56) { offset++; }
        }
        dirty = true;
    }
    // alt-<down>: move a page down
    else if (match_alt(m, k, HID_DOWN)) {
        if (offset < 48)
            offset += 8;
        else {
            offset = 56;
            base = 7;
        }
        dirty = true;
    }
    // <up>: move up
    else if (match_no_mod(m, k, HID_UP)) {
        if (base)
            base--;
        else if (offset)
            offset--;
        dirty = true;
    }
    // alt-<up>: move a page up
    else if (match_alt(m, k, HID_UP)) {
        if (offset > 8) { offset -= 8; }
        else {
            offset = 0;
            base = 0;
        }
        dirty = true;
    }
    // <left>: move left
    else if (match_no_mod(m, k, HID_LEFT)) {
        if (pattern > 0) pattern--;
        dirty = true;
    }
    // alt-<left>: move to the very left
    else if (match_alt(m, k, HID_LEFT)) {
        base = 0;
        offset = 0;
        dirty = true;
    }
    // <right>: move right
    else if (match_no_mod(m, k, HID_RIGHT)) {
        if (pattern < 3) pattern++;
        dirty = true;
    }
    // alt-<right>: move to the very right
    else if (match_alt(m, k, HID_RIGHT)) {
        base = 7;
        offset = 56;
    }
    // [: decrement by 1
    else if (match_no_mod(m, k, HID_OPEN_BRACKET)) {
        int16_t v = ss_get_pattern_val(&scene_state, pattern, base + offset);
        if (v > INT16_MIN) {  // -32767
            ss_set_pattern_val(&scene_state, pattern, base + offset, v - 1);
            dirty = true;
        }
    }
    // ]: increment by 1
    else if (match_no_mod(m, k, HID_CLOSE_BRACKET)) {
        int16_t v = ss_get_pattern_val(&scene_state, pattern, base + offset);
        if (v < INT16_MAX) {  // 32766
            ss_set_pattern_val(&scene_state, pattern, base + offset, v + 1);
            dirty = true;
        }
    }
    // <backspace>: delete a digit
    else if (match_no_mod(m, k, HID_BACKSPACE)) {
        int16_t v =
            ss_get_pattern_val(&scene_state, pattern, base + offset) / 10;
        ss_set_pattern_val(&scene_state, pattern, base + offset, v);
        dirty = true;
    }
    // shift-<backspace>: delete an entry, shift numbers up
    else if (match_shift(m, k, HID_BACKSPACE)) {
        for (size_t i = base + offset; i < 63; i++) {
            int16_t v = ss_get_pattern_val(&scene_state, pattern, i + 1);
            ss_set_pattern_val(&scene_state, pattern, i, v);
        }

        uint16_t l = ss_get_pattern_len(&scene_state, pattern);
        if (l > base + offset) ss_set_pattern_len(&scene_state, pattern, l - 1);
        dirty = true;
    }
    // <enter>: move down (increase length only if on the entry immediately
    // after the current length)
    else if (match_no_mod(m, k, HID_ENTER)) {
        uint16_t l = ss_get_pattern_len(&scene_state, pattern);
        if (base + offset == l && l < 64)
            ss_set_pattern_len(&scene_state, pattern, l + 1);
        base++;
        if (base == 8) {
            base = 7;
            if (offset < 56) { offset++; }
        }
        dirty = true;
    }
    // shift-<enter>: duplicate entry and shift downwards (increase length only
    // if on the entry immediately after the current length)
    else if (match_shift(m, k, HID_ENTER)) {
        for (int i = 63; i > base + offset; i--) {
            int16_t v = ss_get_pattern_val(&scene_state, pattern, i - 1);
            ss_set_pattern_val(&scene_state, pattern, i, v);
        }
        uint16_t l = ss_get_pattern_len(&scene_state, pattern);
        if (base + offset == l && l < 64) {
            ss_set_pattern_len(&scene_state, pattern, l + 1);
        }
        dirty = true;
    }
    // alt-x: cut value (n.b. ctrl-x not supported)
    else if (match_alt(m, k, HID_X)) {
        copy_buffer = ss_get_pattern_val(&scene_state, pattern, base + offset);
        for (int i = base + offset; i < 63; i++) {
            int16_t v = ss_get_pattern_val(&scene_state, pattern, i + 1);
            ss_set_pattern_val(&scene_state, pattern, i, v);
        }

        uint16_t l = ss_get_pattern_len(&scene_state, pattern);
        if (l > base + offset) {
            ss_set_pattern_len(&scene_state, pattern, l - 1);
        }
        dirty = true;
    }
    // alt-c: copy value (n.b. ctrl-c not supported)
    else if (match_alt(m, k, HID_C)) {
        copy_buffer = ss_get_pattern_val(&scene_state, pattern, base + offset);
    }
    // alt-v: paste value (n.b. ctrl-v not supported)
    else if (match_alt(m, k, HID_V)) {
        ss_set_pattern_val(&scene_state, pattern, base + offset, copy_buffer);
        dirty = true;
    }
    // shift-alt-v: insert value
    else if (match_shift_alt(m, k, HID_V)) {
        for (int i = 63; i > base + offset; i--) {
            int16_t v = ss_get_pattern_val(&scene_state, pattern, i - 1);
            ss_set_pattern_val(&scene_state, pattern, i, v);
        }
        uint16_t l = ss_get_pattern_len(&scene_state, pattern);
        if (l >= base + offset && l < 63) {
            ss_set_pattern_len(&scene_state, pattern, l + 1);
        }
        ss_set_pattern_val(&scene_state, pattern, base + offset, copy_buffer);
        dirty = true;
    }
    // shift-l: set length to current position
    else if (match_shift(m, k, HID_L)) {
        ss_set_pattern_len(&scene_state, pattern, base + offset + 1);
        dirty = true;
    }
    // alt-l: go to current length entry
    else if (match_alt(m, k, HID_L)) {
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
        dirty = true;
    }
    // shift-s: set start to current position
    else if (match_shift(m, k, HID_S)) {
        ss_set_pattern_start(&scene_state, pattern, offset + base);
        dirty = true;
    }
    // alt-s: go to start entry
    else if (match_alt(m, k, HID_S)) {
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
        dirty = true;
    }
    // shift-e: set end to current position
    else if (match_shift(m, k, HID_E)) {
        ss_set_pattern_end(&scene_state, pattern, offset + base);
        dirty = true;
    }
    // alt-e: go to end entry
    else if (match_alt(m, k, HID_E)) {
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
        dirty = true;
    }
    // -: negate value
    else if (match_no_mod(m, k, HID_UNDERSCORE)) {
        int16_t v = ss_get_pattern_val(&scene_state, pattern, base + offset);
        ss_set_pattern_val(&scene_state, pattern, base + offset, -v);
        dirty = true;
    }
    // <space>: toggle non-zero to zero, and zero to 1
    else if (match_no_mod(m, k, HID_SPACEBAR)) {
        if (ss_get_pattern_val(&scene_state, pattern, base + offset))
            ss_set_pattern_val(&scene_state, pattern, base + offset, 0);
        else
            ss_set_pattern_val(&scene_state, pattern, base + offset, 1);
        dirty = true;
    }
    // 0-9: numeric entry
    else if (no_mod(m) && k >= HID_1 && k <= HID_0) {
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
        dirty = true;
    }
}

void process_pattern_knob(uint16_t knob, uint8_t m) {
    if (mod_only_ctrl(m)) {
        ss_set_pattern_val(&scene_state, pattern, base + offset, knob >> 7);
        dirty = true;
    }
    else if (mod_only_shift_ctrl(m)) {
        ss_set_pattern_val(&scene_state, pattern, base + offset, knob >> 2);
        dirty = true;
    }
}

bool screen_refresh_pattern() {
    if (!dirty) { return false; }

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

    dirty = false;

    return true;
}
