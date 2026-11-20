# ledger report
tools/merge_ledger.py merges all 22 MAP.tsv files and the code ledger (functions.json + VERIFY v16) into LEDGER_<game>.tsv (whole ROM), LEVELS_<game>.tsv (16 inflated level images), CONFLICTS_<game>.tsv and SUMMARY.md. All 18 tilings pass zero-gap / zero-overlap assertions; no byte without an owner. Priority rules: FORMAT.md.
Still unknown in ROM: Letter 1,156 B in 8 ranges, Number 2,534 B in 10 ranges (unreferenced scraps in the common block). Still unknown in levels: Letter manor 0x60B0-0x6AB0 (2,560 B); Number bump 6,688 B.
Conflicts: Letter 1,811 pairs, Number 1,861 (mostly gfx vs lvlB boundary granularity; reviewed classes: hud vs packC, data1/data2 vs gfx, lvlD vs lvlB/E-H model headers, lvlB vs lvlC size estimates, code vs data1 at 0x103C-0x1050).
Limits: the ledger reflects the agents maps; apart from an all-zero test it does not re-check claims against the ROM. The unused-content list in SUMMARY.md comes from a keyword search and contains some false hits.
