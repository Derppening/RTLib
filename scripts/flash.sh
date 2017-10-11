#!/bin/bash

help() {
    echo "Usage: $0 [device] [script]"
}

if [[ $# -eq 1 ]] && [[ "$1" == "--help" ]]; then
    help
    exit
fi

if [[ $# -ne 2 ]]; then
    echo "Invalid number of arguments."
    help
    exit
fi

# change these parameters as necessary
JLinkExe -if JTAG -device $1 -speed auto -jtagconf -1,-1 -autoconnect 1 -CommanderScript $2
