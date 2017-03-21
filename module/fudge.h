#ifndef _FUDGE_H_
#define _FUDGE_H_

#include <stdbool.h>
#include <stdint.h>
#include "region.h"

// Temporary header, while we transistion modes out of main.c

#define R_PRESET (1 << 0)
#define R_INPUT (1 << 1)
#define R_MESSAGE (1 << 2)
#define R_LIST (1 << 3)
#define R_ALL 0xf
extern uint8_t r_edit_dirty;

extern region line[8];
extern bool screen_dirty;

#endif
