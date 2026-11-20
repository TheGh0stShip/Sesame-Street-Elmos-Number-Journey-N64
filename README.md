# Sesame Street: Elmo's Number Journey (Nintendo 64) — byte-matching decompilation

All **973 catalogued functions** and **444,672 executable bytes** in the USA release reproduce word-for-word from the published sources. Independent verification run v17 compiled every function at its retail address and compared every instruction; float constants and switch tables were checked separately.

| Completion metric | Verified | Catalogued | Progress |
|---|---:|---:|---:|
| **Functions reproduced from verified source** | **973** | **973** | **100.00%** |
| **Code bytes reproduced from verified source** | **444,672** | **444,672** | **100.00%** |
| Ordinary matching C functions | 920 | 973 | 94.55% |
| Ordinary matching C bytes | 434,764 | 444,672 | 97.77% |

[![Source-complete code](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64.svg?version=usa&category=catalogued-code&measure=complete_code_percent&label=Source-complete%20code)](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64/usa)
[![Matching C bytes](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64.svg?version=usa&category=catalogued-code&measure=matched_code_percent&label=Matching%20C%20bytes)](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64/usa)

ByteMatch's **complete code / units** measurement is the project-completion figure. The ordinary-C score deliberately excludes 53 files containing required MIPS assembly; those units are complete source, not missing work.

## Verification and provenance

- USA ROM SHA-1: `7195ea96d9fe5de065af61f70d55c92c8ee905e6` (the ROM is not included).
- `VERIFY.tsv` records the address, owned size, compiler lane, source origin and `word_exact` verdict for every function.
- `progress-inventory.tsv` and `source-accounted.tsv` bind every interval to its published source SHA-256.
- `NAMES.tsv` records resolved named references used during strict replay.
- GitHub Actions validates counts, ranges, source digests, proof rows and generated tracker reports on every push.

This is a byte-matching decompilation at function level: every published function was compiled with its recorded original compiler lane and compared word-for-word with the retail ROM. It is not yet organized as a conventional matching-decomp project with shared headers, translation units, a linker script, and a self-contained `make` ROM build. That packaging gap does not change the function-level matching result. The sources are also not claimed to be fully human-restored or portable; `UGLY_FORMS.tsv` identifies compiler-sensitive forms retained for matching.

## Assets and whole-ROM build

Every byte of the 8 MiB ROM and all eight unpacked level images is classified with zero unknown ranges. The last measured whole-ROM build matched the original SHA-1 while regenerating **80.5%** of ROM bytes and placing the remaining identified bytes verbatim. Later component encoders passed their own byte-exact checks, but the end-to-end regeneration percentage has not yet been remeasured. The pending DOS gzip 1.2.4 experiment is not counted unless it reproduces all compressed level streams exactly.

Research maps and format notes are under `docs/asset-research/`; extracted copyrighted assets are not included.
