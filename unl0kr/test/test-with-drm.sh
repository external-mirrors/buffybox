#!/bin/bash

root=$(dirname "${BASH_SOURCE[0]}")

source "$root/helpers.sh"

function run() {
    info "Executing $1"
    "$1"
    echo
}

run "$root/build-with-drm.sh"
run "$root/test-uses-fb-backend-by-default.sh"
run "$root/test-uses-fb-backend-if-selected-via-config.sh"
run "$root/test-uses-drm-backend-if-selected-via-config-and-available.sh"
