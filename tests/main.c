#include <stdint.h>

#include "greatest/greatest.h"

#include "teletype.h"

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
void tele_scene(uint8_t i) {}
void tele_pi() {}
void tele_script(uint8_t a) {}
void tele_kill() {}
void tele_mute(uint8_t i, uint8_t s) {}
void tele_input_state(uint8_t n) {}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    update_metro = &tele_metro;
    update_tr = &tele_tr;
    update_cv = &tele_cv;
    update_cv_slew = &tele_cv_slew;
    update_delay = &tele_delay;
    update_s = &tele_s;
    update_cv_off = &tele_cv_off;
    update_ii = &tele_ii;
    update_scene = &tele_scene;
    update_pi = &tele_pi;
    run_script = &tele_script;
    update_kill = &tele_kill;
    update_mute = &tele_mute;
    update_input = &tele_input_state;

    GREATEST_MAIN_BEGIN();

    RUN_SUITE(parser_suite);
    RUN_SUITE(process_suite);

    GREATEST_MAIN_END();
}
