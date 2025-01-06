# assignment grading automation

Automatically run tests on a set of submissions for a programming assignment. Assignments should be based on [assignment-template](https://github.com/themmj/assignment-template).

## Prerequisites

- assignment is a C project with a `main.c` containing the `main` function  
- each submission is a zip file containing the assignments whole repository (including `.git` folder)
- gcc toolchain installed (`ld` is required for the wrapping functionality)

## Features

Besides general build automation and some cmake fun, this project provides the following features:

### CMocka capabilities

The individual testcases for an assignment are defined using [CMocka](https://api.cmocka.org/index.html). This means that all capabilites from assertions to paramteter checking are possible.

### Execute the submission's `main`

The headers and cmake are configured in a way which renames the `main` function inside the submission's `main.c` when running the tests. This way it can be called with `CALL_ASSIGNMENT_MAIN` inside a test case, passing a variable number of arguments.

### Function wrapping

Using `ld`'s wrapper support one can define `__wrap_symbol` as a wrapper function for `symbol`. Any undefined reference to `symbol` will be linked against that wrapper. Calling `__real_symbol` will invoke the normal definition of `symbol`. Because every symbol intended for wrapping has to be passed to the linker manually the [configuration](#configuration) contains a variable containing all symbol names that should be wrapped. The rest is done automatically. Defining a wrapper function can easily be done with the `FUNC_WRAPPER` macro.

### Git user extraction

Extracts git user name from the latest commit to combat blatant copy/pasting of submissions.

### Score calculation

The score for a submission is calculated using the test results and the maximum points defined in the [configuration](#configuration). A build error counts as 0%. In case a submission does not achieve 100% its test failures or build errors are preserved for manual investigation if needed.

### CSV export

The results are written to stdout and a result file in a CSV format using tabs as delimiters. (commas are not well suited as an error log may contain commas)

## Configuration

The assignment configuration resides in `./assignment.conf`. The individual paramteres stand for the following:

- `max_points` (integer): represents the number of testcases defined for the assignment
- `submission_zips_location` (string) absolute or relative path to all the submissions
- `wrap_functions` (array of strings) symbols which should be wrapped during linking
- `assignment_source_files` (array of strings) list of source files contained in each assignment (relative to the root of the submission repository)

## CLI options

- `-su` skip unzip. For the general use case: run the script once without this option and then always with this option. This allows you to edit the submissions code and rerun the tests without overwriting these edits with the zip content.
- `--submission=NR` run a single submission. Replace NR with the submission folder's number you want to run. E.g. `--submission=0` to run the first submission.
