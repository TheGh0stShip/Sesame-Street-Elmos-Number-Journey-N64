#!/bin/bash
# run.sh: extract + check both games. usage: run.sh
PY=${N64DK_LAB}/venv/bin/python; T=${N64DK_LAB}/sesassets/packC/tools; O=${N64DK_LAB}/sesassets/packC/out
L="${N64DK_LAB}/roms/Sesame Street - Elmo's Letter Adventure (USA).z64"; N="${N64DK_LAB}/roms/Sesame Street - Elmo's Number Journey (USA).z64"
for g in letter number; do R="$L"; [ $g = number ] && R="$N"; rm -rf "$O/$g"; $PY -I $T/extract_packC.py "$R" $g "$O/$g" && $PY -I $T/extract_packC.py check "$R" $g "$O/$g"; done
( printf 'game\trom_start\trom_end\tclass\tformat\tdescription\treferenced_by\textractor\n'; cat $O/letter/MAP_letter.tsv $O/number/MAP_number.tsv ) > $O/../MAP.tsv
