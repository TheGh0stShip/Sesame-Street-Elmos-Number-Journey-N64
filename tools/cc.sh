#!/bin/sh
# cc.sh LANE "FLAGS" SOURCE OBJECT : compile one C source the way its lane did (tools/flags.tsv).
#   kmc+as      KMC GCC 2.7.2 -S  -> KMC GNU as -mips3 -G0
#   kmc+builtin KMC GCC 2.7.2 -S  -> tools/asmfilter.py builtin -> KMC GNU as
#   kmc+asn64   KMC GCC 2.7.2 -S  -> tools/asmfilter.py asn64 -> mips-linux-gnu-as (ASN64 behaviour)
#   ido5.3 / ido7.1  IDO cc -c
# Toolchain locations come from the Makefile (KMC, IDO53, IDO71, BINUTILS, PYTHON).
set -e
LANE=$1; FLAGS=$2; SRC=$3; OBJ=$4; INC="-Iinclude"
# library units include the SDK headers the way the SDK sources do (<os.h>, "synthInternals.h", <stdio.h>):
# the SDK build's include path (ultralib makefiles: include, include/compiler/<cc>, include/PR, the source's own dir)
case "$SRC" in
src/lib*|src/speech*) case "$LANE" in ido*) CCI=ido ;; *) CCI=gcc ;; esac
    INC="-Iinclude/compiler/$CCI -Iinclude/PR -Iinclude/libultra -Iinclude -D_LANGUAGE_C -D_MIPS_SZLONG=32 -DF3DEX_GBI -DNDEBUG"
    # libultra 2.0D-2.0J was built by gcc with -funsigned-char (ultralib makefiles/gcc.mk): the SDK's plain `char`
    case "$SRC:$LANE" in src/libultra/*:kmc*) INC="$INC -funsigned-char" ;; esac ;;
esac
# game units (src/game below the first library address: Letter 8004C500, Number 8004DE70) include the SDK headers
# too (common.h includes <ultra64.h> under GAME_SDK); no -funsigned-char: the game's plain char is signed
case "$SRC" in
src/game/func_*.c) A=${SRC#src/game/func_}; A=${A%.c}
    case "$(sed -n 's/^GAME := //p' game.mk)" in letter) END=8004C500 ;; *) END=8004DE70 ;; esac
    if [ "$(printf '%s\n%s\n' "$A" "$END" | LC_ALL=C sort | head -1)" = "$A" ] && [ "$A" != "$END" ]; then
        INC="-Iinclude/compiler/gcc -Iinclude/PR -Iinclude/libultra -Iinclude -D_LANGUAGE_C -D_MIPS_SZLONG=32 -DF3DEX_GBI -DNDEBUG -DGAME_SDK"
        [ "$END" = 8004DE70 ] && INC="$INC -DGAME_NUMBER"
    fi ;;
esac
S=${OBJ%.o}.s
case "$LANE" in
kmc+*)
    TMPDIR=$(dirname "$OBJ") "$KMC/gcc" -B"$KMC/" -x c -S $FLAGS $INC -o "$S.raw" "$SRC"
    case "$LANE" in
    kmc+as)      cp "$S.raw" "$S"; "$KMC/as" -mips3 -G0 -o "$OBJ" "$S" ;;
    kmc+builtin) "$PYTHON" tools/asmfilter.py builtin "$S.raw" "$S"; "$KMC/as" -mips3 -G0 -o "$OBJ" "$S" ;;
    kmc+asn64)   "$PYTHON" tools/asmfilter.py asn64 "$S.raw" "$S"
                 LD_LIBRARY_PATH="$BINUTILS/lib/x86_64-linux-gnu" "$BINUTILS/bin/mips-linux-gnu-as" -march=vr4300 -mabi=32 -EB -G0 --no-pad-sections -mips3 -o "$OBJ" "$S" ;;
    *) echo "unknown lane $LANE" >&2; exit 1 ;;
    esac ;;
ido5.3) TMPDIR=$(dirname "$OBJ") "$IDO53/cc" -c $FLAGS $INC -o "$OBJ" "$SRC" ;;
ido7.1) TMPDIR=$(dirname "$OBJ") "$IDO71/cc" -c $FLAGS $INC -o "$OBJ" "$SRC" ;;
*) echo "unknown lane $LANE" >&2; exit 1 ;;
esac
