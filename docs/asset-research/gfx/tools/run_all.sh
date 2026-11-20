#!/bin/bash
# run_all.sh OUTDIR : regenerate code facts, extract both games, check both

T=$(dirname "$0"); PY=${N64DK_LAB}/venv/bin/python
R=${N64DK_LAB}/roms
for g in letter number; do $PY -I $T/gen_tables.py ${N64DK_LAB}/sesverify/v8L/$g/src $g $T/gfx_$g.json; done
L="$R/Sesame Street - Elmo's Letter Adventure (USA).z64"; N="$R/Sesame Street - Elmo's Number Journey (USA).z64"
$PY -I $T/extract_gfx.py "$L" "$1"; $PY -I $T/extract_gfx.py "$N" "$1"
$PY -I $T/extract_gfx.py check "$L" "$1"; $PY -I $T/extract_gfx.py check "$N" "$1"
