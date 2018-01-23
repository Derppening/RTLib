#!/bin/bash

# This file is part of RTLib.
#
# Copyright (C) 2017 Derppening <david.18.19.21@gmail.com>
#
# RTLib is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# RTLib is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with RTLib.  If not, see <http://www.gnu.org/licenses/>.

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
