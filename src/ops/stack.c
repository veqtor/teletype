#include "ops/stack.h"

#include "helpers.h"
#include "teletype.h"
#include "teletype_io.h"

static void mod_S_func(scene_state_t *ss, exec_state_t *es, command_state_t *cs,
                       tele_command_t *sub_command);
static void op_S_ALL_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_S_POP_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_S_CLR_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_S_L_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);

const tele_mod_t mod_S = MAKE_MOD(S, mod_S_func, 0, "ADD COMMAND TO STACK");

const tele_op_t op_S_ALL =
    MAKE_GET_OP(S.ALL, op_S_ALL_get, 0, false, "S: EXECUTE ALL");
const tele_op_t op_S_POP =
    MAKE_GET_OP(S.POP, op_S_POP_get, 0, false, "S: POP LAST");
const tele_op_t op_S_CLR =
    MAKE_GET_OP(S.CLR, op_S_CLR_get, 0, false, "S: FLUSH");
const tele_op_t op_S_L = MAKE_GET_OP(S.L, op_S_L_get, 0, true, "STACK LENGTH");

static void mod_S_func(scene_state_t *ss, exec_state_t *NOTUSED(es),
                       command_state_t *NOTUSED(cs),
                       tele_command_t *sub_command) {
    if (ss->stack_op.top < STACK_OP_SIZE) {
        copy_command(&ss->stack_op.commands[ss->stack_op.top], sub_command);
        ss->stack_op.top++;
        if (ss->stack_op.top == 1) tele_s(1);
    }
}

static void op_S_ALL_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es),
                         command_state_t *NOTUSED(cs)) {
    for (int16_t i = 0; i < ss->stack_op.top; i++)
        process(&ss->stack_op.commands[ss->stack_op.top - i - 1]);
    ss->stack_op.top = 0;
    tele_s(0);
}

static void op_S_POP_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es),
                         command_state_t *NOTUSED(cs)) {
    if (ss->stack_op.top) {
        ss->stack_op.top--;
        process(&ss->stack_op.commands[ss->stack_op.top]);
        if (ss->stack_op.top == 0) tele_s(0);
    }
}

static void op_S_CLR_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es),
                         command_state_t *NOTUSED(cs)) {
    ss->stack_op.top = 0;
    tele_s(0);
}

static void op_S_L_get(const void *NOTUSED(data), scene_state_t *ss,
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, ss->stack_op.top);
}
