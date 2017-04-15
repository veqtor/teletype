#ifndef _FUDGE_H_
#define _FUDGE_H_

#include <stdbool.h>
#include <stdint.h>
#include "region.h"
#include "teletype.h"

// Temporary header, while we transistion modes out of main.c

extern scene_state_t scene_state;

extern region line[8];

extern uint8_t preset_select;

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
