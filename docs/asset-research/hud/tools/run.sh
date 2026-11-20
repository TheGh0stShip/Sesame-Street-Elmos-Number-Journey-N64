#!/bin/bash
# extract + check both games, build ../MAP.tsv (run through the lab job runner)
set -e
H=$(cd "$(dirname "$0")/.." && pwd); P=${N64DK_LAB}/venv/bin/python; R=${N64DK_LAB}/roms
L="$R/Sesame Street - Elmo's Letter Adventure (USA).z64"; N="$R/Sesame Street - Elmo's Number Journey (USA).z64"
rm -rf "$H/out/letter" "$H/out/number"
$P "$H/tools/extract_hud.py" "$L" letter "$H/out"; $P "$H/tools/extract_hud.py" check "$L" letter "$H/out"
$P "$H/tools/extract_hud.py" "$N" number "$H/out"; $P "$H/tools/extract_hud.py" check "$N" number "$H/out"
cat "$H/out/letter/MAP_letter.tsv" "$H/out/number/MAP_number.tsv" > "$H/MAP.tsv"
