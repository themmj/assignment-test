#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>
#include "assignment_start.h"
#define UNUSED(x) (void)x

/* MOCKS */

#include ".assignment/dhbw.h"

void __real_dhbw_print_line(const char* text);
void __wrap_dhbw_print_line(const char* text);

void __wrap_dhbw_print_line(const char* text) {
    check_expected_ptr(text);
    __real_dhbw_print_line(text);
}

/* TESTS */

static void test_print_call(void **state)
{
    UNUSED(state);
    expect_string(__wrap_dhbw_print_line, text, "Hello, two-user!");

    CALL_ASSIGNMENT_MAIN("main");
}

/* MAIN */

int main(int argc, char** argv)
{
    UNUSED(argc);
    UNUSED(argv);
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(test_print_call),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

