#include "ops/stack.h"

#include "helpers.h"
#include "teletype_io.h"

static void op_S_ALL_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_S_POP_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_S_CLR_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_S_L_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);


const tele_op_t op_S_ALL =
    MAKE_GET_OP(S.ALL, op_S_ALL_get, 0, false, "S: EXECUTE ALL");
const tele_op_t op_S_POP =
    MAKE_GET_OP(S.POP, op_S_POP_get, 0, false, "S: POP LAST");
const tele_op_t op_S_CLR =
    MAKE_GET_OP(S.CLR, op_S_CLR_get, 0, false, "S: FLUSH");
const tele_op_t op_S_L = MAKE_GET_OP(S.L, op_S_L_get, 0, true, "STACK LENGTH");


static void op_S_ALL_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                         exec_state_t *NOTUSED(es),
                         command_state_t *NOTUSED(cs)) {
    for (int16_t i = 0; i < tele_stack_top; i++)
        process(&tele_stack[tele_stack_top - i - 1]);
    tele_stack_top = 0;
    tele_s(0);
}

static void op_S_POP_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                         exec_state_t *NOTUSED(es),
                         command_state_t *NOTUSED(cs)) {
    if (tele_stack_top) {
        tele_stack_top--;
        process(&tele_stack[tele_stack_top]);
        if (tele_stack_top == 0) tele_s(0);
    }
}

static void op_S_CLR_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                         exec_state_t *NOTUSED(es),
                         command_state_t *NOTUSED(cs)) {
    tele_stack_top = 0;
    tele_s(0);
}

static void op_S_L_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, tele_stack_top);
}
