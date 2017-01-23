#ifndef _TELETYPE_H_
#define _TELETYPE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "command.h"
#include "state.h"

#define ERROR_MSG_LENGTH 16

#define WELCOME "TELETYPE 1.4.0"


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


error_t parse(const char *cmd, tele_command_t *out,
              char error_msg[ERROR_MSG_LENGTH]);
error_t validate(const tele_command_t *c, char error_msg[ERROR_MSG_LENGTH]);
process_result_t run_script(size_t script_no);
process_result_t run_command(const tele_command_t *cmd);
process_result_t process(exec_state_t *es, const tele_command_t *c);

void tele_tick(uint8_t);

void clear_delays(void);

void tele_init(void);

void tele_set_in(int16_t value);
void tele_set_param(int16_t value);
void tele_set_scene(int16_t value);

int16_t tele_get_pattern_i(size_t pattern);
void tele_set_pattern_i(size_t pattern, int16_t i);
int16_t tele_get_pattern_l(size_t pattern);
void tele_set_pattern_l(size_t pattern, int16_t l);
uint16_t tele_get_pattern_wrap(size_t pattern);
void tele_set_pattern_wrap(size_t pattern, uint16_t wrap);
int16_t tele_get_pattern_start(size_t pattern);
void tele_set_pattern_start(size_t pattern, int16_t start);
int16_t tele_get_pattern_end(size_t pattern);
void tele_set_pattern_end(size_t pattern, int16_t end);
int16_t tele_get_pattern_val(size_t pattern, size_t idx);
void tele_set_pattern_val(size_t pattern, size_t idx, int16_t val);
scene_pattern_t *tele_patterns_ptr(void);
size_t tele_patterns_size(void);
uint8_t tele_get_script_l(size_t idx);
void tele_set_script_l(size_t idx, uint8_t l);
const tele_command_t *tele_get_script_c(size_t script_idx, size_t c_idx);
void tele_set_script_c(size_t script_idx, size_t c_idx,
                       const tele_command_t *cmd);
scene_script_t *tele_script_ptr(void);
size_t tele_script_size(void);

const char *tele_error(error_t);


#endif
