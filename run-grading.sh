#!/bin/bash

abort() {
    echo "$1"
    echo "aborting..."
    exit 1
}

# assert pwd to be repository root
project_root="$PWD"
[[ "$project_root" == *assignment-test ]] || abort "please run this script from the repository root"
handin_repos_root="$project_root/handins"

source ./assignment.conf

current_handin=0
handin_zips=()
for filename in $handin_zips_location/*; do
    [[ "$filename" == *.zip ]] && handin_zips+=("$filename")
done
echo "processing ${#handin_zips[@]} handins"

mkdir -p "$handin_repos_root"
for zip_file in "${handin_zips[@]}"; do
    echo "processing handin $current_handin associated with $zip_file"
    current_handin_repo_dir="$handin_repos_root/$current_handin"
    ((current_handin++))

    echo "moving zip contents to $current_handin_repo_dir"
    mkdir -p "$current_handin_repo_dir"
    unzip "$zip_file" -d "$current_handin_repo_dir" > /dev/null
    [ $? -eq 0 ] || abort "error during unzipping of handin"

    cd "$current_handin_repo_dir" && commit_user_name="$(git log -1 | grep Author | cut -d "<" -f 1 | cut -c 9-)" && cd - || abort "unable to extract author from most recent commit"
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


