#include "help_mode.h"

// this
#include "fudge.h"
#include "keyboard_helper.h"

// libavr32
#include "font.h"
#include "region.h"

// asf
#include "conf_usb_host.h"  // needed in order to include "usb_protocol_hid.h"
#include "usb_protocol_hid.h"

////////////////////////////////////////////////////////////////////////////////
// Help text ///////////////////////////////////////////////////////////////////

#define HELP_PAGES 8

#define HELP1_LENGTH 38
const char* help1[HELP1_LENGTH] = { "1/8 HELP",
                                    "[ ] NAVIGATE HELP PAGES",
                                    "UP/DOWN TO SCROLL",
                                    " ",
                                    "TAB|EDIT/LIVE",
                                    "TILDE|TRACKER",
                                    "ESC|SCENE",
                                    "ALT-ESC|WRITE",
                                    " ",
                                    "ALT-X|CUT",
                                    "ALT-C|COPY",
                                    "ALT-V|PASTE",
                                    " ",
                                    "// LIVE",
                                    "ENTER|EXECUTE",
                                    "UP|PREVIOUS",
                                    "SH-BSP|CLEAR",
                                    " ",
                                    "// EDIT",
                                    "[ ]|PREV, NEXT SCRIPT",
                                    "ENTER|ADD/OVERWRITE",
                                    "SH-ENTER|INSERT",
                                    "SH-BSP|CLEAR",
                                    " ",
                                    "// TRACKER",
                                    "ARROWS|NAVIGATE",
                                    "ALT-ARROWS|JUMP",
                                    "0-9|NUMERIC ENTRY",
                                    "-|FLIP SIGN",
                                    "SPACE|TOGGLE 0/1",
                                    "[ ]|NUDGE UP, DOWN",
                                    "SH-ALT-V|INSERT PASTE",
                                    "SH-BSP|DELETE",
                                    "SH-ENTER|DUPE INSERT",
                                    "SH-L|SET LENGTH",
                                    "SH-S|SET START",
                                    "SH-E|SET END",
                                    "ALT-L,S,E|JUMP" };

#define HELP2_LENGTH 13
const char* help2[HELP2_LENGTH] = {
    "2/8 VARIABLES",           " ",
    "X, Y, Z|GENERAL PURPOSE", "T|USE FOR TIME",
    "A-D|ASSIGNED TO 1-4",     " ",
    "// SPECIAL VARIABLES",    "I|USED BY LOOP",
    "O|INCREMENTS ON READ",    "DRUNK|INC BY -1, 0, +1",
    "Q|SHIFT REGISTER",        "Q.N|SET Q LENGTH",
    "Q.AVG|AVERAGE OF ALL Q"
};

#define HELP3_LENGTH 20
const char* help3[HELP3_LENGTH] = { "3/8 PARAMETERS",
                                    " ",
                                    "TR A-D|SET TR VALUE (0,1)",
                                    "TR.TIME A-D|TR PULSE TIME",
                                    "CV 1-4|CV TARGET VALUE",
                                    "CV.SLEW 1-4|CV SLEW TIME (MS)",
                                    "CV.SET 1-4|SET CV (NO SLEW)",
                                    "CV.OFF 1-4|ADD CV OFFSET",
                                    " ",
                                    "IN|GET IN JACK VAL",
                                    "PARAM|GET KNOB VAL",
                                    " ",
                                    "M|METRO TIME (MS)",
                                    "M.ACT|ENABLE METRO (0/1)",
                                    "M.RESET|HARD RESET TIMER",
                                    " ",
                                    "TIME|TIMER COUNT (MS)",
                                    "TIME.ACT|ENABLE TIMER (0/1)",
                                    " ",
                                    "SCENE|GET/SET SCENE #" };

#define HELP4_LENGTH 9
const char* help4[HELP4_LENGTH] = { "4/8 DATA AND TABLES",
                                    " ",
                                    "ALL PARAMS HAVE 16B RANGE",
                                    "-32768 TO 32767",
                                    " ",
                                    "// LOOKUP TABLES",
                                    "N 0-127|CONVERT TO 1V/8VE",
                                    "V 0-10|VOLT LOOKUP",
                                    "VV 0-1000|V WITH 2 DECIMALS" };

#define HELP5_LENGTH 35
const char* help5[HELP5_LENGTH] = { "5/8 OPERATORS",
                                    " ",
                                    "RAND A|RANDOM 0 - A",
                                    "RRAND A B|RANDOM A - B",
                                    "TOSS|RANDOM 0 OR 1",
                                    " ",
                                    "AVG A B|AVERAGE OF INS",
                                    "MIN A B|LESSER OF INS",
                                    "MAX A B|GREATER OF INS",
                                    " ",
                                    "ADD A B|A + B",
                                    "SUB A B|A - B",
                                    "MUL A B|A * B",
                                    "DIV A B|A / B",
                                    "MOD A B|REMAINDER OF A / B",
                                    " ",
                                    "// LOGIC COMPARISONS",
                                    "RETURNS 1 (TRUE) OR 0 (FALSE)",
                                    "EQ A B|A EQUALS B",
                                    "NE A B|A NOT EQUAL TO B",
                                    "GT A B|A GREATER THAN B",
                                    "LT A B|A LESS THAN B",
                                    "EZ A|A EQUALS 0",
                                    "NZ A|A NOT EQUAL TO 0",
                                    " ",
                                    "RSH A B|BITSHIFT A RIGHT B",
                                    "LSH A B|BITSHIFT A LEFT B",
                                    " ",
                                    "LIM A B C|CLAMP A WITHIN B-C",
                                    "WRAP A B C|WRAP A AROUND B-C",
                                    "QT A B|QUANTIZE A TO B*X",
                                    " ",
                                    "// SPECIAL OPERATORS",
                                    "TR.TOG X|FLIP STATE OF TR X",
                                    "TR.PULSE X|PULSE TR X" };

#define HELP6_LENGTH 22
const char* help6[HELP6_LENGTH] = { "6/8 PRE :",
                                    " ",
                                    "EACH PRE NEEDS A : FOLLOWED",
                                    "BY A COMMAND TO OPERATE ON",
                                    " ",
                                    "PROB A :|PROBABALITY 0-100",
                                    " ",
                                    "DEL A :|DELAY BY A (MS)",
                                    "DEL.CLR|KILL ALL DELAYS",
                                    " ",
                                    "S :|PUSH ONTO STACK",
                                    "S.CLR|CLEAR S",
                                    "S.ALL|EXECUTE ALL S",
                                    "S.POP|EXECUTE NEWEST S",
                                    "S.L|QUERY LENGTH OF S",
                                    " ",
                                    "IF A :|EXECUTE IF NON-ZERO",
                                    "ELIF A :|AFTER IF WHEN TRUE",
                                    "ELSE :|AFTER FAILED IF",
                                    " ",
                                    "L A B :|ITERATE FROM A-B",
                                    "NB: I IS UPDATED EACH TIME" };

#define HELP7_LENGTH 26
const char* help7[HELP7_LENGTH] = { "7/8 PATTERNS",
                                    " ",
                                    "// DIRECT ACCESS",
                                    "P A|GET VAL AT INDEX A",
                                    "P A B|SET VAL AT A TO B",
                                    "P.N A|SELECT BANK A",
                                    "PN A B|GET BANK A, IDX B",
                                    "PN A B C|BANK A, IDX B TO C",
                                    " ",
                                    "// CHANGES LENGTH",
                                    "P.INS A B|INSERT B AT IDX A",
                                    "P.RM A|DELETE ENTRY AT A",
                                    "P.PUSH A|ADD A TO END",
                                    "P.POP|RETURN & REM LAST",
                                    " ",
                                    "// ACTIVE RANGE",
                                    "P.L A|GET/SET LENGTH",
                                    "P.WRAP A|ENABLE WRAPPING",
                                    "P.START A|GET/SET START",
                                    "P.END A|GET/SET END",
                                    " ",
                                    "// READ HEAD",
                                    "P.I A|GET/SET POSITION",
                                    "P.HERE A|GET/SET VAL AT P.I",
                                    "P.NEXT A|GET/SET NEXT POS",
                                    "P.PREV A|GET/SET PREV POS" };

#define HELP8_LENGTH 42
const char* help8[HELP8_LENGTH] = { "8/8 REMOTE",
                                    " ",
                                    "REMOTE CONTROL OF MONOME",
                                    "TRILOGY MODULES OVER II",
                                    " ",
                                    "ALL MESSAGES NEED A VALUE",
                                    " ",
                                    "// WHITE WHALE",
                                    "WW.PRESET|RECALL PRESET",
                                    "WW.POS|CUT TO POSITION",
                                    "WW.SYNC|CUT & HARD SYNC",
                                    "WW.START|SET LOOP START",
                                    "WW.END|SET LOOP END",
                                    "WW.PMODE|SET PLAY MODE",
                                    "WW.PATTERN|  CHANGE PATTERN",
                                    "WW.QPATTERN|  QUEUED P CHANGE",
                                    "WW.MUTE1|MUTE TR 1",
                                    "WW.MUTE2|MUTE TR 2",
                                    "WW.MUTE3|MUTE TR 3",
                                    "WW.MUTE4|MUTE TR 4",
                                    "WW.MUTEA|MUTE CV A",
                                    "WW.MUTEB|MUTE CV B",
                                    " ",
                                    "// MEADOWPHYSICS",
                                    "MP.PRESET|RECALL PRESET",
                                    "MP.RESET|RESET POSITIONS",
                                    "MP.SYNC|RESET POS AND SYNC",
                                    "MP.MUTE|MUTES OUTPUT 1-8",
                                    "MP.UNMUTE|UNMUTES OUTPUT",
                                    "MP.FREEZE|FREEZE ROW 1-8",
                                    "MP.UNFREEZE|  UNFREEZE ROW",
                                    " ",
                                    "// EARTHSEA",
                                    "ES.PRESET|RECALL PRESET",
                                    "ES.MODE|SET PATTERN CLOCK",
                                    "ES.CLOCK|NEXT PATT EVENT",
                                    "ES.RESET|PLAY PATTERN",
                                    "ES.PATTERN|SET PATTERN #",
                                    "ES.TRANS|SET TRANSPOSE",
                                    "ES.STOP|STOP PATTERN",
                                    "ES.TRIPLE|RECALL SHAPE 1-4",
                                    "ES.MAGIC|1=HALF, 2=DOUBLE" };


////////////////////////////////////////////////////////////////////////////////
// Help mode ///////////////////////////////////////////////////////////////////

const char** help_pages[HELP_PAGES] = { help1, help2, help3, help4,
                                        help5, help6, help7, help8 };
const uint8_t help_length[HELP_PAGES] = { HELP1_LENGTH, HELP2_LENGTH,
                                          HELP3_LENGTH, HELP4_LENGTH,
                                          HELP5_LENGTH, HELP6_LENGTH,
                                          HELP7_LENGTH, HELP8_LENGTH };

uint8_t page_no;
uint8_t offset;


void process_help_keys(uint8_t k, uint8_t m, bool is_held_key) {
    if (match_no_mod(m, k, HID_DOWN) || match_ctrl(m, k, HID_N)) {  // down
        if (offset < help_length[page_no] - 8) {
            offset++;
            r_edit_dirty |= R_ALL;
        }
    }
    else if (match_no_mod(m, k, HID_UP) || match_ctrl(m, k, HID_P)) {  // up
        if (offset) {
            offset--;
            r_edit_dirty |= R_ALL;
        }
    }
    else if (match_no_mod(m, k, HID_LEFT) ||
             match_no_mod(m, k, HID_OPEN_BRACKET)) {  // left or [
        if (page_no) {
            offset = 0;
            page_no--;
            r_edit_dirty |= R_ALL;
        }
    }
    else if (match_no_mod(m, k, HID_RIGHT) ||
             match_no_mod(m, k, HID_CLOSE_BRACKET)) {  // right or ]
        if (page_no < HELP_PAGES - 1) {
            offset = 0;
            page_no++;
            r_edit_dirty |= R_ALL;
        }
    }
}

bool screen_refresh_help() {
    if (!(r_edit_dirty & R_ALL)) { return false; }

    // clamp value of page_no
    if (page_no >= HELP_PAGES) page_no = HELP_PAGES - 1;

    // clamp value of offset
    if (offset >= help_length[page_no] - 8) offset = help_length[page_no] - 8;

    const char** text = help_pages[page_no];

    for (uint8_t y = 0; y < 8; y++) {
        region_fill(&line[y], 0);
        font_string_region_clip_tab(&line[y], text[y + offset], 2, 0, 0xa, 0);
    }

    r_edit_dirty &= ~R_ALL;
    return true;
};
