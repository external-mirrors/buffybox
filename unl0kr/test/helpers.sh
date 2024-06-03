#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/../../test/helpers.sh"

function run_unl0kr_async() {
    local log=$1
    local conf=$2

    ./_build/unl0kr -v -C "$conf" > "$log" 2>&1 &
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

    ./_build/unl0kr -v -C "$conf" $@ > "$log" 2>&1
}