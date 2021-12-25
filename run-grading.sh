#!/bin/bash

abort() {
    echo "$1"
    echo "aborting..."
    exit 1
}

# assert pwd to be repository root
project_root="$PWD"
[[ "$project_root" == *assignment-test ]] || abort "please run this script from the repository root"
submission_repos_root="$project_root/submissions"
build_root="$project_root/build"

source ./assignment.conf

# setup wraps
wrap_cmake_file="$submission_repos_root/wraps.cmake"
echo "generating compiler options for function wrapping"
echo "# automatically generated wrap compiler options based on assignment.conf, which should be edited instead of this file" > "$wrap_cmake_file"
linker_flags="-Wl"
for func in "${wrap_functions[@]}"; do
    echo "adding wrapping for $func"
    linker_flags="$linker_flags,--wrap=$func"
done
echo "set(BIN_COMPILE_OPTIONS \${BIN_COMPILE_OPTIONS} $linker_flags)" >> "$wrap_cmake_file"

# gather all available zip files
submission_zips=()
for filename in $submission_zips_location/*; do
    [[ "$filename" == *.zip ]] && submission_zips+=("$filename")
done
echo "processing ${#submission_zips[@]} submissions"

# loop over all zips
current_submission=0
mkdir -p "$submission_repos_root"
for zip_file in "${submission_zips[@]}"; do
    echo "processing submission $current_submission associated with $zip_file"
    current_submission_repo_dir="$submission_repos_root/$current_submission"
    ((current_submission++))

    echo "moving zip contents to $current_submission_repo_dir"
    mkdir -p "$current_submission_repo_dir"
    unzip "$zip_file" -d "$current_submission_repo_dir" > /dev/null
    [ $? -eq 0 ] || abort "error during unzipping of submission"

    cd "$current_submission_repo_dir" && commit_user_name="$(git log -1 | grep Author | cut -d "<" -f 1 | cut -c 9-)" && cd - || abort "unable to extract author from most recent commit"
    echo "identified user $commit_user_name based on latest commit user"
done

calculate_percentage_from_test_result() {
    local num_correct_tests=$(($max_points - $1))
    return $((num_correct_tests * 100 / $max_points))
}

#echo "$max_points"
#echo "${wrap_functions[1]}"
#echo "${assignment_source_files[0]}"

run_tests() {
    test_err_output="$(./out/assignment-test 2>&1 > /dev/null)"
    calculate_percentage_from_test_result $?
    echo "$?"
}


