# Debug and development material: Elmo's Number Journey

Generated part (the table at the end) comes from `tools/debug.py` (call graph `out/graph_number.json` from
`tools/callgraph.py`, strings `TEXT_number.tsv` from `tools/strings.py`). The narrative was checked against the
matched C in `sesverify/v16/number/src` and the address cross-references from `tools/xref.py`.
"Reachable" means reachable from the entry point 0x80000400 through calls, branches, function addresses built in
code and function pointers stored in data (see REPORT.md for the method).

## Summary

| what | functions | reachable | how it is (or is not) switched on |
|---|---|---|---|
| libultra debugger: rmon | 52 | 51 (func_8006621C, a spin-forever stub, is not) | started unconditionally: the boot thread func_8000C0A8 creates rmonMain func_80060A40 (priority 250); rmonMain calls func_8006614C, which binds OS events 10-13 (CPU break, SP break, faults) and starts the rdb thread func_80060CC4. The threads only wake for a host debugger, so on a retail console they block forever. The rmon request dispatcher table is at 0x80078EB0 (.rodata) |
| rdb / kdebugserver | 4 | 4 | __osRdbSend func_80063440, buffer sender func_800630A0, kdebugserver func_80063248 (called from the exception handler func_80061EB4): need a development board on the RDB port |
| print paths | 7 | 6 | func_80061178 is the game's printf: _Printf (func_80067910) to the KMC debugger port (sink func_80060F40) when D_800A8F80 != 0, else to rdb (sink func_80061224). D_800A8F80 is set only by __osInitialize_common func_80058740 when 0xBFF00000 reads "KMC\0". osSyncPrintf func_800611DC is linked but never called |
| asserts and error messages | 27 callers | 25 reachable | __assert func_80059540 prints `\nASSERTION FAULT: %s, %d: "%s"\n` through func_80061178 with "EX" and a source file name (code/gfx.c, audio/n64aud.c, actors/*.c including actors/bonusact.c which only Number has, ../ratbgle/engine/collide/cs*.c). Only on failed internal checks, and only to the debugger port. Same messages as Letter: "Two keys with same frame #!", "Frame passed last key!" (func_80000450 / func_8000092C), "addActorDL(): draw node list full!  Yow!" (func_80001424), "DrawWorld(): bsp stack blown (boom)" (func_800271AC), RTA_mail messages (func_800496B8 / func_80049894 via func_80049920) |
| on-screen debug overlay func_80001F60 | 1 | yes (called every frame by func_8000BA3C) | draws nothing unless the page byte D_80106C6C is 1-4; its only store is `D_80106C6C = 0` in the init function func_8000C60C (store at 0x8000C77C; loads at 0x80001F88 / 0x80002494). No button combination or menu sets it: only a RAM write of 1-4 to 0x80106C6C turns it on. Pages: 1 "Render Update Build" timings; 2 "ELMO" X/Y/Z, curDir and the truncated "Frame:%3.3"; 3 character distance table (`X Z Dsqr`; names "Ernie", "Count", "Cookie"); 4 "Screen Control" with "ScrHt=%d" (D_800B1092) and "FOV=%f" (D_80108FB4). Number's overlay is a different function from Letter's (different hash): the collision page 4 was replaced by the screen page |
| level-select text func_800327A4 | 1 | NO | no caller and no address reference. Would print "Level select = %s" with name D_8007693C[20*D_80076916] when D_80076917 != 0; Number's name table has only two entries, "Street" and "River" (Letter's has nine). D_80076916/17 are only loaded (initial value 0). func_80034804 also builds the address 0x8007693C, but only as the base of an index into the word table D_80076960 (index >= 9), not to read the names |
| frame-timing statistics func_8000D53C | 1 (+ accessors, all unreachable) | NO | no caller |
| divide-by-zero report func_8001EBF4 | 1 | NO | unused bounding-box culling chain (called only from func_8001F0CC, itself unreachable) |
| collision matrix multiply func_80043BE0 | 1 | NO | aliasing warning through __assert (csutil.c); unreachable |
| profiler | 1 | yes | __osTimerInterrupt func_80059114 has the profiler sampling branch over __osProfileList D_800AD03C; D_800AD03C is only loaded (0x80059240), never stored: osProfileInit/osProfileStart are not linked, the branch never counts |

No other debug switch exists: every byte the debug code tests (D_80106C6C, D_80076917, D_800A8F80) was traced with
`tools/xref.py`; none is written by code reachable from a controller read except the zeroing at init.
