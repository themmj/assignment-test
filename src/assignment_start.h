#ifndef _SRC_ASSIGNMENT_START_H
#define _SRC_ASSIGNMENT_START_H

/**
 * Renamed main of assignment. Avoids naming conflicts while remaining invokable by the test main.
 */
int assignment_main(int, char**);

/**
 * Calls assignment_main. VA_ARGS are passed as argv and number of elements is passed as argc.
 */
#define CALL_ASSIGNMENT_MAIN(...) assignment_main(sizeof((char*[]){__VA_ARGS__})/sizeof(char*), (char*[]){__VA_ARGS__})

#endif

