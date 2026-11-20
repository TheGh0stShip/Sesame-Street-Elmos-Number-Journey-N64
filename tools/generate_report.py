#!/usr/bin/env python3
import argparse, csv, json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

def rows(name):
    with (ROOT / name).open(encoding="utf-8", newline="") as f:
        return list(csv.DictReader(f, delimiter="\t", strict=True))

def build():
    cfg = json.loads((ROOT / "progress-config.json").read_text())
    inventory = rows("progress-inventory.tsv")
    total = sum(int(r["code_size"], 0) for r in inventory)
    crows = [r for r in inventory if r["reproduced_by"] == "C"]
    cbytes = sum(int(r["code_size"], 0) for r in crows)
    scope = cfg["scope"]
    def track(i, label, done, all_, unit):
        return {"id": i, "label": label, "scope": scope, "measurement": {"kind": "ratio", "completed": done, "total": all_, "unit": unit}}
    progress = {"format": "decomphub.progress/v1", "repository": cfg["repository"], "tracks": [
        track("matching-code", "Matching decompiled C code", cbytes, total, "bytes"),
        track("matching-functions", "C-decompiled functions", len(crows), len(inventory), "functions"),
        track("source-complete-code", "Code reproduced from verified source", total, total, "bytes"),
    ]}
    def measures(size, c, funcs, cfuncs):
        return {"total_code": str(size), "matched_code": str(c), "matched_code_percent": 100*c/size,
                "fuzzy_match_percent": 100*c/size, "total_functions": funcs, "matched_functions": cfuncs,
                "matched_functions_percent": 100*cfuncs/funcs, "total_units": funcs,
                "complete_code": str(size), "complete_code_percent": 100.0, "complete_units": funcs}
    units = []
    for r in inventory:
        size = int(r["code_size"], 0); is_c = r["reproduced_by"] == "C"; address = str(int(r["vram"], 0))
        units.append({"name": f"main/{r['function']}@{r['vram']}", "measures": measures(size, size if is_c else 0, 1, int(is_c)),
            "functions": [{"name": r["function"], "size": str(size), "address": address,
                "fuzzy_match_percent": 100.0 if is_c else 0.0, "metadata": {"virtual_address": address}}],
            "metadata": {"progress_categories": ["catalogued-code"], "complete": True, "source_path": r["source"]}})
    totals = measures(total, cbytes, len(inventory), len(crows))
    objdiff = {"version": 2, "measures": totals, "units": units,
        "categories": [{"id": "catalogued-code", "name": "Catalogued executable code", "measures": totals}]}
    return progress, objdiff

def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--check", action="store_true"); args = ap.parse_args()
    progress, report = build()
    outputs = [(ROOT / "decomphub.progress.json", progress), (ROOT / "build/report.json", report)]
    for path, value in outputs:
        text = json.dumps(value, indent=2) + "\n"
        if args.check:
            if not path.exists() or path.read_text() != text: raise SystemExit(f"stale report: {path}")
        else:
            path.parent.mkdir(parents=True, exist_ok=True); path.write_text(text)

if __name__ == "__main__": main()
