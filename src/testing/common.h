#pragma once

// cmocka deps
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

// convenience
#define UNUSED(x) (void) x

int assignment_main(int argc, const char *argv[]);
/// Calls assignment_main. VA_ARGS are passed as argv and number of elements is passed as argc.
#define CALL_ASSIGNMENT_MAIN(...) \
    assignment_main(sizeof((const char *[]) {__VA_ARGS__}) / sizeof(char *), (const char *[]) {__VA_ARGS__})

/// Create function declaration prefixed with __wrap_
#define WRAP_FUNC_DECL(return_type, remaining_signature) return_type __wrap_##remaining_signature
/// Create function declaration prefixed with __real_
#define REAL_FUNC_DECL(return_type, remaining_signature) return_type __real_##remaining_signature
/**
 * \brief Generate function wrapper.
 *
 * \param return_type return type of the function to be wrapped
 * \param remaining_signature signature (but without the return type) of the function to be wrapped
 *
 * Example: void print(const char *text)
 *
 * Call: FUNC_WRAPPER(void,print(const char *text) { printf("wrapped: %s", text; }
 *
 * Result: A call to print will link to the new implementation for the test. The original
 * implementation can still be called inside the test with __real_print.
 * See the cmocka referece for potential use cases of this.
 */
#define FUNC_WRAPPER(return_type, remaining_signature) \
    REAL_FUNC_DECL(return_type, remaining_signature);  \
    WRAP_FUNC_DECL(return_type, remaining_signature);  \
    WRAP_FUNC_DECL(return_type, remaining_signature)

/// allocation checking (leaks and integrity)
#define MAX_ALLOCS 8192
int          enable_alloc_checks(void **state);
int          disable_alloc_checks(void **state);
int          alloc_checks_test_setup(void **state);
int          alloc_checks_test_teardown(void **state);
unsigned int leftover_mem_blocks(void);

/// for use as element in CMUnitTest array
#define TEST_CASE(func) cmocka_unit_test_setup_teardown(func, alloc_checks_test_setup, alloc_checks_test_teardown)
