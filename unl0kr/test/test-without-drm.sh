#!/bin/bash

root=$(dirname "${BASH_SOURCE[0]}")

source "$root/helpers.sh"

run_script "$root/build-without-drm.sh"
run_script "$root/test-version-matches-meson-and-changelog.sh"
run_script "$root/test-uses-fb-backend-by-default.sh"
run_script "$root/test-uses-fb-backend-if-selected-via-config.sh"
run_script "$root/test-uses-fb-backend-if-drm-selected-via-config-but-unavailable.sh"
