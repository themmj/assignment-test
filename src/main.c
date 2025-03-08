#include <stdio.h>

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
    UNUSED(argc);
    UNUSED(argv);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
