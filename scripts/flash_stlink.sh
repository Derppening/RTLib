#!/bin/bash

help() {
    echo "Usage: $0 [file.bin]"
}

if [[ $# -eq 1 ]] && [[ "$1" == "--help" ]]; then
    help
    exit
fi

if [[ $# -ne 1 ]]; then
    echo "Invalid number of arguments."
    help
    exit
fi

st-flash --reset write $1 0x8000000
