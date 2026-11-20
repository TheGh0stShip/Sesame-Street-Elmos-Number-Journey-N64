#!/bin/bash
P=${N64DK_LAB}/venv/bin/python; T=${N64DK_LAB}/sesassets/packA
L="${N64DK_LAB}/roms/Sesame Street - Elmo's Letter Adventure (USA).z64"
N="${N64DK_LAB}/roms/Sesame Street - Elmo's Number Journey (USA).z64"
for g in letter number; do R="$L"; [ $g = number ] && R="$N"
 $P $T/tools/extract_packA.py "$R" $g $T/out/$g && $P $T/tools/extract_packA.py check "$R" $g $T/out/$g; done
