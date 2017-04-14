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

line_editor_t le;
uint8_t line_no;
uint8_t script;
error_t status;
char error_msg[TELE_ERROR_MSG_LENGTH];

void set_edit_mode() {
    status = E_OK;
    error_msg[0] = 0;
    line_no = 0;
    line_editor_set_command(
        &le, ss_get_script_command(&scene_state, script, line_no));
}

void set_edit_mode_script(uint8_t new_script) {
    script = new_script;
    if (script >= SCRIPT_COUNT) script = SCRIPT_COUNT - 1;
}

void process_edit_keys(uint8_t k, uint8_t m, bool is_held_key) {
    if (match_no_mod(m, k, HID_DOWN) || match_ctrl(m, k, HID_N)) {
        if (line_no < (SCRIPT_MAX_COMMANDS - 1) &&
            line_no < ss_get_script_len(&scene_state, script)) {
            line_no++;
            line_editor_set_command(
                &le, ss_get_script_command(&scene_state, script, line_no));
            r_edit_dirty |= R_LIST;
            r_edit_dirty |= R_INPUT;
        }
    }
    else if (match_no_mod(m, k, HID_UP) || match_ctrl(m, k, HID_P)) {
        if (line_no) {
            line_no--;
            line_editor_set_command(
                &le, ss_get_script_command(&scene_state, script, line_no));
            r_edit_dirty |= R_LIST;
            r_edit_dirty |= R_INPUT;
        }
    }
    else if (match_no_mod(m, k, HID_OPEN_BRACKET)) {
        status = E_OK;
        error_msg[0] = 0;
        if (script)
            script--;
        else
            script = SCRIPT_COUNT - 1;
        if (line_no > ss_get_script_len(&scene_state, script))
            line_no = ss_get_script_len(&scene_state, script);
        line_editor_set_command(
            &le, ss_get_script_command(&scene_state, script, line_no));
        r_edit_dirty |= R_LIST;
        r_edit_dirty |= R_INPUT;
    }
    else if (match_no_mod(m, k, HID_CLOSE_BRACKET)) {
        status = E_OK;
        error_msg[0] = 0;
        script++;
        if (script >= SCRIPT_COUNT) script = 0;
        if (line_no > ss_get_script_len(&scene_state, script))
            line_no = ss_get_script_len(&scene_state, script);
        line_editor_set_command(
            &le, ss_get_script_command(&scene_state, script, line_no));
        r_edit_dirty |= R_LIST;
        r_edit_dirty |= R_INPUT;
    }
    else if (match_alt(m, k, HID_X)) {  // override line editors cut
        line_editor_set_copy_buffer(line_editor_get(&le));
        ss_delete_script_command(&scene_state, script, line_no);
        if (line_no > ss_get_script_len(&scene_state, script)) {
            line_no = ss_get_script_len(&scene_state, script);
        }
        line_editor_set_command(
            &le, ss_get_script_command(&scene_state, script, line_no));

        r_edit_dirty |= R_LIST;
        r_edit_dirty |= R_INPUT;
    }
    else if (match_no_mod(m, k, HID_ENTER)) {
        r_edit_dirty |= R_MESSAGE;  // something will happen

        tele_command_t command;
        status = parse(line_editor_get(&le), &command, error_msg);

        if (status != E_OK)
            return;  // quit, screen_refresh_edit will display the error message

        status = validate(&command, error_msg);
        if (status != E_OK)
            return;  // quit, screen_refresh_edit will display the error message

        if (command.length == 0) {  // blank line, delete the command
            ss_delete_script_command(&scene_state, script, line_no);
            if (line_no > ss_get_script_len(&scene_state, script)) {
                line_no = ss_get_script_len(&scene_state, script);
            }
        }
        else {
            ss_overwrite_script_command(&scene_state, script, line_no,
                                        &command);
            if (line_no < SCRIPT_MAX_COMMANDS - 1) { line_no++; }
        }
        line_editor_set_command(
            &le, ss_get_script_command(&scene_state, script, line_no));
        r_edit_dirty |= R_LIST;
        r_edit_dirty |= R_INPUT;
    }
    else if (match_shift(m, k, HID_ENTER)) {
        r_edit_dirty |= R_MESSAGE;  // something will happen

        tele_command_t command;
        status = parse(line_editor_get(&le), &command, error_msg);

        if (status != E_OK)
            return;  // quit, screen_refresh_edit will display the error message

        status = validate(&command, error_msg);
        if (status != E_OK)
            return;  // quit, screen_refresh_edit will display the error message

        if (command.length > 0) {
            ss_insert_script_command(&scene_state, script, line_no, &command);
            if (line_no < (SCRIPT_MAX_COMMANDS - 1)) { line_no++; }
        }

        line_editor_set_command(
            &le, ss_get_script_command(&scene_state, script, line_no));
        r_edit_dirty |= R_LIST;
        r_edit_dirty |= R_INPUT;
    }
    else {  // pass the key though to the line editor
        bool processed = line_editor_process_keys(&le, k, m, is_held_key);
        if (processed) r_edit_dirty |= R_INPUT;
    }
}


void screen_refresh_edit() {
    if (r_edit_dirty & R_INPUT) {
        char prefix = script + '1';
        if (script == METRO_SCRIPT)
            prefix = 'M';
        else if (script == INIT_SCRIPT)
            prefix = 'I';

        line_editor_draw(&le, prefix, &line[7]);
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
            uint8_t a = line_no == i;
            region_fill(&line[i], a);
            if (ss_get_script_len(&scene_state, script) > i) {
                char s[32];
                print_command(ss_get_script_command(&scene_state, script, i),
                              s);
                region_string(&line[i], s, 2, 0, 0xf, a, 0);
            }
        }

        screen_dirty = true;
        r_edit_dirty &= ~R_LIST;
    }
}
