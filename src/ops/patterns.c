#include "ops/patterns.h"

#include "helpers.h"
#include "teletype.h"
#include "teletype_io.h"

static void op_P_N_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_P_N_set(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_P_L_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_P_L_set(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_P_I_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_P_I_set(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_P_HERE_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_P_HERE_set(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_P_NEXT_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_P_NEXT_set(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_P_PREV_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_P_PREV_set(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_P_WRAP_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_P_WRAP_set(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_P_START_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_P_START_set(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_P_END_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_P_END_set(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_P_get(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_P_set(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_P_INS_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_P_RM_get(const void *data, scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs);
static void op_P_PUSH_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_P_POP_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_PN_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_PN_set(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);

// clang-format off
const tele_op_t op_P       = MAKE_GET_SET_OP(P      , op_P_get      , op_P_set      , 1, true);
const tele_op_t op_P_HERE  = MAKE_GET_SET_OP(P.HERE , op_P_HERE_get , op_P_HERE_set , 0, true);
const tele_op_t op_P_END   = MAKE_GET_SET_OP(P.END  , op_P_END_get  , op_P_END_set  , 0, true);
const tele_op_t op_P_I     = MAKE_GET_SET_OP(P.I    , op_P_I_get    , op_P_I_set    , 0, true);
const tele_op_t op_P_L     = MAKE_GET_SET_OP(P.L    , op_P_L_get    , op_P_L_set    , 0, true);
const tele_op_t op_P_N     = MAKE_GET_SET_OP(P.N    , op_P_N_get    , op_P_N_set    , 0, true);
const tele_op_t op_P_NEXT  = MAKE_GET_SET_OP(P.NEXT , op_P_NEXT_get , op_P_NEXT_set , 0, true);
const tele_op_t op_P_PREV  = MAKE_GET_SET_OP(P.PREV , op_P_PREV_get , op_P_PREV_set , 0, true);
const tele_op_t op_P_START = MAKE_GET_SET_OP(P.START, op_P_START_get, op_P_START_set, 0, true);
const tele_op_t op_P_WRAP  = MAKE_GET_SET_OP(P.WRAP , op_P_WRAP_get , op_P_WRAP_set , 0, true);
const tele_op_t op_PN      = MAKE_GET_SET_OP(PN     , op_PN_get     , op_PN_set     , 2, true);

const tele_op_t op_P_INS  = MAKE_GET_OP(P.INS  , op_P_INS_get  , 2, false);
const tele_op_t op_P_RM   = MAKE_GET_OP(P.RM   , op_P_RM_get   , 1, true );
const tele_op_t op_P_PUSH = MAKE_GET_OP(P.PUSH , op_P_PUSH_get , 1, false);
const tele_op_t op_P_POP  = MAKE_GET_OP(P.POP  , op_P_POP_get  , 0, true );
// clang-format on


static void op_P_N_get(const void *NOTUSED(data), scene_state_t *ss,
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, ss->variables.p_n);
}

static void op_P_N_set(const void *NOTUSED(data), scene_state_t *ss,
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    if (a < 0)
        a = 0;
    else if (a > 3)
        a = 3;
    ss->variables.p_n = a;
}

static void op_P_L_get(const void *NOTUSED(data), scene_state_t *ss,
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    cs_push(cs, tele_get_pattern_l(pn));
}

static void op_P_L_set(const void *NOTUSED(data), scene_state_t *ss,
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    if (a < 0)
        tele_set_pattern_l(pn, 0);
    else if (a > 64)
        tele_set_pattern_l(pn, 64);
    else
        tele_set_pattern_l(pn, a);
}

static void op_P_I_get(const void *NOTUSED(data), scene_state_t *ss,
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    cs_push(cs, tele_get_pattern_i(pn));
}

static void op_P_I_set(const void *NOTUSED(data), scene_state_t *ss,
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    if (a < 0)
        tele_set_pattern_i(pn, 0);
    else if (a > tele_get_pattern_l(pn))
        tele_set_pattern_i(pn, tele_get_pattern_l(pn));
    else
        tele_set_pattern_i(pn, a);
    tele_pi();
}

static void op_P_HERE_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    cs_push(cs, tele_get_pattern_val(pn, tele_get_pattern_i(pn)));
}

static void op_P_HERE_set(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    tele_set_pattern_val(pn, tele_get_pattern_i(pn), a);
}

static void op_P_NEXT_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    if ((tele_get_pattern_i(pn) == (tele_get_pattern_l(pn) - 1)) ||
        (tele_get_pattern_i(pn) == tele_get_pattern_end(pn))) {
        if (tele_get_pattern_wrap(pn))
            tele_set_pattern_i(pn, tele_get_pattern_start(pn));
    }
    else
        tele_set_pattern_i(pn, tele_get_pattern_i(pn) + 1);

    if (tele_get_pattern_i(pn) > tele_get_pattern_l(pn))
        tele_set_pattern_i(pn, 0);

    cs_push(cs, tele_get_pattern_val(pn, tele_get_pattern_i(pn)));

    tele_pi();
}

static void op_P_NEXT_set(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    if ((tele_get_pattern_i(pn) == (tele_get_pattern_l(pn) - 1)) ||
        (tele_get_pattern_i(pn) == tele_get_pattern_end(pn))) {
        if (tele_get_pattern_wrap(pn))
            tele_set_pattern_i(pn, tele_get_pattern_start(pn));
    }
    else
        tele_set_pattern_i(pn, tele_get_pattern_i(pn) + 1);

    if (tele_get_pattern_i(pn) > tele_get_pattern_l(pn))
        tele_set_pattern_i(pn, 0);

    int16_t a = cs_pop(cs);
    tele_set_pattern_val(pn, tele_get_pattern_i(pn), a);

    tele_pi();
}

static void op_P_PREV_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    if ((tele_get_pattern_i(pn) == 0) ||
        (tele_get_pattern_i(pn) == tele_get_pattern_start(pn))) {
        if (tele_get_pattern_wrap(pn)) {
            if (tele_get_pattern_end(pn) < tele_get_pattern_l(pn))
                tele_set_pattern_i(pn, tele_get_pattern_end(pn));
            else
                tele_set_pattern_i(pn, tele_get_pattern_l(pn) - 1);
        }
    }
    else
        tele_set_pattern_i(pn, tele_get_pattern_i(pn) - 1);

    cs_push(cs, tele_get_pattern_val(pn, tele_get_pattern_i(pn)));

    tele_pi();
}

static void op_P_PREV_set(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    if ((tele_get_pattern_i(pn) == 0) ||
        (tele_get_pattern_i(pn) == tele_get_pattern_start(pn))) {
        if (tele_get_pattern_wrap(pn)) {
            if (tele_get_pattern_end(pn) < tele_get_pattern_l(pn))
                tele_set_pattern_i(pn, tele_get_pattern_end(pn));
            else
                tele_set_pattern_i(pn, tele_get_pattern_l(pn) - 1);
        }
    }
    else
        tele_set_pattern_i(pn, tele_get_pattern_i(pn) - 1);

    int16_t a = cs_pop(cs);
    tele_set_pattern_val(pn, tele_get_pattern_i(pn), a);

    tele_pi();
}

static void op_P_WRAP_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    cs_push(cs, tele_get_pattern_wrap(pn));
}

static void op_P_WRAP_set(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    if (a < 0)
        tele_set_pattern_wrap(pn, 0);
    else if (a > 1)
        tele_set_pattern_wrap(pn, 1);
    else
        tele_set_pattern_wrap(pn, a);
}

static void op_P_START_get(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    cs_push(cs, tele_get_pattern_start(pn));
}

static void op_P_START_set(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    if (a < 0)
        tele_set_pattern_start(pn, 0);
    else if (a > 63)
        tele_set_pattern_start(pn, 63);
    else
        tele_set_pattern_start(pn, a);
}

static void op_P_END_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    cs_push(cs, tele_get_pattern_end(pn));
}

static void op_P_END_set(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    if (a < 0)
        tele_set_pattern_end(pn, 0);
    else if (a > 63)
        tele_set_pattern_end(pn, 63);
    else
        tele_set_pattern_end(pn, a);
}

static void op_P_get(const void *NOTUSED(data), scene_state_t *ss,
                     exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    if (a < 0) {
        if (tele_get_pattern_l(pn) == 0)
            a = 0;
        else if (a < -tele_get_pattern_l(pn))
            a = 0;
        else
            a = tele_get_pattern_l(pn) + a;
    }
    if (a > 63) a = 63;

    cs_push(cs, tele_get_pattern_val(pn, a));
}
static void op_P_set(const void *NOTUSED(data), scene_state_t *ss,
                     exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    if (a < 0) {
        if (tele_get_pattern_l(pn) == 0)
            a = 0;
        else if (a < -tele_get_pattern_l(pn))
            a = 0;
        else
            a = tele_get_pattern_l(pn) + a;
    }
    if (a > 63) a = 63;

    tele_set_pattern_val(pn, a, b);
    tele_pi();
}

static void op_P_INS_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a, b, i;
    a = cs_pop(cs);
    b = cs_pop(cs);

    if (a < 0) {
        if (tele_get_pattern_l(pn) == 0)
            a = 0;
        else if (a < -tele_get_pattern_l(pn))
            a = 0;
        else
            a = tele_get_pattern_l(pn) + a;
    }
    if (a > 63) a = 63;

    if (tele_get_pattern_l(pn) >= a) {
        for (i = tele_get_pattern_l(pn); i > a; i--) {
            int16_t v = tele_get_pattern_val(pn, i - 1);
            tele_set_pattern_val(pn, i, v);
        }
        if (tele_get_pattern_l(pn) < 63) {
            tele_set_pattern_l(pn, tele_get_pattern_l(pn) + 1);
        }
    }

    tele_set_pattern_val(pn, a, b);
    tele_pi();
}

static void op_P_RM_get(const void *NOTUSED(data), scene_state_t *ss,
                        exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a, i;
    a = cs_pop(cs);

    if (a < 0) {
        if (tele_get_pattern_l(pn) == 0)
            a = 0;
        else if (a < -tele_get_pattern_l(pn))
            a = 0;
        else
            a = tele_get_pattern_l(pn) + a;
    }
    else if (a > tele_get_pattern_l(pn))
        a = tele_get_pattern_l(pn);

    if (tele_get_pattern_l(pn) > 0) {
        cs_push(cs, tele_get_pattern_val(pn, a));
        for (i = a; i < tele_get_pattern_l(pn); i++) {
            int16_t v = tele_get_pattern_val(pn, i + 1);
            tele_set_pattern_val(pn, i, v);
        }

        tele_set_pattern_l(pn, tele_get_pattern_l(pn) - 1);
    }
    else
        cs_push(cs, 0);
    tele_pi();
}

static void op_P_PUSH_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a;
    a = cs_pop(cs);

    if (tele_get_pattern_l(pn) < 64) {
        tele_set_pattern_val(pn, tele_get_pattern_l(pn), a);
        tele_set_pattern_l(pn, tele_get_pattern_l(pn) + 1);
        tele_pi();
    }
}

static void op_P_POP_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    if (tele_get_pattern_l(pn) > 0) {
        tele_set_pattern_l(pn, tele_get_pattern_l(pn) - 1);
        cs_push(cs, tele_get_pattern_val(pn, tele_get_pattern_l(pn)));
        tele_pi();
    }
    else
        cs_push(cs, 0);
}

static void op_PN_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);

    if (a < 0)
        a = 0;
    else if (a > 3)
        a = 3;

    if (b < 0) {
        if (tele_get_pattern_l(a) == 0)
            b = 0;
        else if (b < -tele_get_pattern_l(a))
            b = 0;
        else
            b = tele_get_pattern_l(a) + b;
    }
    if (b > 63) b = 63;

    cs_push(cs, tele_get_pattern_val(a, b));
}

static void op_PN_set(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    int16_t c = cs_pop(cs);

    if (a < 0)
        a = 0;
    else if (a > 3)
        a = 3;

    if (b < 0) {
        if (tele_get_pattern_l(a) == 0)
            b = 0;
        else if (b < -tele_get_pattern_l(a))
            b = 0;
        else
            b = tele_get_pattern_l(a) + b;
    }
    if (b > 63) b = 63;

    tele_set_pattern_val(a, b, c);
    tele_pi();
}
