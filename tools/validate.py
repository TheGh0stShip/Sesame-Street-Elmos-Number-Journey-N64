#!/usr/bin/env python3
import csv, hashlib, json, re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

def rows(name):
    with (ROOT / name).open(encoding="utf-8", newline="") as f:
        return list(csv.DictReader(f, delimiter="\t", strict=True))

def main():
    cfg = json.loads((ROOT / "progress-config.json").read_text())
    inv = rows("progress-inventory.tsv")
    proof = rows("VERIFY.tsv")
    accounted = rows("source-accounted.tsv")
    assert len(inv) == len(proof) == len(accounted) == cfg["functions"]
    assert sum(int(r["code_size"], 0) for r in inv) == cfg["code_bytes"]
    seen = set()
    for row, verified, source_row in zip(inv, proof, accounted):
        key = (row["image"], int(row["vram"], 0))
        assert key not in seen
        seen.add(key)
        assert verified["status"].startswith("MATCH") and verified["word_exact"].startswith("exact")
        assert int(verified["address"], 16) == key[1]
        assert int(verified["size"]) == int(row["code_size"], 0)
        source = ROOT / row["source"]
        assert source.is_file() and source == ROOT / source_row["source"]
        assert hashlib.sha256(source.read_bytes()).hexdigest() == row["source_sha256"] == source_row["source_sha256"]
    intervals = sorted((int(r["vram"], 0), int(r["vram"], 0) + int(r["code_size"], 0)) for r in inv)
    assert all(a1 <= b0 for (_, a1), (b0, _) in zip(intervals, intervals[1:]))
    print(f"validated {len(inv)} word-exact functions and {cfg['code_bytes']} code bytes")

if __name__ == "__main__": main()
