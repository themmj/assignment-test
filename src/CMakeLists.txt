set (SRC_FILES
    main.c
    testing/alloc.c
)

add_executable(${BINARY} ${SRC_FILES} ${APP_SRC_FILES})

target_include_directories(${BINARY} PUBLIC
    ${INCLUDE_DIRS}
)

target_link_libraries(${BINARY}
    ${LIBRARIES}
)

target_compile_options(${BINARY} PUBLIC
    ${BIN_COMPILE_OPTIONS}
)

target_link_options(${BINARY} PUBLIC
    ${BIN_LINK_OPTIONS}
)

set_target_properties(
    ${BINARY} PROPERTIES

    # set output to bin dir
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

add_custom_target(run
    COMMAND ${BIN_DIR}/${BINARY}
    WORKING_DIRECTORY ${BIN_DIR}
)

add_dependencies(run ${BINARY})

