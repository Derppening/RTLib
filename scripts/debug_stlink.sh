#!/bin/bash

help() {
    echo "Usage: $0"
}

if [[ $# -eq 1 ]] && [[ "$1" == "--help" ]]; then
    help
    exit
fi

if [[ $# -ne 0 ]]; then
    echo "Invalid number of arguments."
    help
    exit
fi

st-util
