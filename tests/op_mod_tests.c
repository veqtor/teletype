#include "op_mod_tests.h"

#include "greatest/greatest.h"

#include "ops/op.h"

// Check every op has a unique name
TEST unique_ops() {
    for (size_t i = 0; i < E_OP__LENGTH; i++) {
        for (size_t j = 0; j < E_OP__LENGTH; j++) {
            if (i == j) continue;
            ASSERTm(tele_ops[i]->name,
                    strcmp(tele_ops[i]->name, tele_ops[j]->name) != 0);
        }
    }
    PASS();
}

// Check every op has a unique name
TEST unique_mods() {
    for (size_t i = 0; i < E_MOD__LENGTH; i++) {
        for (size_t j = 0; j < E_MOD__LENGTH; j++) {
            if (i == j) continue;
            ASSERTm(tele_mods[i]->name,
                    strcmp(tele_mods[i]->name, tele_mods[j]->name) != 0);
        }
    }
    PASS();
}

SUITE(op_mod_suite) {
    RUN_TEST(unique_ops);
    RUN_TEST(unique_mods);
}
