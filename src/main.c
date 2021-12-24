#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

static void test_true(void **state)
{
    (void) state;
    assert_int_equal(2, 2);
}

static void test_false(void **state)
{
    (void) state;
    assert_int_equal(4, 5);
}

int main()
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(test_true),
        cmocka_unit_test(test_false),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

