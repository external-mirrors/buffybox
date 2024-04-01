#!/bin/bash

root=$(dirname "${BASH_SOURCE[0]}")

source "$root/helpers.sh"

run_script "$root/build.sh"
run_script "$root/test-version-matches-meson-and-changelog.sh"
