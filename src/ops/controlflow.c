#include "ops/controlflow.h"

#include <stdlib.h>

#include "helpers.h"
#include "teletype.h"
#include "teletype_io.h"

static void mod_PROB_func(scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs, tele_command_t *sub_command);
static void mod_IF_func(scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs, tele_command_t *sub_command);
static void mod_ELIF_func(scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs, tele_command_t *sub_command);
static void mod_ELSE_func(scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs, tele_command_t *sub_command);
static void mod_L_func(scene_state_t *ss, exec_state_t *es, command_state_t *cs,
                       tele_command_t *sub_command);

static void op_SCENE_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_SCENE_set(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_SCRIPT_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_KILL_get(const void *data, scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs);


const tele_mod_t mod_PROB =
    MAKE_MOD(PROB, mod_PROB_func, 1, "PROBABILITY TO CONTINUE EXECUTING LINE");
const tele_mod_t mod_IF =
    MAKE_MOD(IF, mod_IF_func, 1, "IF CONDITION FOR COMMAND");
const tele_mod_t mod_ELIF = MAKE_MOD(ELIF, mod_ELIF_func, 1, "ELSE IF");
const tele_mod_t mod_ELSE = MAKE_MOD(ELSE, mod_ELSE_func, 0, "ELSE");
const tele_mod_t mod_L =
    MAKE_MOD(L, mod_L_func, 2, "LOOPED COMMAND WITH ITERATION");

const tele_op_t op_SCRIPT =
    MAKE_GET_OP(SCRIPT, op_SCRIPT_get, 1, false, "CALL SCRIPT");
const tele_op_t op_KILL =
    MAKE_GET_OP(KILL, op_KILL_get, 0, false, "CLEAR DELAYS, STACK, SLEW");
const tele_op_t op_SCENE =
    MAKE_GET_SET_OP(SCENE, op_SCENE_get, op_SCENE_set, 0, true, "SCENE");


static void mod_PROB_func(scene_state_t *NOTUSED(ss), exec_state_t *NOTUSED(es),
                          command_state_t *cs, tele_command_t *sub_command) {
    int16_t a = cs_pop(cs);

    if (rand() % 101 < a) { process(sub_command); }
}

static void mod_IF_func(scene_state_t *NOTUSED(ss), exec_state_t *es,
                        command_state_t *cs, tele_command_t *sub_command) {
    es->if_else_condition = false;
    if (cs_pop(cs)) {
        es->if_else_condition = true;
        process(sub_command);
    }
}

static void mod_ELIF_func(scene_state_t *NOTUSED(ss), exec_state_t *es,
                          command_state_t *cs, tele_command_t *sub_command) {
    if (!es->if_else_condition) {
        if (cs_pop(cs)) {
            es->if_else_condition = true;
            process(sub_command);
        }
    }
}

static void mod_ELSE_func(scene_state_t *NOTUSED(ss), exec_state_t *es,
                          command_state_t *NOTUSED(cs),
                          tele_command_t *sub_command) {
    if (!es->if_else_condition) {
        es->if_else_condition = true;
        process(sub_command);
    }
}

static void mod_L_func(scene_state_t *ss, exec_state_t *NOTUSED(es),
                       command_state_t *cs, tele_command_t *sub_command) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    int16_t loop_size = a < b ? b - a : a - b;

    for (int16_t i = 0; i <= loop_size; i++) {
        ss->variables.i = a < b ? a + i : a - i;
        process(sub_command);
    }
}

static void op_SCENE_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, ss->variables.scene);
}

static void op_SCENE_set(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t scene = cs_pop(cs);
    ss->variables.scene = scene;
    tele_scene(scene);
}

static void op_SCRIPT_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint16_t a = cs_pop(cs);
    if (a > 0 && a < 9) tele_script(a);
}
static void op_KILL_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                        exec_state_t *NOTUSED(es),
                        command_state_t *NOTUSED(cs)) {
    clear_delays();
    tele_kill();
}
