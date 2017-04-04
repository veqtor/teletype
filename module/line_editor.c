#include "line_editor.h"

#include <string.h>

// this
#include "keyboard_helper.h"

// teletype
#include "teletype.h"

// libavr32
#include "font.h"
#include "kbd.h"
#include "region.h"

// asf
#include "conf_usb_host.h"  // needed in order to include "usb_protocol_hid.h"
#include "usb_protocol_hid.h"

// global copy buffer
static char copy_buffer[LINE_EDITOR_SIZE];

void line_editor_set(line_editor_t *le, const char value[LINE_EDITOR_SIZE]) {
    size_t length = strlen(value);
    if (length < LINE_EDITOR_SIZE) {
        strcpy(le->buffer, value);
        le->cursor = length;
        le->length = length;
    }
    else {
        le->buffer[0] = 0;
        le->cursor = 0;
        le->length = 0;
    }
}

void line_editor_set_command(line_editor_t *le, const tele_command_t *command) {
    print_command(command, le->buffer);
    le->length = strlen(le->buffer);
    le->cursor = le->length;
}

char *line_editor_get(line_editor_t *le) {
    return le->buffer;
}

bool line_editor_process_keys(line_editor_t *le, uint8_t k, uint8_t m,
                              bool is_key_held) {
    if (match_no_mod(m, k, HID_LEFT)) {  // left
        if (le->cursor) { le->cursor--; }
        return true;
    }
    else if (match_no_mod(m, k, HID_RIGHT)) {  // right
        if (le->cursor < le->length) { le->cursor++; }
        return true;
    }
    else if (match_no_mod(m, k, HID_BACKSPACE)) {  // backspace
        if (le->cursor) {
            le->cursor--;
            for (size_t x = le->cursor; x < LINE_EDITOR_SIZE - 1; x++) {
                le->buffer[x] = le->buffer[x + 1];
            }
            le->length--;
        }
        return true;
    }
    else if (match_shift(m, k, HID_BACKSPACE)) {  // backspace
        line_editor_set(le, "");
        return true;
    }
    else if (match_alt(m, k, HID_X)) {  // alt + x
        strcpy(copy_buffer, le->buffer);
        line_editor_set(le, "");
        return true;
    }
    else if (match_alt(m, k, HID_C)) {  // alt + c
        strcpy(copy_buffer, le->buffer);
        return true;
    }
    else if (match_alt(m, k, HID_V)) {  // alt + v
        line_editor_set(le, copy_buffer);
        return true;
    }
    else if (no_mod(m) || mod_only_shift(m)) {
        if (le->length < LINE_EDITOR_SIZE - 2) {  // room for another char & 0
            uint8_t n = hid_to_ascii(k, m);
            if (n) {
                for (size_t x = LINE_EDITOR_SIZE - 1; x > le->cursor; x--) {
                    le->buffer[x] = le->buffer[x - 1];  // shuffle forwards
                }

                le->buffer[le->cursor] = n;
                le->cursor++;
                le->length++;
                return true;
            }
        }
    }

    // did't process a key
    return false;
}

void line_editor_draw(line_editor_t *le, char prefix, region *reg) {
    // LINE_EDITOR_SIZE includes space for null, need to also include space for
    // the prefix, the space after the prefix and a space at the very end
    char s[LINE_EDITOR_SIZE + 3] = { prefix, ' ', 0 };
    strcat(s, le->buffer);
    strcat(s, " ");

    region_fill(reg, 0);
    font_string_region_clip_hi(reg, s, 0, 0, 0xf, 0, le->cursor + 2);
}

void line_editor_set_copy_buffer(const char *value) {
    strcpy(copy_buffer, value);
}
