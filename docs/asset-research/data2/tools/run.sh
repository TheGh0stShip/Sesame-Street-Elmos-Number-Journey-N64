#!/bin/bash
# run.sh STEP : drives the data2 tools for both games (ROM paths resolved here)
P=${N64DK_LAB}; D=$P/sesassets/data2; PY="$P/venv/bin/python -I"
rom() { if [ $1 = letter ]; then echo "$P/roms/Sesame Street - Elmo's Letter Adventure (USA).z64"; else echo "$P/roms/Sesame Street - Elmo's Number Journey (USA).z64"; fi; }
for g in letter number; do
  case $1 in
    scan) $PY $D/tools/scan_refs.py "$(rom $g)" $g $P/sesverify/v8L/$g/VERIFY.tsv $D/out/refs_$g.tsv ;;
    draft) $PY $D/tools/draft_types.py "$(rom $g)" $g $D/out/refs_$g.tsv $D/tools/types_$g.tsv ;;
    gen) $PY $D/tools/gen_data.py "$(rom $g)" $g $D/out/c ;;
    check) $PY $D/tools/gen_data.py check "$(rom $g)" $g $D/out/c ;;
    strings) $PY $D/tools/gen_data.py strings "$(rom $g)" $g $D/out ;;
  esac
done
