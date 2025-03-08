cmake_minimum_required(VERSION 3.10)

project(assignment-test)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(DEBUG_BUILD "Build in debug mode" OFF)

# general settings
set(BINARY assignment-test)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/out)
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third-party)

# add cmocka
include(${THIRD_PARTY_DIR}/FetchCMocka.cmake)

# include dirs
set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
    ${THIRD_PARTY_DIR}/cmocka/include
)

# libraries
set(LIBRARIES
    cmocka
)

# options for bin
if(${DEBUG_BUILD})
    set(BIN_COMPILE_OPTIONS
        -DDEBUG_BUILD
        -O0
        -g
    )
else()
    set(BIN_COMPILE_OPTIONS
        -OFast
    )
endif()

set(BIN_COMPILE_OPTIONS
    ${BIN_COMPILE_OPTIONS}
    -std=c17
    -Wall
    -Werror
    -Wextra
    -Wpedantic
    -Wconversion
    -Wcast-align
    -Wno-unused # TODO reintroduce this and make UNUSED macro available?
    -Wshadow
    -Wformat-security
    -Wmissing-prototypes
    -Wmissing-format-attribute
    -D_ASSIGNMENT_RUNNING_TESTS
)

include(submissions/wraps.cmake)
include(submissions/submission.cmake)

add_subdirectory(src)

