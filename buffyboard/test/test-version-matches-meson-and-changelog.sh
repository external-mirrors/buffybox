#!/bin/bash

log=tmp.log

root=$(dirname "${BASH_SOURCE[0]}")

source "$root/helpers.sh"

function clean_up() {
    rm -f "$log"
}

trap clean_up EXIT

info "Querying version from build.meson"
meson_version=$(read_version_from_meson)

info "Querying version from CHANGELOG.md"
changelog_version=$(read_version_from_changelog)

info "Verifying versions"
if [[ "$meson_version" != "$changelog_version" ]]; then
    error "Version $meson_version from meson.build doesn't match version $changelog_version from CHANGELOG.md"
    exit 1
fi

info "Running buffyboard"
run_buffyboard_sync "$log" "$conf" -V

info "Verifying output"
if ! grep "buffyboard $meson_version" "$log"; then
    error "Expected version $meson_version"
    cat "$log"
    exit 1
fi

ok
