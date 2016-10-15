#include "ops/metronome.h"

#include "helpers.h"
#include "teletype_io.h"

static void op_M_get(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_M_set(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_M_ACT_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_M_ACT_set(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_M_RESET_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);

const tele_op_t op_M = MAKE_GET_SET_OP(M, op_M_get, op_M_set, 0, true);

static void op_M_get(const void *NOTUSED(data), scene_state_t *ss,
                     exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, ss->variables.m);
}

static void op_M_set(const void *NOTUSED(data), scene_state_t *ss,
                     exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t m = cs_pop(cs);
    if (m < 10) m = 10;
    ss->variables.m = m;
    tele_metro(m, ss->variables.m_act, 0);
}

const tele_op_t op_M_ACT =
    MAKE_GET_SET_OP(M.ACT, op_M_ACT_get, op_M_ACT_set, 0, true);


static void op_M_ACT_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, ss->variables.m_act);
}

static void op_M_ACT_set(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t m_act = cs_pop(cs);
    if (m_act != 0) m_act = 1;
    ss->variables.m_act = m_act;
    tele_metro(ss->variables.m, m_act, 0);
}

const tele_op_t op_M_RESET = MAKE_GET_OP(M.RESET, op_M_RESET_get, 0, false);

static void op_M_RESET_get(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es),
                           command_state_t *NOTUSED(cs)) {
    tele_metro(ss->variables.m, ss->variables.m_act, 1);
}
