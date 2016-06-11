#include "state.h"


void es_init(exec_state_t *es) {
    es->if_else_condition = false;
}

void cs_init(command_state_t *cs) {
    cs->stack.top = 0;
}

int16_t cs_stack_size(command_state_t *cs) {
    return cs->stack.top;
}
