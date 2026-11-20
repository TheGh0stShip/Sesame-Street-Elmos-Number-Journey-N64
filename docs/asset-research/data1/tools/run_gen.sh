#!/bin/bash
# run_gen.sh GAME... : generate + check
D=${N64DK_LAB}/sesassets/data1; P=${N64DK_LAB}/venv/bin/python
for g in "$@"; do
  case $g in letter) R="${N64DK_LAB}/roms/Sesame Street - Elmo's Letter Adventure (USA).z64";; number) R="${N64DK_LAB}/roms/Sesame Street - Elmo's Number Journey (USA).z64";; esac
  $P -I $D/tools/gen_data.py "$R" $g $D/out && $P -I $D/tools/gen_data.py check "$R" $g $D/out
done
