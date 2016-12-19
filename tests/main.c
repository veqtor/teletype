#include <stdint.h>

#include "greatest/greatest.h"

#include "teletype.h"
#include "teletype_io.h"

#include "match_token_tests.h"
#include "op_mod_tests.h"
#include "parser_tests.h"
#include "process_tests.h"

void tele_metro(int16_t m, int16_t m_act, uint8_t m_reset) {}
void tele_tr(uint8_t i, int16_t v) {}
void tele_cv(uint8_t i, int16_t v, uint8_t s) {}
void tele_cv_slew(uint8_t i, int16_t v) {}
void tele_delay(uint8_t i) {}
void tele_s(uint8_t i) {}
void tele_cv_off(uint8_t i, int16_t v) {}
void tele_ii(uint8_t i, int16_t d) {}
void tele_ii_tx(uint8_t addr, uint8_t *data, uint8_t l) {}
void tele_ii_tx_now(uint8_t addr, uint8_t *data, uint8_t l) {}
void tele_ii_rx(uint8_t addr, uint8_t *data, uint8_t l) {}
void tele_scene(uint8_t i) {}
void tele_pi() {}
void tele_script(uint8_t a) {}
void tele_kill() {}
void tele_mute(uint8_t i, uint8_t s) {}
bool tele_get_input_state(uint8_t n) {
    return false;
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(match_token_suite);
    RUN_SUITE(op_mod_suite);
    RUN_SUITE(parser_suite);
    RUN_SUITE(process_suite);

    GREATEST_MAIN_END();
}
