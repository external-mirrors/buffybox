#!/bin/bash

log=tmp.log
conf=tmp.conf

source "$(dirname "${BASH_SOURCE[0]}")/helpers.sh"

function clean_up() {
    rm -f "$log" "$conf"
}

trap clean_up EXIT

info "Writing config"
cat << EOF > "$conf"
[general]
backend=fb
EOF

info "Running unl0kr"
run_unl0kr_async "$log" "$conf"

info "Verifying output"
if ! grep "Using framebuffer backend" "$log"; then
    error "Expected framebuffer backend to be selected"
    cat "$log"
    exit 1
fi

ok
