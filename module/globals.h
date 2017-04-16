#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdbool.h>
#include <stdint.h>
#include "region.h"
#include "teletype.h"

// global variables (defined in main.c)

// holds the current scene
extern scene_state_t scene_state;

// the current preset
extern uint8_t preset_select;

// holds screen data
extern region line[8];

// mode handling
typedef enum {
    M_LIVE,
    M_EDIT,
    M_PATTERN,
    M_PRESET_W,
    M_PRESET_R,
    M_HELP
} tele_mode_t;

void set_mode(tele_mode_t mode);
void set_last_mode(void);

#endif
