#!/usr/bin/env bash

cat "$1" | grep Sequence | cut -d ' ' -f 4-12 | cut -d ';' -f 1 | sort | uniq -cd
