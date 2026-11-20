#!/bin/bash
# run every g2 tool in extract then check mode for both games (call through job.sh)
cd "$(dirname "$0")/.." || exit 1
PY=${N64DK_LAB}/venv/bin/python
for g in letter number; do
  case $g in letter) R="../../roms/Sesame Street - Elmo's Letter Adventure (USA).z64";; *) R="../../roms/Sesame Street - Elmo's Number Journey (USA).z64";; esac
  for m in ${MODES:-extract check}; do
    $PY tools/boot_g2.py $m "$R" out/boot || exit 1
    $PY tools/rsp_g2.py $m "$R" ../ucode/out/ucode_$g/ucode.json out/rsp || exit 1
    $PY tools/tex_g2.py $m "$R" $g ../r4/out/REBUILD_$g.tsv out/tex || exit 1
    $PY tools/leftover_g2.py $m "$R" $g ../r4/out/REBUILD_$g.tsv out/leftover || exit 1
    $PY tools/adpcm_g2.py $m "$R" ../audio/out out/adpcm || exit 1
  done
done
$PY tools/map_g2.py . ../audio/out ../ucode/out
