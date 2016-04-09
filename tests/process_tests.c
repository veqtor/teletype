#include "process_tests.h"

#include "greatest/greatest.h"

#include "teletype.h"

// runs multiple lines of commands and then asserts that the final answer is
// correct
TEST process_helper(size_t n, char* lines[], int16_t answer) {
    process_result_t result = {.h = PR_EMPTY, .v = 0 };
    for (size_t i = 0; i < n; i++) {
        tele_command_t cmd;
        error_t error = parse(lines[i], &cmd);
        if (error != E_OK) { FAIL(); }
        if (validate(&cmd) != E_OK) { FAIL(); }
        result = process(&cmd);
    }
    ASSERT_EQ(PR_VALUE, result.h);
    ASSERT_EQ(answer, result.v);

    PASS();
}

TEST test_ADD() {
    // beware of global state!!!
    char* test1[1] = { "ADD 5 6" };
    CHECK_CALL(process_helper(1, test1, 11));

    char* test2[1] = { "ADD 2 -2" };
    CHECK_CALL(process_helper(1, test2, 0));

    PASS();
}

TEST test_IF() {
    char* test1[3] = { "X 0", "IF 1 : X 1", "X"};
    CHECK_CALL(process_helper(3, test1, 1));

    char* test2[3] = { "X 0", "IF 0 : X 1", "X"};
    CHECK_CALL(process_helper(3, test2, 0));

    char* test3[3] = { "PN 0 0 0", "IF 1 : PN 0 0 1", "PN 0 0"};
    CHECK_CALL(process_helper(3, test3, 1));

    char* test4[3] = { "PN 0 0 0", "IF 0 : PN 0 0 1", "PN 0 0"};
    CHECK_CALL(process_helper(3, test4, 0));

    PASS();
}

TEST test_P() {
    // beware of global state!!!
    char* test1[2] = { "P 0 1", "P 0" };
    CHECK_CALL(process_helper(2, test1, 1));

    char* test2[2] = { "P 0 2", "P 0" };
    CHECK_CALL(process_helper(2, test2, 2));

    PASS();
}

TEST test_PN() {
    // beware of global state!!!
    char* test1[2] = { "PN 0 0 1", "PN 0 0" };
    CHECK_CALL(process_helper(2, test1, 1));

    char* test2[2] = { "PN 0 0 2", "PN 0 0" };
    CHECK_CALL(process_helper(2, test2, 2));

    char* test3[3] = { "P.N 0", "P 0 3", "PN 0 0" };
    CHECK_CALL(process_helper(3, test3, 3));

    char* test4[3] = { "P.N 0", "PN 0 0 4", "P 0" };
    CHECK_CALL(process_helper(3, test4, 4));

    PASS();
}

TEST test_X() {
    // beware of global state!!!
    char* test1[2] = { "X 0", "X" };
    CHECK_CALL(process_helper(2, test1, 0));

    char* test2[2] = { "X 10", "X" };
    CHECK_CALL(process_helper(2, test2, 10));

    PASS();
}

SUITE(process_suite) {
    RUN_TEST(test_ADD);
    RUN_TEST(test_IF);
    RUN_TEST(test_P);
    RUN_TEST(test_PN);
    RUN_TEST(test_X);
}
