#pragma once

#define WRAP_FUNC_DECL(return_type, remaining_signature) return_type __wrap_##remaining_signature
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
    REAL_FUNC_DECL(return_type, remaining_signature); \
    WRAP_FUNC_DECL(return_type, remaining_signature); \
    WRAP_FUNC_DECL(return_type, remaining_signature)
