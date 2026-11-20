#!/bin/bash
# re-extract and check every morph-anim tool for both games; output dirs: agent dir for its own game(s), fix/out otherwise
S=${N64DK_LAB}/sesassets; P="${N64DK_LAB}/venv/bin/python -I"
R=${N64DK_LAB}/roms
L="$R/Sesame Street - Elmo's Letter Adventure (USA).z64"; N="$R/Sesame Street - Elmo's Number Journey (USA).z64"
F=$S/fix/out; mkdir -p $F
run() { echo "### $1"; shift; "$@" > $F/last.log 2>&1; rc=$?; tail -4 $F/last.log; echo "rc=$rc"; }
for g in letter number; do
  [ $g = letter ] && ROM="$L" || ROM="$N"
  run "sprA ext $g" $P $S/sprA/tools/extract_sprites.py "$ROM" $g $S/sprA/out
  run "sprA check $g" $P $S/sprA/tools/extract_sprites.py "$ROM" $g $S/sprA/out check
  run "lvlD ext $g" $P $S/lvlD/tools/extract_level_misc.py "$ROM" $g $S/lvlD/out/$g
  run "lvlD check $g" $P $S/lvlD/tools/extract_level_misc.py check "$ROM" $g $S/lvlD/out/$g
  run "hud ext $g" $P $S/hud/tools/extract_hud.py "$ROM" $g $S/hud/out
  run "hud check $g" $P $S/hud/tools/extract_hud.py check "$ROM" $g $S/hud/out
  if [ $g = letter ]; then E=$S/lvlE/out; FF=$S/lvlF/out; G=$F/lvlG; H=$F/lvlH; else E=$F/lvlE; FF=$F/lvlF; G=$S/lvlG/out; H=$S/lvlH/out; fi
  run "lvlE ext $g" $P $S/lvlE/tools/extract_level_rest.py "$ROM" $g $E
  run "lvlE check $g" $P $S/lvlE/tools/extract_level_rest.py check "$ROM" $g $E
  run "lvlF ext $g" $P $S/lvlF/tools/extract_level_rest.py "$ROM" $g $FF
  run "lvlF check $g" $P $S/lvlF/tools/extract_level_rest.py "$ROM" $g $FF check
  run "lvlG ext $g" $P $S/lvlG/tools/extract_level_rest.py "$ROM" $g $G extract
  run "lvlG check $g" $P $S/lvlG/tools/extract_level_rest.py "$ROM" $g $G check
  run "lvlH ext $g" $P $S/lvlH/tools/extract_level_rest.py "$ROM" $g $H
  run "lvlH check $g" $P $S/lvlH/tools/extract_level_rest.py "$ROM" $g $H check
done
for g in letter number; do [ $g = letter ] && ROM="$L" || ROM="$N"
  run "gfx ext $g" $P $S/gfx/tools/extract_gfx.py "$ROM" $S/gfx/out
  run "gfx check $g" $P $S/gfx/tools/extract_gfx.py check "$ROM" $S/gfx/out
done
