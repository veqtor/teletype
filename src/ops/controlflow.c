#include "ops/controlflow.h"

#include "helpers.h"
#include "teletype_io.h"

static void op_SCENE_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_SCENE_set(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_SCRIPT_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_KILL_get(const void *data, scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs);


const tele_op_t op_SCRIPT =
    MAKE_GET_OP(SCRIPT, op_SCRIPT_get, 1, false, "CALL SCRIPT");
const tele_op_t op_KILL =
    MAKE_GET_OP(KILL, op_KILL_get, 0, false, "CLEAR DELAYS, STACK, SLEW");
const tele_op_t op_SCENE =
    MAKE_GET_SET_OP(SCENE, op_SCENE_get, op_SCENE_set, 0, true, "SCENE");


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
