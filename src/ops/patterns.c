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
const tele_op_t op_P       = MAKE_GET_SET_OP(P      , op_P_get      , op_P_set      , 1, true, "PATTERN: GET/SET"  );
const tele_op_t op_P_HERE  = MAKE_GET_SET_OP(P.HERE , op_P_HERE_get , op_P_HERE_set , 0, true, "P.HERE"            );
const tele_op_t op_P_END   = MAKE_GET_SET_OP(P.END  , op_P_END_get  , op_P_END_set  , 0, true, "P.END"             );
const tele_op_t op_P_I     = MAKE_GET_SET_OP(P.I    , op_P_I_get    , op_P_I_set    , 0, true, "P.I"               );
const tele_op_t op_P_L     = MAKE_GET_SET_OP(P.L    , op_P_L_get    , op_P_L_set    , 0, true, "P.L"               );
const tele_op_t op_P_N     = MAKE_GET_SET_OP(P.N    , op_P_N_get    , op_P_N_set    , 0, true, "P.N"               );
const tele_op_t op_P_NEXT  = MAKE_GET_SET_OP(P.NEXT , op_P_NEXT_get , op_P_NEXT_set , 0, true, "P.NEXT"            );
const tele_op_t op_P_PREV  = MAKE_GET_SET_OP(P.PREV , op_P_PREV_get , op_P_PREV_set , 0, true, "P.PREV"            );
const tele_op_t op_P_START = MAKE_GET_SET_OP(P.START, op_P_START_get, op_P_START_set, 0, true, "P.START"           );
const tele_op_t op_P_WRAP  = MAKE_GET_SET_OP(P.WRAP , op_P_WRAP_get , op_P_WRAP_set , 0, true, "P.WRAP"            );
const tele_op_t op_PN      = MAKE_GET_SET_OP(PN     , op_PN_get     , op_PN_set     , 2, true, "PATTERN: GET/SET N");

const tele_op_t op_P_INS  = MAKE_GET_OP(P.INS  , op_P_INS_get  , 2, false, "PATTERN: INSERT");
const tele_op_t op_P_RM   = MAKE_GET_OP(P.RM   , op_P_RM_get   , 1, true , "PATTERN: REMOVE");
const tele_op_t op_P_PUSH = MAKE_GET_OP(P.PUSH , op_P_PUSH_get , 1, false, "PATTERN: PUSH"  );
const tele_op_t op_P_POP  = MAKE_GET_OP(P.POP  , op_P_POP_get  , 0, true , "PATTERN: POP"   );
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
    cs_push(cs, tele_patterns[pn].l);
}

static void op_P_L_set(const void *NOTUSED(data), scene_state_t *ss,
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    if (a < 0)
        tele_patterns[pn].l = 0;
    else if (a > 63)
        tele_patterns[pn].l = 63;
    else
        tele_patterns[pn].l = a;
}

static void op_P_I_get(const void *NOTUSED(data), scene_state_t *ss,
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    cs_push(cs, tele_patterns[pn].i);
}

static void op_P_I_set(const void *NOTUSED(data), scene_state_t *ss,
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    if (a < 0)
        tele_patterns[pn].i = 0;
    else if (a > tele_patterns[pn].l)
        tele_patterns[pn].i = tele_patterns[pn].l;
    else
        tele_patterns[pn].i = a;
    tele_pi();
}

static void op_P_HERE_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    cs_push(cs, tele_patterns[pn].v[tele_patterns[pn].i]);
}

static void op_P_HERE_set(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    tele_patterns[pn].v[tele_patterns[pn].i] = a;
}

static void op_P_NEXT_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    if ((tele_patterns[pn].i == (tele_patterns[pn].l - 1)) ||
        (tele_patterns[pn].i == tele_patterns[pn].end)) {
        if (tele_patterns[pn].wrap)
            tele_patterns[pn].i = tele_patterns[pn].start;
    }
    else
        tele_patterns[pn].i++;

    if (tele_patterns[pn].i > tele_patterns[pn].l) tele_patterns[pn].i = 0;

    cs_push(cs, tele_patterns[pn].v[tele_patterns[pn].i]);

    tele_pi();
}

static void op_P_NEXT_set(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    if ((tele_patterns[pn].i == (tele_patterns[pn].l - 1)) ||
        (tele_patterns[pn].i == tele_patterns[pn].end)) {
        if (tele_patterns[pn].wrap)
            tele_patterns[pn].i = tele_patterns[pn].start;
    }
    else
        tele_patterns[pn].i++;

    if (tele_patterns[pn].i > tele_patterns[pn].l) tele_patterns[pn].i = 0;

    int16_t a = cs_pop(cs);
    tele_patterns[pn].v[tele_patterns[pn].i] = a;

    tele_pi();
}

static void op_P_PREV_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    if ((tele_patterns[pn].i == 0) ||
        (tele_patterns[pn].i == tele_patterns[pn].start)) {
        if (tele_patterns[pn].wrap) {
            if (tele_patterns[pn].end < tele_patterns[pn].l)
                tele_patterns[pn].i = tele_patterns[pn].end;
            else
                tele_patterns[pn].i = tele_patterns[pn].l - 1;
        }
    }
    else
        tele_patterns[pn].i--;

    cs_push(cs, tele_patterns[pn].v[tele_patterns[pn].i]);

    tele_pi();
}

static void op_P_PREV_set(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    if ((tele_patterns[pn].i == 0) ||
        (tele_patterns[pn].i == tele_patterns[pn].start)) {
        if (tele_patterns[pn].wrap) {
            if (tele_patterns[pn].end < tele_patterns[pn].l)
                tele_patterns[pn].i = tele_patterns[pn].end;
            else
                tele_patterns[pn].i = tele_patterns[pn].l - 1;
        }
    }
    else
        tele_patterns[pn].i--;

    int16_t a = cs_pop(cs);
    tele_patterns[pn].v[tele_patterns[pn].i] = a;

    tele_pi();
}

static void op_P_WRAP_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    cs_push(cs, tele_patterns[pn].wrap);
}

static void op_P_WRAP_set(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    if (a < 0)
        tele_patterns[pn].wrap = 0;
    else if (a > 1)
        tele_patterns[pn].wrap = 1;
    else
        tele_patterns[pn].wrap = a;
}

static void op_P_START_get(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    cs_push(cs, tele_patterns[pn].start);
}

static void op_P_START_set(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    if (a < 0)
        tele_patterns[pn].start = 0;
    else if (a > 63)
        tele_patterns[pn].start = 63;
    else
        tele_patterns[pn].start = a;
}

static void op_P_END_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    cs_push(cs, tele_patterns[pn].end);
}

static void op_P_END_set(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    if (a < 0)
        tele_patterns[pn].end = 0;
    else if (a > 63)
        tele_patterns[pn].end = 63;
    else
        tele_patterns[pn].end = a;
}

static void op_P_get(const void *NOTUSED(data), scene_state_t *ss,
                     exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    if (a < 0) {
        if (tele_patterns[pn].l == 0)
            a = 0;
        else if (a < -tele_patterns[pn].l)
            a = 0;
        else
            a = tele_patterns[pn].l + a;
    }
    if (a > 63) a = 63;

    cs_push(cs, tele_patterns[pn].v[a]);
}
static void op_P_set(const void *NOTUSED(data), scene_state_t *ss,
                     exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    if (a < 0) {
        if (tele_patterns[pn].l == 0)
            a = 0;
        else if (a < -tele_patterns[pn].l)
            a = 0;
        else
            a = tele_patterns[pn].l + a;
    }
    if (a > 63) a = 63;

    tele_patterns[pn].v[a] = b;
    tele_pi();
}

static void op_P_INS_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a, b, i;
    a = cs_pop(cs);
    b = cs_pop(cs);

    if (a < 0) {
        if (tele_patterns[pn].l == 0)
            a = 0;
        else if (a < -tele_patterns[pn].l)
            a = 0;
        else
            a = tele_patterns[pn].l + a;
    }
    if (a > 63) a = 63;

    if (tele_patterns[pn].l >= a) {
        for (i = tele_patterns[pn].l; i > a; i--)
            tele_patterns[pn].v[i] = tele_patterns[pn].v[i - 1];
        if (tele_patterns[pn].l < 63) tele_patterns[pn].l++;
    }

    tele_patterns[pn].v[a] = b;
    tele_pi();
}

static void op_P_RM_get(const void *NOTUSED(data), scene_state_t *ss,
                        exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    int16_t a, i;
    a = cs_pop(cs);

    if (a < 0) {
        if (tele_patterns[pn].l == 0)
            a = 0;
        else if (a < -tele_patterns[pn].l)
            a = 0;
        else
            a = tele_patterns[pn].l + a;
    }
    else if (a > tele_patterns[pn].l)
        a = tele_patterns[pn].l;

    if (tele_patterns[pn].l > 0) {
        cs_push(cs, tele_patterns[pn].v[a]);
        for (i = a; i < tele_patterns[pn].l; i++)
            tele_patterns[pn].v[i] = tele_patterns[pn].v[i + 1];

        tele_patterns[pn].l--;
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

    if (tele_patterns[pn].l < 64) {
        tele_patterns[pn].v[tele_patterns[pn].l] = a;
        tele_patterns[pn].l++;
        tele_pi();
    }
}

static void op_P_POP_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t pn = ss->variables.p_n;
    if (tele_patterns[pn].l > 0) {
        tele_patterns[pn].l--;
        cs_push(cs, tele_patterns[pn].v[tele_patterns[pn].l]);
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
        if (tele_patterns[a].l == 0)
            b = 0;
        else if (b < -tele_patterns[a].l)
            b = 0;
        else
            b = tele_patterns[a].l + b;
    }
    if (b > 63) b = 63;

    cs_push(cs, tele_patterns[a].v[b]);
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
        if (tele_patterns[a].l == 0)
            b = 0;
        else if (b < -tele_patterns[a].l)
            b = 0;
        else
            b = tele_patterns[a].l + b;
    }
    if (b > 63) b = 63;

    tele_patterns[a].v[b] = c;
    tele_pi();
}
