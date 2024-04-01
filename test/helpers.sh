#!/bin/bash

function info() {
    echo -e "[Info] $1"
}

function error() {
    echo -e "\e[31m[Error]\e[0m $1"
}

function ok() {
    echo -e "\e[32m[Ok]\e[0m $1"
}

function run_script() {
    info "Executing $1"
    "$1"
    echo
}

function read_version_from_meson() {
    grep "^[[:space:]]*version:" "$root/../meson.build" | head -n1 | grep -o "[[:digit:]]\+\.[[:digit:]]\+\.[[:digit:]]\+"
}

function read_version_from_changelog() {
    grep "^## [[:digit:]]" "$root/../../CHANGELOG.md" | head -n1 | grep -o "[[:digit:]]\+\.[[:digit:]]\+\.[[:digit:]]\+"
}
