#ifndef _STATE_H_
#define _STATE_H_

#include <stdint.h>

typedef struct {
    int16_t a;
    int16_t b;
    int16_t c;
    int16_t d;
    int16_t drunk;
    int16_t drunk_max;
    int16_t drunk_min;
    int16_t drunk_wrap;
    int16_t i;
    int16_t o;
    int16_t o_inc;
    int16_t o_min;
    int16_t o_max;
    int16_t o_wrap;
    int16_t t;
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
