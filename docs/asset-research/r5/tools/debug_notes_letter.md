# Debug and development material: Elmo's Letter Adventure

Generated part (the table at the end) comes from `tools/debug.py` (call graph `out/graph_letter.json` from
`tools/callgraph.py`, strings `TEXT_letter.tsv` from `tools/strings.py`). The narrative was checked against the
matched C in `sesverify/v16/letter/src` and the address cross-references from `tools/xref.py`.
"Reachable" means reachable from the entry point 0x80000400 through calls, branches, function addresses built in
code and function pointers stored in data (see REPORT.md for the method).

## Summary

| what | functions | reachable | how it is (or is not) switched on |
|---|---|---|---|
| libultra debugger: rmon | 52 | 51 (func_800648AC, a spin-forever stub, is not) | started unconditionally: the boot thread func_8000BB84 creates rmonMain func_8005F0D0 (priority 250); rmonMain calls func_800647DC, which binds OS events 10-13 (CPU break, SP break, faults) and starts the rdb thread func_8005F354. The threads only wake for a host debugger (rdb event 0x14, break/fault events), so on a retail console they block forever. The command dispatcher table at 0x800786A0 (.rodata) holds the rmon request handlers (registers, memory, threads, breakpoints) |
| rdb / kdebugserver | 4 | 4 | __osRdbSend func_80061AD0, buffer sender func_80061730, kdebugserver func_800618D8 (called from the exception handler func_80060544): talk to a development board over the RDB port; no effect without one |
| print paths | 7 | 6 | func_8005F808 is the game's printf: it formats with _Printf (func_80065FA0) and sends to the KMC debugger port (sink func_8005F5D0) when D_800A8B10 != 0, else to rdb (sink func_8005F8B4). D_800A8B10 is set only by __osInitialize_common func_80056DD0 when the word at 0xBFF00000 reads "KMC\0" (a Kyoto Microcomputer debugger cartridge). osSyncPrintf func_8005F86C is linked but never called |
| asserts and error messages | 27 callers | 25 reachable | __assert func_80057BD0 prints `\nASSERTION FAULT: %s, %d: "%s"\n` through func_8005F808 with the expression text "EX" and a source file name (code/gfx.c, audio/n64aud.c, actors/*.c, ../ratbgle/engine/collide/cs*.c). They run only when an internal check fails, and the text goes to the debugger port, never to the screen. Other messages: "Two keys with same frame #!", "Frame passed last key!" (animation sampler func_80000450 / func_8000092C), "addActorDL(): draw node list full!  Yow!" (func_80001424), "DrawWorld(): bsp stack blown (boom)" (func_80025E4C), the RTA_mail messages (func_80047CF8 / func_80047ED4 via func_80047F60) |
| on-screen debug overlay func_80001F60 | 1 | yes (called every frame by func_8000B470) | draws nothing unless the page byte D_8010686C is 1-4. The only store to D_8010686C in the program is `D_8010686C = 0` in the init function func_8000C164 (xref: loads at 0x80001F88/0x80002494, one store at 0x8000C2F4). No button combination or menu sets it: it can be switched on only by writing 1-4 to RAM 0x8010686C (debugger or cheat device). Pages: 1 "Render Update Build" with three timings (counts / 760625); 2 "ELMO" with X/Y/Z, curDir (heading in degrees) and "Frame:%3.3" (a truncated format: prints "Frame:" and nothing after the precision); 3 character distance table (`X Z Dsqr` for "Ernie", "Grover", "B.Bird"); 4 collision probe: NX/NY/NZ normal, X1..Z2 segment, idx, "node flags=%08x". D_80070760 selects which of two 4 KB buffers the text is rendered into (double buffering) |
| level-select text func_800301C8 | 1 | NO | no caller and no address reference anywhere (the earlier "addr in func_80032EB4" in agent ucode's graph was a stale %hi). It would print "Level select = %s" with name D_80075D78[20*D_80075D52] (Bonus_ABC, Ernie_chute, Ernie_manor, Bird_orchard, Bird_garden, StreetABC, SelectDiff, Grover_Space, Grover_Planet) at (70,20) when D_80075D53 != 0; D_80075D52/53 are only ever loaded (initial value 0). Dead twice over |
| frame-timing statistics func_8000D0CC | 1 (+ 10 accessors 0x8000D060-0x8000D7E0, all unreachable) | NO | no caller; its counters D_80070774.. are read only by the unreachable accessors |
| divide-by-zero report func_8001E314 | 1 | NO | part of an unused bounding-box culling chain (func_8001E718 -> func_8001E7EC -> func_8001E314) |
| collision matrix multiply func_80041F60 | 1 | NO | prints an aliasing warning through __assert (csutil.c); unreachable |
| profiler | 1 | yes | __osTimerInterrupt func_800577A4 contains the libultra profiler sampling branch (histogram of the interrupted PC over __osProfileList D_800ACC30..D_8012FCB8). D_800ACC30 is only loaded, never stored, by any code (xref: one load at 0x800578D0): osProfileInit/osProfileStart are not linked, so the branch never counts anything |

No other debug switch exists: every byte the debug code tests (D_8010686C, D_80075D53, D_800A8B10) was traced with
`tools/xref.py`; none is written by code reachable from a controller read except the zeroing at init.
