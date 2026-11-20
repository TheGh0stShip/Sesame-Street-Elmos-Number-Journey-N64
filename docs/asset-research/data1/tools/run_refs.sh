#!/bin/bash
set -e
D=${N64DK_LAB}/sesassets/data1; V=${N64DK_LAB}/sesverify/v8L; P=${N64DK_LAB}/venv/bin/python
L="${N64DK_LAB}/roms/Sesame Street - Elmo's Letter Adventure (USA).z64"
N="${N64DK_LAB}/roms/Sesame Street - Elmo's Number Journey (USA).z64"
#$P -I $D/tools/refs.py "$L" letter $V/letter/VERIFY.tsv 8000043C 800785B0 $D/out/refs_letter.tsv
#$P -I $D/tools/refs.py "$N" number $V/number/VERIFY.tsv 8000043C 80078DC0 $D/out/refs_number.tsv
$P -I $D/tools/mk_number_types.py $D/tools/types_letter.tsv $D/out/refs_number.tsv $D/tools/types_number.tsv
