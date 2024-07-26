#!/bin/bash

root=$(dirname "${BASH_SOURCE[0]}")

source "$root/helpers.sh"

run_script "$root/build-with-drm.sh"
run_script "$root/test-version-matches-meson-and-changelog.sh"
run_script "$root/test-uses-fb-backend-by-default.sh"
run_script "$root/test-uses-fb-backend-if-selected-via-config.sh"
run_script "$root/test-uses-drm-backend-if-selected-via-config-and-available.sh"
