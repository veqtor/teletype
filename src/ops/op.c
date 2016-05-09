#include "op.h"

#include <stddef.h>  // offsetof

#include "helpers.h"

void op_constant(const void *data, scene_state_t *NOTUSED(ss),
                 exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, (intptr_t)data);
}

void op_peek_i16(const void *data, scene_state_t *ss, exec_state_t *NOTUSED(es),
                 command_state_t *cs) {
    char *base = (char *)ss;
    size_t offset = (size_t)data;
    int16_t *ptr = (int16_t *)(base + offset);
    cs_push(cs, *ptr);
}

void op_poke_i16(const void *data, scene_state_t *ss, exec_state_t *NOTUSED(es),
                 command_state_t *cs) {
    char *base = (char *)ss;
    size_t offset = (size_t)data;
    int16_t *ptr = (int16_t *)(base + offset);
    *ptr = cs_pop(cs);
}
