#ifndef _STATE_H_
#define _STATE_H_

#include <stdbool.h>
#include <stdint.h>

#include "command.h"

#define STACK_SIZE 8
#define CV_COUNT 4
#define Q_LENGTH 16
#define TR_COUNT 4
#define DELAY_SIZE 8
#define STACK_OP_SIZE 8
#define PATTERN_COUNT 4
#define PATTERN_LENGTH 64

////////////////////////////////////////////////////////////////////////////////
// SCENE STATE /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    int16_t a;
    int16_t b;
    int16_t c;
    int16_t cv[CV_COUNT];
    int16_t cv_off[CV_COUNT];
    int16_t cv_slew[CV_COUNT];
    int16_t d;
    int16_t drunk;
    int16_t drunk_max;
    int16_t drunk_min;
    int16_t drunk_wrap;
    int16_t flip;
    int16_t i;
    int16_t in;
    int16_t m;
    int16_t m_act;
    int16_t o;
    int16_t o_inc;
    int16_t o_min;
    int16_t o_max;
    int16_t o_wrap;
    int16_t p_n;
    int16_t param;
    int16_t q[Q_LENGTH];
    int16_t q_n;
    int16_t scene;
    int16_t t;
    int16_t time;
    int16_t time_act;
    int16_t tr[TR_COUNT];
    int16_t tr_pol[TR_COUNT];
    int16_t tr_time[TR_COUNT];
    int16_t x;
    int16_t y;
    int16_t z;
} scene_variables_t;

typedef struct {
    int16_t i;
    uint16_t l;
    uint16_t wrap;
    int16_t start;
    int16_t end;
    int16_t v[PATTERN_LENGTH];
} scene_pattern_t;

typedef struct {
    tele_command_t commands[DELAY_SIZE];
    int16_t time[DELAY_SIZE];
    uint8_t count;
} scene_delay_t;

typedef struct {
    tele_command_t commands[STACK_OP_SIZE];
    uint8_t top;
} scene_stack_op_t;

typedef struct {
    scene_variables_t variables;
    scene_pattern_t patterns[PATTERN_COUNT];
    scene_delay_t delay;
    scene_stack_op_t stack_op;
    int16_t tr_pulse_timer[TR_COUNT];
} scene_state_t;

////////////////////////////////////////////////////////////////////////////////
// EXEC STATE //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct { bool if_else_condition; } exec_state_t;

////////////////////////////////////////////////////////////////////////////////
// COMMAND STATE ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    int16_t values[STACK_SIZE];
    int16_t top;
} command_state_stack_t;

typedef struct { command_state_stack_t stack; } command_state_t;

extern void cs_init(command_state_t *cs);
extern int16_t cs_stack_size(command_state_t *cs);

// by declaring the following static inline, each compilation unit (i.e. C
// file), gets its own copy of the function
static inline int16_t cs_pop(command_state_t *cs) {
    cs->stack.top--;
    return cs->stack.values[cs->stack.top];
}

static inline void cs_push(command_state_t *cs, int16_t data) {
    cs->stack.values[cs->stack.top] = data;
    cs->stack.top++;
}


#endif
