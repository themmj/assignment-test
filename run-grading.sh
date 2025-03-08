#!/bin/bash

abort() {
    echo "$1"
    echo "aborting..."
    exit 1
}

skip_unzip=0
single_submission=""
# init arguments
for arg in "$@"; do
    case "$arg" in
        "-su")
            skip_unzip=1
            ;;
        *)
            IFS='='
            read -ra split <<< "$arg"
            case "${split[0]}" in
                "--submission")
                    single_submission="${split[1]}"
                    ;;
                *)
                    abort "unknow option $arg"
                    ;;
            esac
            IFS=' '
            ;;
    esac
done

# delimiter used in csv
delimiter='\t'

# assert pwd to be repository root
project_root="$PWD"
[ -f "$project_root/assignment.conf" ] || abort "please run this script from the repository root"
[ -f "$project_root/run-grading.sh" ] || abort "please run this script from the repository root"
submission_repos_root="$project_root/submissions"
build_root="$project_root/build"

mkdir -p "$submission_repos_root"
mkdir -p "$build_root"

source ./assignment.conf

# setup wraps
wrap_cmake_file="$submission_repos_root/wraps.cmake"
echo "generating compiler options for function wrapping in $wrap_cmake_file"
echo "# automatically generated wrap compiler options based on assignment.conf, which should be edited instead of this file" > "$wrap_cmake_file"
linker_flags="-Wl"
for func in "${wrap_functions[@]}"; do
    echo "adding wrapping for $func"
    linker_flags="$linker_flags,--wrap=$func"
done
[ "$linker_flags" = "-Wl" ] || echo "set(BIN_LINK_OPTIONS $linker_flags)" >> "$wrap_cmake_file"

# gather all available zip files
submission_zips=()
for filename in $submission_zips_location/*; do
    [[ "$filename" == *.zip ]] && submission_zips+=("$filename")
done
echo "processing ${#submission_zips[@]} submissions"

# output of results
output=()
output+=("folder $delimiter zip $delimiter github_name $delimiter score $delimiter error_output")

# loop over all zips
current_submission=0
for zip_file in "${submission_zips[@]}"; do
    echo "processing submission $current_submission associated with $zip_file"
    current_submission_repo_dir="$submission_repos_root/$current_submission"
    ((current_submission++))

    # in case a single submission is requested, skip all others
    [ -n "$single_submission" ] && [ "$single_submission" != "$(basename $current_submission_repo_dir)" ] && continue

    # skip unzipping step if desired
    if [ $skip_unzip -eq 0 ]; then
        echo "moving zip contents to $current_submission_repo_dir"
        mkdir -p "$current_submission_repo_dir"
        unzip -o "$zip_file" -d "$current_submission_repo_dir" > /dev/null
        [ $? -eq 0 ] || abort "error during unzipping of submission"
    fi

    # setup appication sources
    submission_files=()
    submission_cmake_file="$submission_repos_root/submission.cmake"
    echo "writing include dirs and submission source files to $submission_cmake_file"
    echo "# automatically generated include dirs and application files based on assignment.conf, which should be edited instead of this file" > "$submission_cmake_file"
    for file in "${assignment_source_files[@]}"; do
        full_file="$current_submission_repo_dir/$file"
        # File content replacements could be done here following the below format
        [[ "$full_file" == */main.c ]] && [ -f $full_file ] && sed -i 's/ *int *main *.int/int assignment_main(int argc, const char *argv[]);int assignment_main(int/' "$full_file"
        submission_files+=("$full_file")
    done
    echo "set(APP_SRC_FILES ${submission_files[*]})" >> "$submission_cmake_file"
    echo "set(INCLUDE_DIRS \${INCLUDE_DIRS} $current_submission_repo_dir)" >> "$submission_cmake_file"

    # extract git user
    cd "$current_submission_repo_dir" && commit_user_name="$(git log -1 | grep Author | cut -d "<" -f 1 | cut -c 9-)" && cd - || abort "unable to extract author from most recent commit"
    echo "identified user $commit_user_name based on latest commit user"

    # build cmake project
    cd "$build_root" && cmake .. -DDEBUG_BUILD=ON && cd - || abort "cmake did not initialize correctly"

    # build and run binary   
    test_result=0
    error_output=""
    cd "$build_root"
    make_output=$(make 2>&1 > /dev/null)
    if [ $? -eq 0 ]; then
        # build successful. run tests and calculate score
        test_err_output="$(../out/assignment-test 2>&1 > /dev/null)"
        test_result=$?
        [ $test_result -ne 100 ] && error_output="$test_err_output"
    else
        echo "error during compilation"
        error_output="$make_output"
    fi
    cd -
    
    # remove new lines from error output
    error_output=$(echo "$error_output" | tr -d \\n)
    echo "$error_output" > "$current_submission_repo_dir/err.log"
    output+=("$current_submission_repo_dir$delimiter$zip_file$delimiter$commit_user_name$delimiter$test_result$delimiter$error_output")
done

# output results to file and stdout
[ -n "$single_submission" ] && result_file="$submission_repos_root/$single_submission-result.csv" || result_file="$submission_repos_root/result.csv"
> "$result_file"
echo ""
echo "Results:"
for line in "${output[@]}"; do
    echo -e "$line"
    echo -e "$line" >> "$result_file"
done
echo "results have also been written to $result_file"
