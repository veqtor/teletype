#include "live_mode.h"

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

uint8_t pos;
char input[32];
char input_buffer[32];
uint8_t edit_line;
scene_script_t history;
uint8_t edit;

void set_live_mode() {
    for (int n = 0; n < 32; n++) input[n] = 0;
    pos = 0;
    edit_line = SCRIPT_MAX_COMMANDS;
    activity |= A_REFRESH;
}

void process_live_keys(uint8_t key, uint8_t mod_key, bool is_held_key) {
    uint8_t mod_SH = mod_key & SHIFT;
    uint8_t mod_ALT = mod_key & ALT;

    switch (key) {
        case 0x51:  // down
            if (edit_line < (SCRIPT_MAX_COMMANDS - 1)) {
                edit_line++;
                print_command(&history.c[edit_line], input);
                pos = strlen(input);
                for (size_t n = pos; n < 32; n++) input[n] = 0;
            }
            else {
                edit_line = SCRIPT_MAX_COMMANDS;
                pos = 0;
                for (size_t n = 0; n < 32; n++) input[n] = 0;
            }
            break;
        case 0x52:  // up
            if (edit_line) {
                edit_line--;
                print_command(&history.c[edit_line], input);

                pos = strlen(input);
                for (size_t n = pos; n < 32; n++) input[n] = 0;
            }
            break;

        case 0x50:  // back
            if (pos) { pos--; }
            break;

        case 0x4f:  // forward
            if (pos < strlen(input)) { pos++; }
            break;

        case BACKSPACE:
            if (mod_SH) {
                for (size_t n = 0; n < 32; n++) input[n] = 0;
                pos = 0;
            }
            else if (pos) {
                pos--;
                // input[pos] = ' ';
                for (int x = pos; x < 31; x++) input[x] = input[x + 1];
            }
            break;

        case RETURN: {
            tele_command_t temp;
            status = parse(input, &temp, error_msg);

            if (status == E_OK) {
                status = validate(&temp, error_msg);

                if (status == E_OK) {
                    edit_line = SCRIPT_MAX_COMMANDS;

                    if (temp.length) {
                        memcpy(&history.c[0], &history.c[1],
                               sizeof(tele_command_t));
                        memcpy(&history.c[1], &history.c[2],
                               sizeof(tele_command_t));
                        memcpy(&history.c[2], &history.c[3],
                               sizeof(tele_command_t));
                        memcpy(&history.c[3], &history.c[4],
                               sizeof(tele_command_t));
                        memcpy(&history.c[4], &history.c[5],
                               sizeof(tele_command_t));
                        memcpy(&history.c[5], &temp, sizeof(tele_command_t));

                        process_result_t o = run_command(&temp);
                        if (o.has_value) {
                            output = o.value;
                            output_new++;
                        }
                    }

                    for (size_t n = 0; n < 32; n++) input[n] = 0;
                    pos = 0;
                }
            }
            r_edit_dirty |= R_MESSAGE;
            break;
        }

        default:
            if (mod_ALT) {          // ALT
                if (key == 0x1b) {  // x CUT
                    memcpy(&input_buffer, &input, sizeof(input));
                    for (size_t n = 0; n < 32; n++) input[n] = 0;
                    pos = 0;
                }
                else if (key == 0x06) {  // c COPY
                    memcpy(&input_buffer, &input, sizeof(input));
                }
                else if (key == 0x19) {  // v PASTE
                    memcpy(&input, &input_buffer, sizeof(input));
                    pos = strlen(input);
                }
            }
            else {  /// NORMAL TEXT ENTRY
                if (pos < 29) {
                    u8 n = hid_to_ascii(key, mod_key);
                    if (n) {
                        for (int x = 31; x > pos; x--) input[x] = input[x - 1];

                        input[pos] = n;
                        pos++;
                    }
                }
            }

            break;
    }

    r_edit_dirty |= R_INPUT;
}


void screen_refresh_live() {
    if (r_edit_dirty & R_INPUT) {
        char s[32];

        s[0] = '>';
        s[1] = ' ';
        s[2] = 0;

        strcat(s, input);
        strcat(s, " ");

        region_fill(&line[7], 0);
        font_string_region_clip_hi(&line[7], s, 0, 0, 0xf, 0, pos + 2);

        screen_dirty = true;
        r_edit_dirty &= ~R_INPUT;
    }

    if (r_edit_dirty & R_MESSAGE) {
        char s[32];
        if (status != E_OK) {
            strcpy(s, tele_error(status));
            if (error_msg[0]) {
                strcat(s, ": ");
                strcat(s, error_msg);
                error_msg[0] = 0;
            }
            status = E_OK;
        }
        else if (output_new) {
            output_new = 0;
            itoa(output, s, 10);
        }
        else {
            s[0] = 0;
        }

        region_fill(&line[6], 0);
        font_string_region_clip(&line[6], s, 0, 0, 0x4, 0);

        screen_dirty = true;
        r_edit_dirty &= ~R_MESSAGE;
    }

    if (r_edit_dirty & R_LIST) {
        for (int i = 0; i < 6; i++) region_fill(&line[i], 0);

        screen_dirty = true;
        r_edit_dirty &= ~R_LIST;
    }

    if ((activity != activity_prev)) {
        region_fill(&line[0], 0);

        // slew icon
        uint8_t slew_fg = activity & A_SLEW ? 15 : 1;
        line[0].data[98 + 0 + 512] = slew_fg;
        line[0].data[98 + 1 + 384] = slew_fg;
        line[0].data[98 + 2 + 256] = slew_fg;
        line[0].data[98 + 3 + 128] = slew_fg;
        line[0].data[98 + 4 + 0] = slew_fg;

        // delay icon
        uint8_t delay_fg = activity & A_DELAY ? 15 : 1;
        line[0].data[106 + 0 + 0] = delay_fg;
        line[0].data[106 + 1 + 0] = delay_fg;
        line[0].data[106 + 2 + 0] = delay_fg;
        line[0].data[106 + 3 + 0] = delay_fg;
        line[0].data[106 + 4 + 0] = delay_fg;
        line[0].data[106 + 0 + 128] = delay_fg;
        line[0].data[106 + 0 + 256] = delay_fg;
        line[0].data[106 + 0 + 384] = delay_fg;
        line[0].data[106 + 0 + 512] = delay_fg;
        line[0].data[106 + 4 + 128] = delay_fg;
        line[0].data[106 + 4 + 256] = delay_fg;
        line[0].data[106 + 4 + 384] = delay_fg;
        line[0].data[106 + 4 + 512] = delay_fg;

        // queue icon
        uint8_t queue_fg = activity & A_Q ? 15 : 1;
        line[0].data[114 + 0 + 0] = queue_fg;
        line[0].data[114 + 1 + 0] = queue_fg;
        line[0].data[114 + 2 + 0] = queue_fg;
        line[0].data[114 + 3 + 0] = queue_fg;
        line[0].data[114 + 4 + 0] = queue_fg;
        line[0].data[114 + 0 + 256] = queue_fg;
        line[0].data[114 + 1 + 256] = queue_fg;
        line[0].data[114 + 2 + 256] = queue_fg;
        line[0].data[114 + 3 + 256] = queue_fg;
        line[0].data[114 + 4 + 256] = queue_fg;
        line[0].data[114 + 0 + 512] = queue_fg;
        line[0].data[114 + 1 + 512] = queue_fg;
        line[0].data[114 + 2 + 512] = queue_fg;
        line[0].data[114 + 3 + 512] = queue_fg;
        line[0].data[114 + 4 + 512] = queue_fg;

        // metro icon
        uint8_t metro_fg = activity & A_METRO ? 15 : 1;
        line[0].data[122 + 0 + 0] = metro_fg;
        line[0].data[122 + 0 + 128] = metro_fg;
        line[0].data[122 + 0 + 256] = metro_fg;
        line[0].data[122 + 0 + 384] = metro_fg;
        line[0].data[122 + 0 + 512] = metro_fg;
        line[0].data[122 + 1 + 128] = metro_fg;
        line[0].data[122 + 2 + 256] = metro_fg;
        line[0].data[122 + 3 + 128] = metro_fg;
        line[0].data[122 + 4 + 0] = metro_fg;
        line[0].data[122 + 4 + 128] = metro_fg;
        line[0].data[122 + 4 + 256] = metro_fg;
        line[0].data[122 + 4 + 384] = metro_fg;
        line[0].data[122 + 4 + 512] = metro_fg;

        // mutes
        line[0].data[87 + 0 + 128] = 15 - mutes[0] * 12;
        line[0].data[87 + 1 + 384] = 15 - mutes[1] * 12;
        line[0].data[87 + 2 + 128] = 15 - mutes[2] * 12;
        line[0].data[87 + 3 + 384] = 15 - mutes[3] * 12;
        line[0].data[87 + 4 + 128] = 15 - mutes[4] * 12;
        line[0].data[87 + 5 + 384] = 15 - mutes[5] * 12;
        line[0].data[87 + 6 + 128] = 15 - mutes[6] * 12;
        line[0].data[87 + 7 + 384] = 15 - mutes[7] * 12;

        activity_prev = activity;
        screen_dirty = true;
        activity &= ~A_MUTES;
        activity &= ~A_REFRESH;
    }
}
