#!/bin/bash

# Emilio Scott, Paris Kalathas
# Date created: 2026/11/09
# compile.sh
# compiles the filesystem into a.out, then run it with ./a.out
set -euo pipefail

readonly SOURCE='filesystem.c'
readonly OUTPUT='a.out'

# c99 is the standard the filesystem is written for, and -Wall shows the
# common warnings so problems are caught before the program is run
readonly -a FLAGS=(-std=c99 -Wall)

# move to the folder this script is in, so it works from any directory
cd "$(dirname "$0")"

# check the source file exists and is readable before compiling
if [[ ! -r "$SOURCE" ]]; then
	echo "compile.sh: cannot read $SOURCE" >&2
	exit 1
fi

gcc "${FLAGS[@]}" "$SOURCE" -o "$OUTPUT"
echo "compiled $SOURCE into $OUTPUT"
