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

#define A_METRO 0x1
#define A_TR 0x2
#define A_SLEW 0x4
#define A_DELAY 0x8
#define A_Q 0x10
#define A_X 0x20
#define A_REFRESH 0x40
#define A_MUTES 0x80
extern uint8_t activity;

#endif
