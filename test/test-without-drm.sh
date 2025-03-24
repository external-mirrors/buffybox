#!/bin/bash

root=$(dirname "${BASH_SOURCE[0]}")

source "$root/helpers.sh"

rc=0

run_script "$root/build-without-drm.sh" || rc=1
run_script "$root/test-version-matches-meson-and-changelog.sh" || rc=1
run_script "$root/test-uses-fb-backend-by-default.sh" || rc=1
run_script "$root/test-uses-fb-backend-if-selected-via-config.sh" || rc=1
run_script "$root/test-uses-fb-backend-if-drm-selected-via-config-but-unavailable.sh" || rc=1

exit $rc
