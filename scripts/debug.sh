#!/bin/bash

help() {
    echo "Usage: $0 [device]"
}

if [[ $# -ne 1 ]]; then
    echo "Invalid number of arguments."
    help
    exit
fi

if [[ "$1" == "--help" ]]; then
    help
    exit
fi

# change these parameters as necessary
JLinkGDBServer -device $1
