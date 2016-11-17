#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdint.h>

#define COMMAND_MAX_LENGTH 12

typedef enum { NUMBER, MOD, SEP, OP } tele_word_t;

typedef struct {
    tele_word_t t;
    int16_t v;
} tele_data_t;

typedef struct {
    uint8_t length;
    int8_t separator;
    tele_data_t data[COMMAND_MAX_LENGTH];
} tele_command_t;

void copy_command(tele_command_t *dst, const tele_command_t *src);
void copy_sub_command(tele_command_t *dst, const tele_command_t *src);
void print_command(const tele_command_t *c, char *out);

#endif
