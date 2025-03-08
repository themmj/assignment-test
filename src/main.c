#include <stdio.h>
#include <string.h>

#include "testing/common.h"

/* MOCKS */

#include ".assignment/dhbw.h"

FUNC_WRAPPER(void, dhbw_print_line(const char* text)) {
    check_expected_ptr(text);
    __real_dhbw_print_line(text);
}

/* TESTS */

static void test_print_call(void **state) {
    UNUSED(state);
    expect_string(__wrap_dhbw_print_line, text, "Hello, two-user!");

    CALL_ASSIGNMENT_MAIN("main");
}

static const struct CMUnitTest tests[] =
{
    TEST_CASE(test_print_call),
};

/* MAIN */

int main(int argc, const char *argv[]) {
    // run tests
    disable_alloc_checks();
    int test_failures = cmocka_run_group_tests(tests, NULL, NULL);
    enable_alloc_checks();
    test_failures += cmocka_run_group_tests(tests, NULL, NULL);
    fprintf(stderr, "\n%u leftover mem blocks\n", leftover_mem_blocks());

    // calculate score
    const int test_count = sizeof(tests) / sizeof(tests[0]) * 2;
    return 100 - (100 * test_failures / test_count);
}
