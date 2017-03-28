#include "edit_mode.h"

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
uint8_t edit;
error_t status;
char error_msg[TELE_ERROR_MSG_LENGTH];

void set_edit_mode() {
    status = E_OK;
    error_msg[0] = 0;
    edit_line = 0;
    print_command(tele_get_script_c(edit, edit_line), input);
    pos = strlen(input);
    for (int n = pos; n < 32; n++) input[n] = 0;
}

void process_edit_keys(uint8_t key, uint8_t mod_key, bool is_held_key) {
    uint8_t mod_SH = mod_key & SHIFT;
    uint8_t mod_ALT = mod_key & ALT;

    switch (key) {
        case 0x51:  // down
            if (edit_line < (SCRIPT_MAX_COMMANDS - 1)) {
                if (tele_get_script_l(edit) > edit_line) {
                    edit_line++;
                    print_command(tele_get_script_c(edit, edit_line), input);
                    pos = strlen(input);
                    for (size_t n = pos; n < 32; n++) input[n] = 0;
                    r_edit_dirty |= R_LIST;
                }
            }
            break;
        case 0x52:  // up
            if (edit_line) {
                edit_line--;
                print_command(tele_get_script_c(edit, edit_line), input);

                pos = strlen(input);
                for (size_t n = pos; n < 32; n++) input[n] = 0;
                r_edit_dirty |= R_LIST;
            }
            break;

        case 0x50:  // back
            if (pos) { pos--; }
            break;

        case 0x4f:  // forward
            if (pos < strlen(input)) { pos++; }
            break;

        case 0x30:  // ]
            status = E_OK;
            error_msg[0] = 0;
            edit++;
            if (edit == 10) edit = 0;
            if (edit_line > tele_get_script_l(edit))
                edit_line = tele_get_script_l(edit);
            print_command(tele_get_script_c(edit, edit_line), input);
            pos = strlen(input);
            for (size_t n = pos; n < 32; n++) input[n] = 0;


            r_edit_dirty |= R_LIST;
            break;

        case 0x2F:  // [
            status = E_OK;
            error_msg[0] = 0;
            if (edit)
                edit--;
            else
                edit = 9;
            if (edit_line > tele_get_script_l(edit))
                edit_line = tele_get_script_l(edit);
            print_command(tele_get_script_c(edit, edit_line), input);
            pos = strlen(input);
            for (size_t n = pos; n < 32; n++) input[n] = 0;
            r_edit_dirty |= R_LIST;
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
                    {
                        if (temp.length == 0) {  // BLANK LINE
                            uint8_t script_len = tele_get_script_l(edit);
                            if (script_len &&
                                tele_get_script_c(edit, edit_line)->length) {
                                script_len--;
                                tele_set_script_l(edit, script_len);

                                for (size_t n = edit_line; n < script_len;
                                     n++) {
                                    const tele_command_t* cmd =
                                        tele_get_script_c(edit, n + 1);
                                    tele_set_script_c(edit, n, cmd);
                                }

                                tele_command_t blank_command;
                                blank_command.length = 0;
                                tele_set_script_c(edit, script_len,
                                                  &blank_command);

                                if (edit_line > script_len)
                                    edit_line = script_len;
                                print_command(
                                    tele_get_script_c(edit, edit_line), input);
                                pos = strlen(input);
                            }
                        }
                        else if (mod_SH) {  // SHIFT = INSERT
                            for (size_t n = tele_get_script_l(edit);
                                 n > edit_line; n--) {
                                const tele_command_t* cmd =
                                    tele_get_script_c(edit, n - 1);
                                tele_set_script_c(edit, n, cmd);
                            }

                            if (tele_get_script_l(edit) < SCRIPT_MAX_COMMANDS) {
                                tele_set_script_l(edit,
                                                  tele_get_script_l(edit) + 1);
                            }

                            tele_set_script_c(edit, edit_line, &temp);
                            if ((edit_line == tele_get_script_l(edit)) &&
                                (tele_get_script_l(edit) < 4)) {
                                tele_set_script_l(edit,
                                                  tele_get_script_l(edit) + 1);
                            }
                            if (edit_line < (SCRIPT_MAX_COMMANDS - 1)) {
                                edit_line++;
                                print_command(
                                    tele_get_script_c(edit, edit_line), input);
                                pos = strlen(input);
                                for (size_t n = pos; n < 32; n++) {
                                    input[n] = 0;
                                }
                            }
                        }
                        else {
                            tele_set_script_c(edit, edit_line, &temp);
                            if ((edit_line == tele_get_script_l(edit)) &&
                                (tele_get_script_l(edit) <
                                 SCRIPT_MAX_COMMANDS)) {
                                tele_set_script_l(edit,
                                                  tele_get_script_l(edit) + 1);
                            }
                            if (edit_line < (SCRIPT_MAX_COMMANDS - 1)) {
                                edit_line++;
                                print_command(
                                    tele_get_script_c(edit, edit_line), input);
                                pos = strlen(input);
                                for (size_t n = pos; n < 32; n++) {
                                    input[n] = 0;
                                }
                            }
                        }

                        r_edit_dirty |= R_MESSAGE;
                    }
                    r_edit_dirty |= R_LIST;
                }
            }
            r_edit_dirty |= R_MESSAGE;
            break;
        }

        default:
            if (mod_ALT) {          // ALT
                if (key == 0x1b) {  // x CUT
                    memcpy(&input_buffer, &input, sizeof(input));
                    if (tele_get_script_l(edit)) {
                        tele_set_script_l(edit, tele_get_script_l(edit) - 1);
                        for (size_t n = edit_line; n < tele_get_script_l(edit);
                             n++) {
                            tele_set_script_c(edit, n,
                                              tele_get_script_c(edit, n + 1));
                        }

                        tele_command_t blank_command;
                        blank_command.length = 0;
                        tele_set_script_c(edit, tele_get_script_l(edit),
                                          &blank_command);
                        if (edit_line > tele_get_script_l(edit)) {
                            edit_line = tele_get_script_l(edit);
                        }
                        print_command(tele_get_script_c(edit, edit_line),
                                      input);
                        pos = strlen(input);
                    }

                    r_edit_dirty |= R_LIST;
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


void screen_refresh_edit() {
    if (r_edit_dirty & R_INPUT) {
        char s[32];

        if (edit == 8)
            s[0] = 'M';
        else if (edit == 9)
            s[0] = 'I';
        else
            s[0] = edit + 49;
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
        else {
            s[0] = 0;
        }

        region_fill(&line[6], 0);
        font_string_region_clip(&line[6], s, 0, 0, 0x4, 0);

        screen_dirty = true;
        r_edit_dirty &= ~R_MESSAGE;
    }

    if (r_edit_dirty & R_LIST) {
        for (int i = 0; i < 6; i++) {
            uint8_t a = edit_line == i;
            region_fill(&line[i], a);
            if (tele_get_script_l(edit) > i) {
                char s[32];
                print_command(tele_get_script_c(edit, i), s);
                region_string(&line[i], s, 2, 0, 0xf, a, 0);
            }
        }

        screen_dirty = true;
        r_edit_dirty &= ~R_LIST;
    }
}
