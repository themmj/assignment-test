#ifndef _SRC_ASSIGNMENT_START_H
#define _SRC_ASSIGNMENT_START_H

int assignment_main(int argc, const char *argv[]);

/**
 * Calls assignment_main. VA_ARGS are passed as argv and number of elements is passed as argc.
 */
#define CALL_ASSIGNMENT_MAIN(...) \
    assignment_main(sizeof((const char *[]){__VA_ARGS__}) / sizeof(char *), (const char *[]){__VA_ARGS__})
#endif
