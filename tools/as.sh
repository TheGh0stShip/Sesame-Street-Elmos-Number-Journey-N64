#!/bin/sh
# as.sh LANE SOURCE.s OBJECT : assemble one original assembly unit: C preprocessor (include/asm.h macros and
# register names), then the assembler of its lane.
set -e
LANE=$1; SRC=$2; OBJ=$3; I=${OBJ%.o}.i.s
"$KMC/gcc" -B"$KMC/" -x assembler-with-cpp -E -P -Iinclude "$SRC" > "$I"
case "$LANE" in
kmc+as|kmc+builtin) "$KMC/as" -mips3 -G0 -o "$OBJ" "$I" ;;
kmc+asn64) "$PYTHON" tools/asmfilter.py asn64 "$I" "${OBJ%.o}.s"
           LD_LIBRARY_PATH="$BINUTILS/lib/x86_64-linux-gnu" "$BINUTILS/bin/mips-linux-gnu-as" -march=vr4300 -mabi=32 -EB -G0 --no-pad-sections -mips3 -o "$OBJ" "${OBJ%.o}.s" ;;
*) echo "unknown lane $LANE" >&2; exit 1 ;;
esac
