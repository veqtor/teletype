#include "command.h"

#include <string.h>  // memcpy

#include "util.h"
#include "ops/op.h"

void copy_command(tele_command_t *dst, const tele_command_t *src) {
    // TODO does this need to use memcpy?
    memcpy(dst, src, sizeof(tele_command_t));
}

void copy_sub_command(tele_command_t *dst, const tele_command_t *src) {
    dst->l = src->l - src->separator - 1;
    dst->separator = -1;
    memcpy(dst->data, &src->data[src->separator + 1],
           dst->l * sizeof(tele_data_t));
}

void print_command(const tele_command_t *c, char *out) {
    int16_t n = 0;
    char number[8];

    *out = 0;

    while (n < c->l) {
        switch (c->data[n].t) {
        case OP:
            strcpy(out, tele_ops[c->data[n].v]->name);
            out += strlen(tele_ops[c->data[n].v]->name) - 1;
            break;
        case NUMBER:
            itoa(c->data[n].v, number, 10);
            strcpy(out, number);
            out += strlen(number) - 1;
            break;
        case MOD:
            strcpy(out, tele_mods[c->data[n].v]->name);
            out += strlen(tele_mods[c->data[n].v]->name) - 1;
            break;
        case SEP: *out = ':'; break;
        default: break;
        }

        n++;
        out++;
        *out = ' ';
        out++;
    }
    out--;
    *out = 0;
}
