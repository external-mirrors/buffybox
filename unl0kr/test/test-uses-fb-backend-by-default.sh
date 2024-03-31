#!/bin/bash

log=tmp.log

source "$(dirname "${BASH_SOURCE[0]}")/helpers.sh"

function clean_up() {
    rm -f "$log"
}

trap clean_up EXIT

info "Running unl0kr"
run_unl0kr "$log"

info "Verifying output"
if ! grep "Using framebuffer backend" "$log"; then
    error "Expected framebuffer backend to be selected"
    cat "$log"
    exit 1
fi

ok
