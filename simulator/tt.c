#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "teletype.h"
#include "teletype_io.h"
#include "util.h"


void tele_metro_updated() {
    printf("METRO UPDATED");
    printf("\n");
}

void tele_metro_reset() {
    printf("METRO RESET");
    printf("\n");
}

void tele_tr(uint8_t i, int16_t v) {
    printf("TR  i:%" PRIu8 " v:%" PRId16, i, v);
    printf("\n");
}

void tele_cv(uint8_t i, int16_t v, uint8_t s) {
    printf("CV  i:%" PRIu8 " v:%" PRId16 " s:%" PRIu8, i, v, s);
    printf("\n");
}

void tele_cv_slew(uint8_t i, int16_t v) {
    printf("CV_SLEW  i:%" PRIu8 " v:%" PRId16, i, v);
    printf("\n");
}

void tele_update_in(void) {
    printf("UPDATE IN");
    printf("\n");
}

void tele_has_delays(bool i) {
    printf("DELAY  i:%s", i ? "true" : "false");
    printf("\n");
}

void tele_has_stack(bool i) {
    printf("STACK  i:%s", i ? "true" : "false");
    printf("\n");
}

void tele_cv_off(uint8_t i, int16_t v) {
    printf("CV_OFF  i:%" PRIu8 " v:%" PRId16, i, v);
    printf("\n");
}

void tele_ii_tx(uint8_t addr, uint8_t *data, uint8_t l) {
    printf("II_tx  addr:%" PRIu8 " l:%" PRIu8, addr, l);
    printf("\n");
    for (size_t i = 0; i < l; i++) {
        printf("[%" PRIuPTR "] = %" PRIu8 "\n", i, data[i]);
    }
}

void tele_ii_rx(uint8_t addr, uint8_t *data, uint8_t l) {
    printf("II_rx  addr:%" PRIu8 " l:%" PRIu8, addr, l);
    printf("\n");
}

void tele_scene(uint8_t i) {
    printf("SCENE  i:%" PRIu8, i);
    printf("\n");
}

void tele_pattern_updated() {
    printf("PATTERN UPDATED");
    printf("\n");
}

void tele_kill() {
    printf("KILL");
    printf("\n");
}

void tele_mute() {
    printf("MUTE");
    printf("\n");
}

bool tele_get_input_state(uint8_t n) {
    printf("INPUT_STATE  n:%" PRIu8, n);
    printf("\n");
    return false;
}

int main() {
    char *in;
    time_t t;
    error_t status;
    int i;

    srand((unsigned)time(&t));

    // tele_command_t stored;
    // stored.data[0].t = OP;
    // stored.data[0].v = 2;
    // stored.data[1].t = NUMBER;
    // stored.data[1].v = 8;
    // stored.data[2].t = NUMBER;
    // stored.data[2].v = 10;
    // stored.separator = -1;
    // stored.l = 3;
    // printf("\nstored process: ");
    // process(&stored);

    in = malloc(256);

    printf("teletype. (blank line quits)\n\n");

    scene_state_t ss;
    ss_init(&ss);

    do {
        printf("> ");
        fgets(in, 256, stdin);

        i = 0;
        while (in[i]) {
            in[i] = toupper(in[i]);
            i++;
        }

        tele_command_t temp;
        exec_state_t es;
        es_init(&es);
        char error_msg[TELE_ERROR_MSG_LENGTH];
        status = parse(in, &temp, error_msg);
        if (status == E_OK) {
            status = validate(&temp, error_msg);
            printf("validate: %s", tele_error(status));
            if (error_msg[0]) printf(": %s", error_msg);
            printf("\n");
            if (status == E_OK) {
                process_result_t output = process_command(&ss, &es, &temp);
                if (output.has_value) { printf(">>> %i\n", output.value); }
            }
        }
        else {
            printf("ERROR: %s", tele_error(status));
            if (error_msg[0]) printf(": %s", error_msg);
            printf("\n");
        }

        // tele_tick(100);
        printf("\n");
    } while (in[0] != 10);

    free(in);

    printf("(teletype exit.)\n");
}
