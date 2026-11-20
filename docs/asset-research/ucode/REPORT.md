# REPORT: header, IPL3, RSP microcode, call graph (agent ucode)

## Done
| class | letter bytes | number bytes |
|---|---|---|
| rom_metadata (header) | 64 | 64 |
| boot (IPL3) | 4032 | 4032 |
| rsp_ucode text (boot 208 + F3DEX 5168 + audio 3156) | 8532 | 8532 |
| rsp_ucode data (F3DEX 2048 + audio 688) | 2736 | 2736 |
| pad | 12 | 12 |
Every range is in MAP.tsv; tools: `tools/check_header.py ROM`, `tools/extract_ucode.py ROM OUTDIR [--check]`,
`tools/callgraph.py` (all run through job.sh; driver: `out/runall.sh`).

- Header: both are SDK 2.0I builds, entry 0x80000400, region E, revision 0, reserved fields zero. Field meanings in FORMAT.md.
- IPL3: standard **CIC-NUS-6102/7101**, identical in both games. CRC1/CRC2 recomputed with the 6102 algorithm: **MATCH** in both.
- Microcode: rspboot, **gspF3DEX 1.23** (id string in its data) and one audio microcode (task type 2, built by the libmus
  audio frame loop). All five sections are **byte-identical between the two games** (same sha1 per section, see
  out/ucode_*/ucode.json). Exact boundaries: FORMAT.md / MAP.tsv.
- SDK comparison: **not possible**: ${N64DK_LAB}/sdk_src holds only ultralib sources and headers, no rspboot / gspF3DEX /
  aspMain objects or dumps. So "identical to a known SDK release" is unverified; the kit survey matched the libultra
  code itself to 2.0D-2.0I, and the header says 2.0I. The audio microcode's family (aspMain vs n_aspMain) is also
  unconfirmed: 0xC54 text / 0x2B0 data, no id string. The 0xC54 audio text ends on a non-16-byte boundary, followed by 12 zero bytes.
- The kit's RSP ranges (letter 0x8006B780.., etc.) were heuristic pieces; the real blobs start earlier (boot at
  letter 0x8006B390, number 0x8006CD00) and the audio text runs past the survey's .text end into what it called .data.

## Call graph (out/callgraph_<game>.tsv)
Built from machine code: jal and j targets, lui+addiu/ori address loads, and function addresses stored as words in
.data/.rodata. letter 962 functions, number 971.

Unreferenced (no call, no address load, no data pointer):
| | letter | number |
|---|---|---|
| game | 123 (28796 B) | 125 (33972 B) |
| sdk | 26 (6880 B) | 26 (6880 B) |
| handwritten | 1 (32 B) | 1 (32 B) |
Caveats: a few are entered by a branch from the neighbouring function rather than a call (for example the 64-bit compare tail
func_80068C20 in letter), or are reached through addresses built in ways the scanner misses (for example through a
pointer table in .bss filled at runtime). Treat the list as candidates. Most look like real leftovers: unused libultra
pieces (guLookAt, guScale, alInit, osLeoDiskInit, __osLeoInterrupt, _frexpf, osSyncPrintf), unused libmus API stubs
(the run of tiny functions near letter 0x8004D2xx-0x8004D5xx), unused game helpers (cyclic-table randomisers,
object/particle spawners, a Catmull-Rom path routine, frame-timing statistics func_8000D0CC, camera code).
letter: func_80000CA8 func_80000E50 func_8000125C func_80001BF8 func_8000C3A0 func_8000C458 func_8000C588 func_8000C62C func_8000C744 func_8000C998 func_8000CA50 func_8000CB14 func_8000CCDC func_8000CDAC func_8000CE6C func_8000D060 func_8000D0CC func_8000D658 func_8000D690 func_8000D6C8 func_8000D6F0 func_8000D718 func_8000D74C func_8000D774 func_8000D7A8 func_8000D7E0 func_8000DDB8 func_8000E2D4 func_8000E310 func_8000E3EC func_8000E4DC func_8000E5C4 func_8000E6B4 func_8000F948 func_800108E4 func_80010FF8 func_80016D80 func_80017050 func_800172B8 func_80017408 func_8001747C func_800174E4 func_800175D0 func_80017658 func_800176A4 func_800176F0 func_8001773C func_800184AC func_800186A8 func_8001D1BC func_8001DFD0 func_8001E718 func_8001EA68 func_8001EE68 func_8001F184 func_8001F248 func_8001FD28 func_80020F60 func_80022B18 func_80023080 func_80024820 func_80025A00 func_80029EFC func_8002A07C func_8002A0C8 func_80030198 func_800301C8 func_8003054C func_800307D4 func_800316BC func_80035720 func_8003F140 func_80040874 func_800408C4 func_80041090 func_800413A0 func_80041E78 func_80041F60 func_8004216C func_80042270 func_800445A0 func_80044840 func_800477C0 func_80047E10 func_80048120 func_80049994 func_8004C500 func_8004C58C func_8004C9E0 func_8004CE94 func_8004D160 func_8004D2D8 func_8004D2FC func_8004D308 func_8004D364 func_8004D3E0 func_8004D4C0 func_8004D4C8 func_8004D4D4 func_8004D4E0 func_8004D4EC func_8004D4F4 func_8004D4FC func_8004D508 func_8004D56C func_8004D5E4 func_80051474 func_8005364C func_80053980 func_80054060 func_800540A4 func_80055200 func_80055720 func_80055A14 func_80055C50 func_80055F7C func_80056100 func_80056480 func_800566A0 func_800570F0 func_800598F8 func_800599D8 func_8005ADB0 func_8005ADDC func_8005B1FC func_8005B3AC func_8005B4CC func_8005B6F0 func_8005B704 func_8005E860 func_8005F86C func_8005FAC0 func_8005FF14 func_80060F3C func_80061CF0 func_80062F70 func_800648AC func_80067C38 func_80068204 func_80068280 func_800683A0 func_800687A8 func_80068C20 func_80068C54 func_80068F00 func_80069240 func_80069908 func_80069C00 func_8006ADA4 func_8006AEB0 

number: func_80000CA8 func_80000E50 func_8000125C func_80001BF8 func_8000C810 func_8000C8C8 func_8000C9F8 func_8000CA9C func_8000CBB4 func_8000CE08 func_8000CEC0 func_8000CF84 func_8000D14C func_8000D21C func_8000D2DC func_8000D4D0 func_8000D53C func_8000DAC8 func_8000DB00 func_8000DB38 func_8000DB60 func_8000DB88 func_8000DBBC func_8000DBE4 func_8000DC18 func_8000DC50 func_8000E228 func_8000E744 func_8000E780 func_8000E85C func_8000E94C func_8000EA34 func_8000EB24 func_8000FDB8 func_800106BC func_80011468 func_800171F0 func_800174DC func_80017728 func_80017878 func_800178EC func_80017954 func_80017A40 func_80017AC8 func_80017B14 func_80017B60 func_80017BAC func_8001891C func_80018B18 func_8001DAC0 func_8001E8B0 func_8001EFF8 func_8001F348 func_8001F748 func_8001FA64 func_8001FB28 func_80020638 func_80021870 func_80023440 func_800239A0 func_80026D60 func_8002C5D8 func_8002C758 func_8002C7A4 func_8002F78C func_80032774 func_800327A4 func_800336AC func_80033850 func_800374A0 func_8003BC9C func_8003BD24 func_8003EFE0 func_8003FC40 func_800424F4 func_80042544 func_80042D10 func_80043020 func_80043AF8 func_80043BE0 func_80043DEC func_80043EF0 func_80046220 func_800464C0 func_80049180 func_800497D0 func_80049AE0 func_8004B354 func_8004DE70 func_8004DEFC func_8004E350 func_8004E804 func_8004EAD0 func_8004EC48 func_8004EC6C func_8004EC78 func_8004ECD4 func_8004ED50 func_8004EE30 func_8004EE38 func_8004EE44 func_8004EE50 func_8004EE5C func_8004EE64 func_8004EE6C func_8004EE78 func_8004EEDC func_8004EF54 func_80052DE4 func_80054FBC func_800552F0 func_800559D0 func_80055A14 func_80056B70 func_80057090 func_80057384 func_800575C0 func_800578EC func_80057A70 func_80057DF0 func_80058010 func_80058A60 func_8005B268 func_8005B348 func_8005C720 func_8005C74C func_8005CB6C func_8005CD1C func_8005CE3C func_8005D060 func_8005D074 func_800601D0 func_800611DC func_80061430 func_80061884 func_800628AC func_80063660 func_800648E0 func_8006621C func_800695A8 func_80069B74 func_80069BF0 func_80069D10 func_8006A118 func_8006A590 func_8006A5C4 func_8006A870 func_8006ABB0 func_8006B278 func_8006B570 func_8006C714 func_8006C820 

## Debug / development leftovers
- Both games link the **rmon debugger and kdebugserver/rdb** libultra code, and the boot thread (letter func_8000BB84,
  number func_8000C0A8) starts the rdb and rmon threads. This is a development libultra configuration that shipped.
  The rmon set (__rmonGet/Set{G,F,S,V}Regs, __rmonStopThread/RunThread/ListThreads/SetSingleStep, __rmonSendReply) is
  referenced: it is reached from the rmon thread.
- __osInitialize_common has the KMC debugger hook; __osTimerInterrupt has the profiler sampling.
- osSyncPrintf (letter func_8005F86C, number func_800611DC) is linked but never called. A printf-style sink
  (letter func_8005F808, number func_80061178) and sprintf are used.
- **On-screen debug overlay** func_80001F60 (both games) prints pages of debug values into a texture. It is referenced.
  The .rodata debug strings next to the audio ucode data (key-timing and display-list-overflow messages)
  are error/assert messages. Assert-style calls (letter func_80057BD0 with file/line 0x1324 in func_8000B470) remain.

## Differences between the games (by function hash, sestools/functions.json)
letter has 86 functions (139292 B) with no identical copy in number; number has 87 (144212 B) with none in letter.
These are game logic (84 / 85), plus the boot entry and one handwritten routine at shifted addresses. All 288 SDK
functions have identical copies in both games. Microcode, IPL3 and header layout are identical except the name, id and CRCs.

## Odd
- number's matched sources reuse letter symbol names for some libmus data (number func_80052530 declares D_8006B390,
  which is letter's rspboot address; number's rspboot is at 0x8006CD00). The bytes match because of relocations, but the
  names are wrong for number.
- Audio task ucode_data_size says 0x800 though the audio data is 0x2B0, so the DMA also pulls 0x550 bytes of game strings into DMEM.
- number's audio data end (0x2B0) is taken to be the same as letter's, because the sha1 is identical. Strings were checked to follow in letter only.
