#include "ops/maths.h"

#include <stdlib.h>  // rand

#include "euclidean/euclidean.h"
#include "helpers.h"
#include "table.h"

static void op_ADD_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_SUB_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_MUL_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_DIV_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_MOD_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_RAND_get(const void *data, scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs);
static void op_RRAND_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_TOSS_get(const void *data, scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs);
static void op_MIN_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_MAX_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_LIM_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_WRAP_get(const void *data, scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs);
static void op_QT_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_AVG_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_EQ_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_NE_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_LT_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_GT_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_NZ_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_EZ_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_RSH_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_LSH_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_EXP_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_ABS_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_AND_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_OR_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_XOR_get(const void *data, scene_state_t *ss, exec_state_t *es,
                       command_state_t *cs);
static void op_JI_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_SCALE_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_N_get(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_V_get(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_VV_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_ER_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);


// clang-format off
const tele_op_t op_ADD   = MAKE_GET_OP(ADD     , op_ADD_get     , 2, true , "[A B] ADD A TO B"                          );
const tele_op_t op_SUB   = MAKE_GET_OP(SUB     , op_SUB_get     , 2, true , "[A B] SUBTRACT B FROM A"                   );
const tele_op_t op_MUL   = MAKE_GET_OP(MUL     , op_MUL_get     , 2, true , "[A B] MULTIPLY TWO VALUES"                 );
const tele_op_t op_DIV   = MAKE_GET_OP(DIV     , op_DIV_get     , 2, true , "[A B] DIVIDE FIRST BY SECOND"              );
const tele_op_t op_MOD   = MAKE_GET_OP(MOD     , op_MOD_get     , 2, true , "[A B] MOD FIRST BY SECOND"                 );
const tele_op_t op_RAND  = MAKE_GET_OP(RAND    , op_RAND_get    , 1, true , "[A] RETURN RANDOM NUMBER UP TO A"          );
const tele_op_t op_RRAND = MAKE_GET_OP(RRAND   , op_RRAND_get   , 2, true , "[A B] RETURN RANDOM NUMBER BETWEEN A AND B");
const tele_op_t op_TOSS  = MAKE_GET_OP(TOSS    , op_TOSS_get    , 0, true , "RETURN RANDOM STATE"                       );
const tele_op_t op_MIN   = MAKE_GET_OP(MIN     , op_MIN_get     , 2, true , "RETURN LESSER OF TWO VALUES"               );
const tele_op_t op_MAX   = MAKE_GET_OP(MAX     , op_MAX_get     , 2, true , "RETURN GREATER OF TWO VALUES"              );
const tele_op_t op_LIM   = MAKE_GET_OP(LIM     , op_LIM_get     , 3, true , "[A B C] LIMIT C TO RANGE A TO B"           );
const tele_op_t op_WRAP  = MAKE_GET_OP(WRAP    , op_WRAP_get    , 3, true , "[A B C] WRAP C WITHIN RANGE A TO B"        );
const tele_op_t op_QT    = MAKE_GET_OP(QT      , op_QT_get      , 2, true , "[A B] QUANTIZE A TO STEP SIZE B"           );
const tele_op_t op_AVG   = MAKE_GET_OP(AVG     , op_AVG_get     , 2, true , "AVERAGE TWO VALUES"                        );
const tele_op_t op_EQ    = MAKE_GET_OP(EQ      , op_EQ_get      , 2, true , "LOGIC: EQUAL"                              );
const tele_op_t op_NE    = MAKE_GET_OP(NE      , op_NE_get      , 2, true , "LOGIC: NOT EQUAL"                          );
const tele_op_t op_LT    = MAKE_GET_OP(LT      , op_LT_get      , 2, true , "LOGIC: LESS THAN"                          );
const tele_op_t op_GT    = MAKE_GET_OP(GT      , op_GT_get      , 2, true , "LOGIC: GREATER THAN"                       );
const tele_op_t op_NZ    = MAKE_GET_OP(NZ      , op_NZ_get      , 1, true , "LOGIC: NOT ZERO"                           );
const tele_op_t op_EZ    = MAKE_GET_OP(EZ      , op_EZ_get      , 1, true , "LOGIC: EQUALS ZERO"                        );
const tele_op_t op_RSH   = MAKE_GET_OP(RSH     , op_RSH_get     , 2, true , "RIGHT SHIFT"                               );
const tele_op_t op_LSH   = MAKE_GET_OP(LSH     , op_LSH_get     , 2, true , "LEFT SHIFT"                                );
const tele_op_t op_EXP   = MAKE_GET_OP(EXP     , op_EXP_get     , 1, true , "EXPONENTIATE"                              );
const tele_op_t op_ABS   = MAKE_GET_OP(ABS     , op_ABS_get     , 1, true , "ABSOLUTE VALUE"                            );
const tele_op_t op_AND   = MAKE_GET_OP(AND     , op_AND_get     , 2, true , "LOGIC: AND"                                );
const tele_op_t op_OR    = MAKE_GET_OP(OR      , op_OR_get      , 2, true , "LOGIC: OR"                                 );
const tele_op_t op_XOR   = MAKE_GET_OP(XOR     , op_XOR_get     , 2, true , "LOGIC: XOR"                                );
const tele_op_t op_JI    = MAKE_GET_OP(JI      , op_JI_get      , 2, true , "JUST INTONE DIVISON"                       );
const tele_op_t op_SCALE = MAKE_GET_OP(SCALE   , op_SCALE_get   , 5, true , "SCALE NUMBER RANGES"                       );
const tele_op_t op_N     = MAKE_GET_OP(N       , op_N_get       , 1, true , "TABLE FOR NOTE VALUES"                     );
const tele_op_t op_V     = MAKE_GET_OP(V       , op_V_get       , 1, true , "TO VOLT"                                   );
const tele_op_t op_VV    = MAKE_GET_OP(VV      , op_VV_get      , 1, true , "TO VOLT WITH PRECISION"                    );
const tele_op_t op_ER    = MAKE_GET_OP(ER      , op_ER_get      , 3, true , "EUCLIDEAN RHYTHMS"                         );
// clang-format on


static void op_ADD_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) + cs_pop(cs));
}

static void op_SUB_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) - cs_pop(cs));
}

static void op_MUL_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) * cs_pop(cs));
}

static void op_DIV_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) / cs_pop(cs));
}

static void op_MOD_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) % cs_pop(cs));
}

static void op_RAND_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                        exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    if (a == -1)
        cs_push(cs, 0);
    else
        cs_push(cs, rand() % (a + 1));
}

static void op_RRAND_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a, b, min, max, range;
    a = cs_pop(cs);
    b = cs_pop(cs);
    if (a < b) {
        min = a;
        max = b;
    }
    else {
        min = b;
        max = a;
    }
    range = max - min + 1;
    if (range == 0)
        cs_push(cs, a);
    else
        cs_push(cs, rand() % range + min);
}

static void op_TOSS_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                        exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, rand() & 1);
}

static void op_MIN_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a, b;
    a = cs_pop(cs);
    b = cs_pop(cs);
    if (b > a)
        cs_push(cs, a);
    else
        cs_push(cs, b);
}

static void op_MAX_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a, b;
    a = cs_pop(cs);
    b = cs_pop(cs);
    if (a > b)
        cs_push(cs, a);
    else
        cs_push(cs, b);
}

static void op_LIM_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a, b, i;
    i = cs_pop(cs);
    a = cs_pop(cs);
    b = cs_pop(cs);
    if (i < a)
        cs_push(cs, a);
    else if (i > b)
        cs_push(cs, b);
    else
        cs_push(cs, i);
}

static void op_WRAP_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                        exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a, b, i, c;
    i = cs_pop(cs);
    a = cs_pop(cs);
    b = cs_pop(cs);
    if (a < b) {
        c = b - a + 1;
        while (i >= b) i -= c;
        while (i < a) i += c;
    }
    else {
        c = a - b + 1;
        while (i >= a) i -= c;
        while (i < b) i += c;
    }
    cs_push(cs, i);
}

static void op_QT_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    // this rounds negative numbers rather than quantize (choose closer)
    int16_t a, b, c, d, e;
    b = cs_pop(cs);
    a = cs_pop(cs);

    c = b / a;
    d = c * a;
    e = (c + 1) * a;

    if (abs(b - d) < abs(b - e))
        cs_push(cs, d);
    else
        cs_push(cs, e);
}

static void op_AVG_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, (cs_pop(cs) + cs_pop(cs)) >> 1);
}

static void op_EQ_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) == cs_pop(cs));
}

static void op_NE_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) != cs_pop(cs));
}

static void op_LT_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) < cs_pop(cs));
}

static void op_GT_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) > cs_pop(cs));
}

static void op_NZ_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) != 0);
}

static void op_EZ_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) == 0);
}

static void op_RSH_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) >> cs_pop(cs));
}

static void op_LSH_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) << cs_pop(cs));
}

static void op_EXP_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    if (a > 16383)
        a = 16383;
    else if (a < -16383)
        a = -16383;

    a = a >> 6;

    if (a < 0) {
        a = -a;
        cs_push(cs, -table_exp[a]);
    }
    else
        cs_push(cs, table_exp[a]);
}

static void op_ABS_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);

    if (a < 0)
        cs_push(cs, -a);
    else
        cs_push(cs, a);
}

static void op_AND_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) & cs_pop(cs));
}

static void op_OR_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) | cs_pop(cs));
}

static void op_XOR_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                       exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, cs_pop(cs) ^ cs_pop(cs));
}

static void op_JI_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint32_t ji = (((cs_pop(cs) << 8) / cs_pop(cs)) * 1684) >> 8;
    while (ji > 1683) ji >>= 1;
    cs_push(cs, ji);
}

static void op_SCALE_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a, b, x, y, i;
    a = cs_pop(cs);
    b = cs_pop(cs);
    x = cs_pop(cs);
    y = cs_pop(cs);
    i = cs_pop(cs);

    cs_push(cs, (i - a) * (y - x) / (b - a) + x);
}

static void op_N_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                     exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);

    if (a < 0) {
        if (a < -127) a = -127;
        a = -a;
        cs_push(cs, -table_n[a]);
    }
    else {
        if (a > 127) a = 127;
        cs_push(cs, table_n[a]);
    }
}

static void op_V_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                     exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    if (a > 10)
        a = 10;
    else if (a < -10)
        a = -10;

    if (a < 0) {
        a = -a;
        cs_push(cs, -table_v[a]);
    }
    else
        cs_push(cs, table_v[a]);
}

static void op_VV_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint8_t negative = 1;
    int16_t a = cs_pop(cs);
    if (a < 0) {
        negative = -1;
        a = -a;
    }
    if (a > 1000) a = 1000;

    cs_push(cs, negative * (table_v[a / 100] + table_vv[a % 100]));
}

static void op_ER_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t fill = cs_pop(cs);
    int16_t len = cs_pop(cs);
    int16_t step = cs_pop(cs);
    cs_push(cs, euclidean(fill, len, step));
}
