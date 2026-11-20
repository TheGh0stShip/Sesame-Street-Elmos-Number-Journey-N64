#!/bin/bash
# run.sh STEP : runs one r5 tool (call through job.sh). Paths are the lab's; ROMs are read only.
L=${N64DK_LAB}
LR="$L/roms/Sesame Street - Elmo's Letter Adventure (USA).z64"
NR="$L/roms/Sesame Street - Elmo's Number Journey (USA).z64"
PY="$L/venv/bin/python -I"
T=$L/sesassets/r5/tools
O=$L/sesassets/r5/out
FJ=$L/sestools/functions.json
SRC=$L/sesverify/v16
FD=$L/sesassets/files
mkdir -p $O
case "$1" in
  graph)   $PY $T/callgraph.py "$LR" "$NR" $FJ $SRC $FD $O ;;
  strings) $PY $T/strings.py "$LR" "$NR" $FJ $FD $L/sesassets/ledger $L/sesassets/data1,$L/sesassets/data2 $O ;;
  diff)    $PY $T/gamediff.py "$LR" "$NR" $FJ $SRC $O ;;
  unused)  $PY $T/unused.py "$LR" "$NR" $FJ $L/sesassets $O ;;
  docs)    $PY $T/textdiff.py $O && $PY $T/mkdocs.py $L/sesassets/r5 ;;
  debug)   $PY $T/debug.py $O $FJ $SRC ;;
  xref)    G=$2; shift 2; if [ $G = letter ]; then R="$LR"; else R="$NR"; fi; $PY $T/xref.py "$R" $FJ $FD "$@" ;;
  *) $PY "$@" ;;
esac
