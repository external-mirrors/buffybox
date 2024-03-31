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

function run_unl0kr() {
    local log=$1
    local conf=$2

    ./_build/unl0kr -v -C "$conf" > "$log" 2>&1 &
    pid=$!
    sleep 3

    kill -9 $pid
    wait $pid > /dev/null 2>&1
}
