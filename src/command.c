#include "command.h"

#include <string.h> // memcpy

void copy_command(tele_command_t *dst, tele_command_t *src) {
    // TODO does this need to use memcpy?
    memcpy(dst, src, sizeof(tele_command_t));
}

void copy_sub_command(tele_command_t *dst, tele_command_t *src) {
    dst->l = src->l - src->separator - 1;
    dst->separator = -1;
    memcpy(dst->data, &src->data[src->separator + 1],
           dst->l * sizeof(tele_data_t));
}
