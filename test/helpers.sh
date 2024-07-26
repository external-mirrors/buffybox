#!/bin/bash

function run_unl0kr_async() {
    local log=$1
    local conf=$2

    ./_build/unl0kr/unl0kr -v -C "$conf" > "$log" 2>&1 &
    pid=$!
    sleep 3

    kill -9 $pid
    wait $pid > /dev/null 2>&1
}

function run_unl0kr_sync() {
    local log=$1
    shift
    local conf=$2
    shift
    local args=$@

    ./_build/unl0kr/unl0kr -v -C "$conf" $@ > "$log" 2>&1
}

function run_buffyboard_async() {
    local log=$1
    local conf=$2

    ./_build/buffyboard/buffyboard -v -C "$conf" > "$log" 2>&1 &
    pid=$!
    sleep 3

    kill -9 $pid
    wait $pid > /dev/null 2>&1
}

function run_buffyboard_sync() {
    local log=$1
    shift
    local conf=$2
    shift
    local args=$@

    ./_build/buffyboard/buffyboard -v -C "$conf" $@ > "$log" 2>&1
}

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
    grep "^## [[:digit:]]" "$root/../CHANGELOG.md" | head -n1 | grep -o "[[:digit:]]\+\.[[:digit:]]\+\.[[:digit:]]\+"
}
