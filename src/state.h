#ifndef _STATE_H_
#define _STATE_H_

#include <stdint.h>

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} scene_variables_t;

typedef struct { scene_variables_t variables; } scene_state_t;

typedef struct {
} exec_state_t;

typedef struct {
} command_state_t;

#endif
