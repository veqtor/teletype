#ifndef _TELETYPE_H_
#define _TELETYPE_H_

#include <stdbool.h>
#include <stdint.h>

#include "command.h"
#include "state.h"

#define SCRIPT_MAX_COMMANDS 6
#define SCRIPT_MAX_COMMANDS_ 5

#define WELCOME "TELETYPE 1.12"


typedef enum {
    E_OK,
    E_WELCOME,
    E_PARSE,
    E_LENGTH,
    E_NEED_PARAMS,
    E_EXTRA_PARAMS,
    E_NO_MOD_HERE,
    E_MANY_SEP,
    E_NEED_SEP,
    E_PLACE_SEP,
    E_NOT_LEFT
} error_t;

typedef struct {
    bool has_value;
    int16_t value;
} process_result_t;


typedef struct {
    uint8_t l;
    tele_command_t c[SCRIPT_MAX_COMMANDS];
} tele_script_t;


typedef struct {
    int16_t i;
    uint16_t l;
    uint16_t wrap;
    int16_t start;
    int16_t end;
    int16_t v[64];
} tele_pattern_t;


error_t parse(char *cmd, tele_command_t *out);
error_t validate(tele_command_t *c);
process_result_t process(tele_command_t *c);
char *print_command(const tele_command_t *c);

void tele_tick(uint8_t);

void clear_delays(void);

void tele_init(void);

void tele_set_in(int16_t value);
void tele_set_param(int16_t value);
void tele_set_scene(int16_t value);

const char *tele_error(error_t);

extern void clear_delays(void);
extern int16_t tr_pulse[4];
extern tele_pattern_t tele_patterns[4];

extern char error_detail[16];

#endif
