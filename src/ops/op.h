#ifndef _OPS_OP_H_
#define _OPS_OP_H_

#include <stdbool.h>
#include <stddef.h>

#include "state.h"
#include "teletype.h"

typedef struct {
    const char *name;
    void (*const get)(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
    void (*const set)(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
    const uint8_t params;
    const bool returns;
    const void *data;
    const char *doc;
} tele_op_t;

typedef struct {
    const char *name;
    void (*const func)(scene_state_t *ss, exec_state_t *es, command_state_t *cs,
                       tele_command_t *sub_command);
    const uint8_t params;
    const char *doc;
} tele_mod_t;


// Get only ops
#define MAKE_GET_OP(n, g, p, r, d)                                    \
    {                                                                 \
        .name = #n, .get = g, .set = NULL, .params = p, .returns = r, \
        .data = NULL, .doc = d                                        \
    }


// Get & set ops
#define MAKE_GET_SET_OP(n, g, s, p, r, d)                          \
    {                                                              \
        .name = #n, .get = g, .set = s, .params = p, .returns = r, \
        .data = NULL, .doc = d                                     \
    }


// Constant Ops
#define MAKE_CONSTANT_OP(n, v, d)                                 \
    {                                                             \
        .name = #n, .get = op_constant, .set = NULL, .params = 0, \
        .returns = 1, .data = (void *)v, .doc = d                 \
    }

void op_constant(const void *data, scene_state_t *ss, exec_state_t *es,
                 command_state_t *cs);


// Variables, peek & poke
#define MAKE_SIMPLE_VARIABLE_OP(n, v, d)                                   \
    {                                                                      \
        .name = #n, .get = op_peek_i16, .set = op_poke_i16, .params = 0,   \
        .returns = 1, .data = (void *)offsetof(scene_state_t, v), .doc = d \
    }

void op_peek_i16(const void *data, scene_state_t *ss, exec_state_t *es,
                 command_state_t *cs);
void op_poke_i16(const void *data, scene_state_t *ss, exec_state_t *es,
                 command_state_t *cs);

// Mods
#define MAKE_MOD(n, f, p, d) \
    { .name = #n, .func = f, .params = p, .doc = d }


#endif
