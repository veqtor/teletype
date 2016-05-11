#include "ops/delay.h"

#include "helpers.h"

static void op_DEL_CLR_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);


const tele_op_t op_DEL_CLR =
    MAKE_GET_OP(DEL.CLR, op_DEL_CLR_get, 0, false, "DELAY: FLUSH");

static void op_DEL_CLR_get(const void *NOTUSED(data),
                           scene_state_t *NOTUSED(ss),
                           exec_state_t *NOTUSED(es),
                           command_state_t *NOTUSED(cs)) {
    clear_delays();
}
