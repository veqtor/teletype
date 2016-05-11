#include <ctype.h>   // isdigit
#include <stddef.h>  // offsetof
#include <stdint.h>  // types
#include <stdio.h>   // printf
#include <stdlib.h>  // rand, strtol
#include <string.h>

#include "helpers.h"
#include "ops/constants.h"
#include "ops/controlflow.h"
#include "ops/delay.h"
#include "ops/hardware.h"
#include "ops/maths.h"
#include "ops/metronome.h"
#include "ops/op.h"
#include "ops/patterns.h"
#include "ops/queue.h"
#include "ops/stack.h"
#include "ops/variables.h"
#include "table.h"
#include "teletype.h"
#include "teletype_io.h"
#include "util.h"

#ifdef SIM
#define DBG printf("%s", dbg);
#else
#include "print_funcs.h"
#define DBG print_dbg(dbg);
#endif

static const char *errordesc[] = { "OK",
                                   WELCOME,
                                   "UNKNOWN WORD",
                                   "COMMAND TOO LONG",
                                   "NOT ENOUGH PARAMS",
                                   "TOO MANY PARAMS",
                                   "MOD NOT ALLOWED HERE",
                                   "EXTRA SEPARATOR",
                                   "NEED SEPARATOR",
                                   "BAD SEPARATOR",
                                   "MOVE LEFT" };

const char *tele_error(error_t e) {
    return errordesc[e];
}

// static char dbg[32];
static char pcmd[32];


char error_detail[16];

uint8_t mutes[8];

tele_pattern_t tele_patterns[4];

tele_command_t tele_stack[TELE_STACK_SIZE];
uint8_t tele_stack_top;

const char *to_v(int16_t);

void copy_command(tele_command_t *dst, tele_command_t *src);
void copy_sub_command(tele_command_t *dst, tele_command_t *src);

/////////////////////////////////////////////////////////////////
// STATE ////////////////////////////////////////////////////////

// eventually these will not be global variables
static scene_state_t scene_state = {
    // variables that haven't been explicitly initialised, will be set to 0
    .variables = {.a = 1,
                  .b = 2,
                  .c = 3,
                  .cv_slew = { 1, 1, 1, 1 },
                  .d = 4,
                  .drunk_min = 0,
                  .drunk_max = 255,
                  .m = 1000,
                  .m_act = 1,
                  .o_inc = 1,
                  .o_min = 0,
                  .o_max = 63,
                  .o_wrap = 1,
                  .q_n = 1,
                  .time_act = 1,
                  .tr_pol = { 1, 1, 1, 1 },
                  .tr_time = { 100, 100, 100, 100 } }
};
static exec_state_t exec_state = {};

/////////////////////////////////////////////////////////////////
// DELAY ////////////////////////////////////////////////////////

static tele_command_t delay_c[TELE_D_SIZE];
static int16_t delay_t[TELE_D_SIZE];
static uint8_t delay_count;
int16_t tr_pulse[4];

static void process_delays(uint8_t);

static void process_delays(uint8_t v) {
    for (int16_t i = 0; i < TELE_D_SIZE; i++) {
        if (delay_t[i]) {
            delay_t[i] -= v;
            if (delay_t[i] <= 0) {
                // sprintf(dbg,"\r\ndelay %d", i);
                // DBG
                process(&delay_c[i]);
                delay_t[i] = 0;
                delay_count--;
                if (delay_count == 0) tele_delay(0);
            }
        }
    }

    for (int16_t i = 0; i < 4; i++) {
        if (tr_pulse[i]) {
            tr_pulse[i] -= v;
            if (tr_pulse[i] <= 0) {
                tr_pulse[i] = 0;
                scene_state.variables.tr[i] =
                    scene_state.variables.tr_pol[i] == 0;
                tele_tr(i, scene_state.variables.tr[i]);
            }
        }
    }
}

void clear_delays(void) {
    for (int16_t i = 0; i < 4; i++) tr_pulse[i] = 0;

    for (int16_t i = 0; i < TELE_D_SIZE; i++) { delay_t[i] = 0; }

    delay_count = 0;

    tele_stack_top = 0;

    tele_delay(0);
    tele_s(0);
}


/////////////////////////////////////////////////////////////////
// MOD //////////////////////////////////////////////////////////

static void mod_PROB(scene_state_t *ss, exec_state_t *es, command_state_t *cs,
                     tele_command_t *sub_command);
static void mod_DEL(scene_state_t *ss, exec_state_t *es, command_state_t *cs,
                    tele_command_t *sub_command);
static void mod_S(scene_state_t *ss, exec_state_t *es, command_state_t *cs,
                  tele_command_t *sub_command);
static void mod_IF(scene_state_t *ss, exec_state_t *es, command_state_t *cs,
                   tele_command_t *sub_command);
static void mod_ELIF(scene_state_t *ss, exec_state_t *es, command_state_t *cs,
                     tele_command_t *sub_command);
static void mod_ELSE(scene_state_t *ss, exec_state_t *es, command_state_t *cs,
                     tele_command_t *sub_command);
static void mod_L(scene_state_t *ss, exec_state_t *es, command_state_t *cs,
                  tele_command_t *sub_command);

#define MODS 7
static const tele_mod_t tele_mods[MODS] = {
    MAKE_MOD(PROB, mod_PROB, 1, "PROBABILITY TO CONTINUE EXECUTING LINE"),
    MAKE_MOD(DEL, mod_DEL, 1, "DELAY THIS COMMAND"),
    MAKE_MOD(S, mod_S, 0, "ADD COMMAND TO STACK"),
    MAKE_MOD(IF, mod_IF, 1, "IF CONDITION FOR COMMAND"),
    MAKE_MOD(ELIF, mod_ELIF, 1, "ELSE IF"),
    MAKE_MOD(ELSE, mod_ELSE, 0, "ELSE"),
    MAKE_MOD(L, mod_L, 2, "LOOPED COMMAND WITH ITERATION")
};


void mod_PROB(scene_state_t *NOTUSED(ss), exec_state_t *NOTUSED(es),
              command_state_t *cs, tele_command_t *sub_command) {
    int16_t a = cs_pop(cs);

    if (rand() % 101 < a) { process(sub_command); }
}
void mod_DEL(scene_state_t *NOTUSED(ss), exec_state_t *NOTUSED(es),
             command_state_t *cs, tele_command_t *sub_command) {
    int16_t i = 0;
    int16_t a = cs_pop(cs);

    if (a < 1) a = 1;

    while (delay_t[i] != 0 && i != TELE_D_SIZE) i++;

    if (i < TELE_D_SIZE) {
        delay_count++;
        if (delay_count == 1) tele_delay(1);
        delay_t[i] = a;

        copy_command(&delay_c[i], sub_command);
    }
}
void mod_S(scene_state_t *NOTUSED(ss), exec_state_t *NOTUSED(es),
           command_state_t *NOTUSED(cs), tele_command_t *sub_command) {
    if (tele_stack_top < TELE_STACK_SIZE) {
        copy_command(&tele_stack[tele_stack_top], sub_command);
        tele_stack_top++;
        if (tele_stack_top == 1) tele_s(1);
    }
}
void mod_IF(scene_state_t *NOTUSED(ss), exec_state_t *es, command_state_t *cs,
            tele_command_t *sub_command) {
    es->if_else_condition = false;
    if (cs_pop(cs)) {
        es->if_else_condition = true;
        process(sub_command);
    }
}
void mod_ELIF(scene_state_t *NOTUSED(ss), exec_state_t *es, command_state_t *cs,
              tele_command_t *sub_command) {
    if (!es->if_else_condition) {
        if (cs_pop(cs)) {
            es->if_else_condition = true;
            process(sub_command);
        }
    }
}
void mod_ELSE(scene_state_t *NOTUSED(ss), exec_state_t *es,
              command_state_t *NOTUSED(cs), tele_command_t *sub_command) {
    if (!es->if_else_condition) {
        es->if_else_condition = true;
        process(sub_command);
    }
}
void mod_L(scene_state_t *NOTUSED(ss), exec_state_t *NOTUSED(es),
           command_state_t *cs, tele_command_t *sub_command) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    int16_t loop_size = a < b ? b - a : a - b;

    for (int16_t i = 0; i <= loop_size; i++) {
        scene_state.variables.i = a < b ? a + i : a - i;
        process(sub_command);
    }
}


/////////////////////////////////////////////////////////////////
// OPS //////////////////////////////////////////////////////////


#define OPS 145
static const tele_op_t *tele_ops[OPS] = {
    // variables
    &op_A, &op_B, &op_C, &op_D, &op_DRUNK, &op_DRUNK_MAX, &op_DRUNK_MIN,
    &op_DRUNK_WRAP, &op_FLIP, &op_I, &op_IN, &op_O, &op_O_INC, &op_O_MAX,
    &op_O_MIN, &op_O_WRAP, &op_PARAM, &op_T, &op_TIME, &op_TIME_ACT, &op_X,
    &op_Y, &op_Z,

    // metronome
    &op_M, &op_M_ACT, &op_M_RESET,

    // patterns
    &op_P, &op_P_HERE, &op_P_END, &op_P_I, &op_P_L, &op_P_N, &op_P_NEXT,
    &op_P_PREV, &op_P_START, &op_P_WRAP, &op_P_INS, &op_P_RM, &op_P_PUSH,
    &op_P_POP, &op_PN,

    // queue
    &op_Q, &op_Q_AVG, &op_Q_N,

    // hardware
    &op_CV, &op_CV_OFF, &op_CV_SLEW, &op_TR, &op_TR_POL, &op_TR_TIME,
    &op_TR_TOG, &op_TR_PULSE, &op_II, &op_CV_SET, &op_MUTE, &op_UNMUTE,
    &op_STATE,

    // maths
    &op_ADD, &op_SUB, &op_MUL, &op_DIV, &op_MOD, &op_RAND, &op_RRAND, &op_TOSS,
    &op_MIN, &op_MAX, &op_LIM, &op_WRAP, &op_QT, &op_AVG, &op_EQ, &op_NE,
    &op_LT, &op_GT, &op_NZ, &op_EZ, &op_RSH, &op_LSH, &op_EXP, &op_ABS, &op_AND,
    &op_OR, &op_XOR, &op_JI, &op_SCALE, &op_N, &op_V, &op_VV, &op_ER,

    // stack
    &op_S_ALL, &op_S_POP, &op_S_CLR, &op_S_L,

    // controlflow
    &op_SCRIPT, &op_KILL, &op_SCENE,

    // delay
    &op_DEL_CLR,

    // constants
    &op_WW_PRESET, &op_WW_POS, &op_WW_SYNC, &op_WW_START, &op_WW_END,
    &op_WW_PMODE, &op_WW_PATTERN, &op_WW_QPATTERN, &op_WW_MUTE1, &op_WW_MUTE2,
    &op_WW_MUTE3, &op_WW_MUTE4, &op_WW_MUTEA, &op_WW_MUTEB, &op_MP_PRESET,
    &op_MP_RESET, &op_MP_SYNC, &op_MP_MUTE, &op_MP_UNMUTE, &op_MP_FREEZE,
    &op_MP_UNFREEZE, &op_MP_STOP, &op_ES_PRESET, &op_ES_MODE, &op_ES_CLOCK,
    &op_ES_RESET, &op_ES_PATTERN, &op_ES_TRANS, &op_ES_STOP, &op_ES_TRIPLE,
    &op_ES_MAGIC, &op_ORCA_TRACK, &op_ORCA_CLOCK, &op_ORCA_DIVISOR,
    &op_ORCA_PHASE, &op_ORCA_RESET, &op_ORCA_WEIGHT, &op_ORCA_MUTE,
    &op_ORCA_SCALE, &op_ORCA_BANK, &op_ORCA_PRESET, &op_ORCA_RELOAD,
    &op_ORCA_ROTATES, &op_ORCA_ROTATEW, &op_ORCA_GRESET, &op_ORCA_CVA,
    &op_ORCA_CVB,
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// PROCESS //////////////////////////////////////////////////////

error_t parse(char *cmd, tele_command_t *out) {
    char cmd_copy[32];
    strcpy(cmd_copy, cmd);
    const char *delim = " \n";
    const char *s = strtok(cmd_copy, delim);

    uint8_t n = 0;
    out->l = n;

    while (s) {
        // CHECK IF NUMBER
        if (isdigit(s[0]) || s[0] == '-') {
            out->data[n].t = NUMBER;
            out->data[n].v = strtol(s, NULL, 0);
        }
        else if (s[0] == ':')
            out->data[n].t = SEP;
        else {
            int16_t i = -1;

            if (i == -1) {
                // CHECK AGAINST OPS
                i = OPS;

                while (i--) {
                    if (!strcmp(s, tele_ops[i]->name)) {
                        out->data[n].t = OP;
                        out->data[n].v = i;
                        break;
                    }
                }
            }

            if (i == -1) {
                // CHECK AGAINST MOD
                i = MODS;

                while (i--) {
                    if (!strcmp(s, tele_mods[i].name)) {
                        out->data[n].t = MOD;
                        out->data[n].v = i;
                        break;
                    }
                }
            }

            if (i == -1) {
                strcpy(error_detail, s);
                return E_PARSE;
            }
        }

        s = strtok(NULL, delim);

        n++;
        out->l = n;

        if (n == COMMAND_MAX_LENGTH) return E_LENGTH;
    }

    return E_OK;
}


/////////////////////////////////////////////////////////////////
// VALIDATE /////////////////////////////////////////////////////

error_t validate(tele_command_t *c) {
    int16_t stack_depth = 0;
    uint8_t idx = c->l;
    c->separator = -1;  // i.e. the index ':'

    while (idx--) {  // process words right to left
        tele_word_t word_type = c->data[idx].t;
        int16_t word_value = c->data[idx].v;
        // A first_cmd is either at the beginning of the command or immediately
        // after the SEP
        bool first_cmd = idx == 0 || c->data[idx - 1].t == SEP;

        if (word_type == NUMBER) { stack_depth++; }
        else if (word_type == OP) {
            const tele_op_t *op = tele_ops[word_value];

            // if we're not a first_cmd we need to return something
            if (!first_cmd && !op->returns) {
                strcpy(error_detail, op->name);
                return E_NOT_LEFT;
            }

            stack_depth -= op->params;

            if (stack_depth < 0) {
                strcpy(error_detail, op->name);
                return E_NEED_PARAMS;
            }

            stack_depth += op->returns ? 1 : 0;

            // if we are in the first_cmd position and there is a set fn
            // decrease the stack depth
            // TODO this is technically wrong. the only reason we get away with
            // it is that it's idx == 0, and the while loop is about to end.
            if (first_cmd && op->set != NULL) stack_depth--;
        }
        else if (word_type == MOD) {
            error_t mod_error = E_OK;

            if (idx != 0)
                mod_error = E_NO_MOD_HERE;
            else if (c->separator == -1)
                mod_error = E_NEED_SEP;
            else if (stack_depth < tele_mods[word_value].params)
                mod_error = E_NEED_PARAMS;
            else if (stack_depth > tele_mods[word_value].params)
                mod_error = E_EXTRA_PARAMS;

            if (mod_error != E_OK) {
                strcpy(error_detail, tele_mods[word_value].name);
                return mod_error;
            }

            stack_depth = 0;
        }
        else if (word_type == SEP) {
            if (c->separator != -1)
                return E_MANY_SEP;
            else if (idx == 0)
                return E_PLACE_SEP;

            c->separator = idx;
            if (stack_depth > 1)
                return E_EXTRA_PARAMS;
            else
                stack_depth = 0;
        }
    }

    if (stack_depth > 1)
        return E_EXTRA_PARAMS;
    else
        return E_OK;
}

void copy_command(tele_command_t *dst, tele_command_t *src) {
    // TODO does this need to use memcpy?
    memcpy(dst, src, sizeof(tele_command_t));
}

void copy_sub_command(tele_command_t *dst, tele_command_t *src) {
    dst->l = src->l - src->separator - 1;
    dst->separator = -1;
    memcpy(dst->data, &src->data[src->separator + 1],
           dst->l * sizeof(tele_data_t));
}

/////////////////////////////////////////////////////////////////
// PROCESS //////////////////////////////////////////////////////

process_result_t process(tele_command_t *c) {
    command_state_t cs;
    cs_init(&cs);

    // if the command has a MOD, only process it
    // allow the MOD to deal with processing the remainder
    int16_t idx = c->separator == -1 ? c->l : c->separator;

    while (idx--) {  // process from right to left
        tele_word_t word_type = c->data[idx].t;
        int16_t word_value = c->data[idx].v;

        if (word_type == NUMBER) { cs_push(&cs, word_value); }
        else if (word_type == OP) {
            const tele_op_t *op = tele_ops[word_value];

            // if we're in the first command position, and there is a set fn
            // pointer and we have enough params, then run set, else run get
            if (idx == 0 && op->set != NULL &&
                cs_stack_size(&cs) >= op->params + 1)
                op->set(op->data, &scene_state, &exec_state, &cs);
            else
                op->get(op->data, &scene_state, &exec_state, &cs);
        }
        else if (word_type == MOD) {
            tele_command_t sub_command;
            copy_sub_command(&sub_command, c);
            tele_mods[word_value].func(&scene_state, &exec_state, &cs,
                                       &sub_command);
        }
    }

    if (cs_stack_size(&cs)) {
        process_result_t o = {.has_value = true, .value = cs_pop(&cs) };
        return o;
    }
    else {
        process_result_t o = {.has_value = false, .value = 0 };
        return o;
    }
}


char *print_command(const tele_command_t *c) {
    int16_t n = 0;
    char number[8];
    char *p = pcmd;

    *p = 0;

    while (n < c->l) {
        switch (c->data[n].t) {
            case OP:
                strcpy(p, tele_ops[c->data[n].v]->name);
                p += strlen(tele_ops[c->data[n].v]->name) - 1;
                break;
            case NUMBER:
                itoa(c->data[n].v, number, 10);
                strcpy(p, number);
                p += strlen(number) - 1;
                break;
            case MOD:
                strcpy(p, tele_mods[c->data[n].v].name);
                p += strlen(tele_mods[c->data[n].v].name) - 1;
                break;
            case SEP: *p = ':'; break;
            default: break;
        }

        n++;
        p++;
        *p = ' ';
        p++;
    }
    p--;
    *p = 0;

    return pcmd;
}


void tele_set_in(int16_t value) {
    scene_state.variables.in = value;
}

void tele_set_param(int16_t value) {
    scene_state.variables.param = value;
}

void tele_set_scene(int16_t value) {
    scene_state.variables.scene = value;
}

void tele_tick(uint8_t i) {
    process_delays(i);

    // inc time
    if (scene_state.variables.time_act) scene_state.variables.time += i;
}

void tele_init() {
    u8 i;

    for (i = 0; i < 4; i++) {
        tele_patterns[i].i = 0;
        tele_patterns[i].l = 0;
        tele_patterns[i].wrap = 1;
        tele_patterns[i].start = 0;
        tele_patterns[i].end = 63;
    }

    for (i = 0; i < 8; i++) mutes[i] = 1;
}


const char *to_v(int16_t i) {
    static char n[3], v[7];
    int16_t a = 0, b = 0;

    if (i > table_v[8]) {
        i -= table_v[8];
        a += 8;
    }

    if (i > table_v[4]) {
        i -= table_v[4];
        a += 4;
    }

    if (i > table_v[2]) {
        i -= table_v[2];
        a += 2;
    }

    if (i > table_v[1]) {
        i -= table_v[1];
        a += 1;
    }

    if (i > table_vv[64]) {
        i -= table_vv[64];
        b += 64;
    }

    if (i > table_vv[32]) {
        i -= table_vv[32];
        b += 32;
    }

    if (i > table_vv[16]) {
        i -= table_vv[16];
        b += 16;
    }

    if (i > table_vv[8]) {
        i -= table_vv[8];
        b += 8;
    }

    if (i > table_vv[4]) {
        i -= table_vv[4];
        b += 4;
    }

    if (i > table_vv[2]) {
        i -= table_vv[2];
        b++;
    }

    if (i > table_vv[1]) {
        i -= table_vv[1];
        b++;
    }

    b++;

    itoa(a, n, 10);
    strcpy(v, n);
    strcat(v, ".");
    itoa(b, n, 10);
    strcat(v, n);
    strcat(v, "V");

    return v;
    // printf(" (VV %d %d)",a,b);
}
