#include "ops/hardware.h"

#include "helpers.h"
#include "teletype.h"
#include "teletype_io.h"

static void op_CV_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_CV_set(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_CV_SLEW_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_CV_SLEW_set(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_CV_OFF_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_CV_OFF_set(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_TR_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_TR_set(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_TR_POL_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_TR_POL_set(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_TR_TIME_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_TR_TIME_set(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_TR_TOG_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_TR_PULSE_get(const void *data, scene_state_t *ss,
                            exec_state_t *es, command_state_t *cs);
static void op_II_get(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
static void op_CV_SET_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_MUTE_get(const void *data, scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs);
static void op_UNMUTE_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_STATE_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);


// clang-format off
const tele_op_t op_CV       = MAKE_GET_SET_OP(CV     , op_CV_get     , op_CV_set     , 1, true, "CV"     );
const tele_op_t op_CV_OFF   = MAKE_GET_SET_OP(CV.OFF , op_CV_OFF_get , op_CV_OFF_set , 1, true, "CV.OFF" );
const tele_op_t op_CV_SLEW  = MAKE_GET_SET_OP(CV.SLEW, op_CV_SLEW_get, op_CV_SLEW_set, 1, true, "CV.SLEW");
const tele_op_t op_TR       = MAKE_GET_SET_OP(TR     , op_TR_get     , op_TR_set     , 1, true, "TR"     );
const tele_op_t op_TR_POL   = MAKE_GET_SET_OP(TR.POL , op_TR_POL_get , op_TR_POL_set , 1, true, "TR.POL" );
const tele_op_t op_TR_TIME  = MAKE_GET_SET_OP(TR.TIME, op_TR_TIME_get, op_TR_TIME_set, 1, true, "TR.TIME");
const tele_op_t op_TR_TOG   = MAKE_GET_OP(TR.TOG  , op_TR_TOG_get  , 1, false, "[A] TOGGLE TRIGGER A");
const tele_op_t op_TR_PULSE = MAKE_GET_OP(TR.PULSE, op_TR_PULSE_get, 1, false, "PULSE TRIGGER"       );
const tele_op_t op_II       = MAKE_GET_OP(II      , op_II_get      , 2, false, "II"                  );
const tele_op_t op_CV_SET   = MAKE_GET_OP(CV.SET  , op_CV_SET_get  , 2, false, "CV SET"              );
const tele_op_t op_MUTE     = MAKE_GET_OP(MUTE    , op_MUTE_get    , 1, false, "MUTE INPUT"          );
const tele_op_t op_UNMUTE   = MAKE_GET_OP(UNMUTE  , op_UNMUTE_get  , 1, false, "UNMUTE INPUT"        );
const tele_op_t op_STATE    = MAKE_GET_OP(STATE   , op_STATE_get   , 1, true , "GET INPUT STATE"     );
// clang-format on


static void op_CV_get(const void *NOTUSED(data), scene_state_t *ss,
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    a = normalise_value(0, CV_COUNT - 1, 0, a - 1);
    cs_push(cs, ss->variables.cv[a]);
}

static void op_CV_set(const void *NOTUSED(data), scene_state_t *ss,
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    a = normalise_value(0, CV_COUNT - 1, 0, a - 1);
    b = normalise_value(0, 16383, 0, b);
    ss->variables.cv[a] = b;
    tele_cv(a, b, 1);
}

static void op_CV_SLEW_get(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    a = normalise_value(0, CV_COUNT - 1, 0, a - 1);
    cs_push(cs, ss->variables.cv_slew[a]);
}

static void op_CV_SLEW_set(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    a = normalise_value(0, CV_COUNT - 1, 0, a - 1);
    b = normalise_value(1, 32767, 0, b);  // min slew = 1
    ss->variables.cv_slew[a] = b;
    tele_cv_slew(a, b);
}

static void op_CV_OFF_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    a = normalise_value(0, CV_COUNT - 1, 0, a - 1);
    cs_push(cs, ss->variables.cv_off[a]);
}

static void op_CV_OFF_set(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    a = normalise_value(0, CV_COUNT - 1, 0, a - 1);
    ss->variables.cv_off[a] = b;
    tele_cv_off(a, b);
    tele_cv(a, ss->variables.cv[a], 1);
}

static void op_TR_get(const void *NOTUSED(data), scene_state_t *ss,
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    a = normalise_value(0, TR_COUNT - 1, 0, a - 1);
    cs_push(cs, ss->variables.tr[a]);
}

static void op_TR_set(const void *NOTUSED(data), scene_state_t *ss,
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    a = normalise_value(0, TR_COUNT - 1, 0, a - 1);
    ss->variables.tr[a] = b != 0;
    tele_tr(a, b);
}

static void op_TR_POL_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    a = normalise_value(0, TR_COUNT - 1, 0, a - 1);
    cs_push(cs, ss->variables.tr_pol[a]);
}

static void op_TR_POL_set(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    a = normalise_value(0, TR_COUNT - 1, 0, a - 1);
    ss->variables.tr_pol[a] = b > 0;
}

static void op_TR_TIME_get(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    a = normalise_value(0, TR_COUNT - 1, 0, a - 1);
    cs_push(cs, ss->variables.tr_time[a]);
}

static void op_TR_TIME_set(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    a = normalise_value(0, TR_COUNT - 1, 0, a - 1);
    if (b < 0) b = 0;
    ss->variables.tr_time[a] = b;
}

static void op_TR_TOG_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    // saturate and shift
    if (a < 1)
        a = 1;
    else if (a > 4)
        a = 4;
    a--;
    if (ss->variables.tr[a])
        ss->variables.tr[a] = 0;
    else
        ss->variables.tr[a] = 1;
    tele_tr(a, ss->variables.tr[a]);
}

static void op_TR_PULSE_get(const void *NOTUSED(data), scene_state_t *ss,
                            exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    // saturate and shift
    if (a < 1)
        a = 1;
    else if (a > 4)
        a = 4;
    a--;
    int16_t time = ss->variables.tr_time[a];  // pulse time
    if (time <= 0) return;                    // if time <= 0 don't do anything
    ss->variables.tr[a] = ss->variables.tr_pol[a];
    ss->tr_pulse_timer[a] = time;  // set time
    tele_tr(a, ss->variables.tr[a]);
}

static void op_II_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                      exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    tele_ii(a, b);
}

static void op_CV_SET_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    // saturate and shift
    if (a < 1)
        a = 1;
    else if (a > 4)
        a = 4;
    a--;
    if (b < 0)
        b = 0;
    else if (b > 16383)
        b = 16383;
    ss->variables.cv[a] = b;
    tele_cv(a, b, 0);
}

static void op_MUTE_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                        exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a;
    a = cs_pop(cs);

    if (a > 0 && a < 9) { tele_mute(a - 1, 0); }
}

static void op_UNMUTE_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a;
    a = cs_pop(cs);

    if (a > 0 && a < 9) { tele_mute(a - 1, 1); }
}

static void op_STATE_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    a--;
    if (a < 0)
        a = 0;
    else if (a > 7)
        a = 7;

    cs_push(cs, tele_get_input_state(a));
}
