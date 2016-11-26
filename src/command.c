#include "command.h"

#include <string.h>  // memcpy

#include "ops/op.h"
#include "util.h"

void copy_command(tele_command_t *dst, const tele_command_t *src) {
    // TODO does this need to use memcpy?
    memcpy(dst, src, sizeof(tele_command_t));
}

void copy_sub_command(tele_command_t *dst, const tele_command_t *src) {
    dst->length = src->length - src->separator - 1;
    dst->separator = -1;
    memcpy(dst->data, &src->data[src->separator + 1],
           dst->length * sizeof(tele_data_t));
}

void print_command(const tele_command_t *c, char *out) {
    *out = 0;
    for (int n = 0; n < c->length; n++) {
        switch (c->data[n].tag) {
            case OP: strcat(out, tele_ops[c->data[n].value]->name); break;
            case NUMBER: {
                char number[8];
                itoa(c->data[n].value, number, 10);
                strcat(out, number);
                break;
            }
            case MOD: strcat(out, tele_mods[c->data[n].value]->name); break;
            case SEP: strcat(out, ":"); break;
            default: break;
        }
        strcat(out, " ");
    }
    size_t out_len = strlen(out);
    if (out_len > 0) out[out_len - 1] = 0;
}
