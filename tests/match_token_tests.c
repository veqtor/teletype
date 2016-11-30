#include "match_token_tests.h"

#include <string.h>

#include "greatest/greatest.h"

#include "match_token.h"
#include "ops/op.h"

// This test asserts that match_token always returns the correct op, it does
// this by starting with the op in question, extracting the name and running
// that through match_token.
TEST match_token_should_return_op() {
    for (size_t i = 0; i < TELE_NUM_OPS; i++) {
        const tele_op_t* op = tele_ops[i];
        const char* text = op->name;
        tele_data_t data;
        bool result = match_token(text, strlen(text), &data);
        ASSERT_EQ(result, true);
        ASSERT_EQ(data.tag, OP);
        ASSERT_EQ(data.value, (int16_t)i);
    }
    PASS();
}

// As match_token_should_return_op, but for mods.
TEST match_token_should_return_mod() {
    for (size_t i = 0; i < TELE_NUM_MODS; i++) {
        const tele_mod_t* mod = tele_mods[i];
        const char* text = mod->name;
        tele_data_t data;
        bool result = match_token(text, strlen(text), &data);
        ASSERT_EQ(result, true);
        ASSERT_EQ(data.tag, MOD);
        ASSERT_EQ(data.value, (int16_t)i);
    }
    PASS();
}

SUITE(match_token_suite) {
    RUN_TEST(match_token_should_return_op);
    RUN_TEST(match_token_should_return_mod);
}
