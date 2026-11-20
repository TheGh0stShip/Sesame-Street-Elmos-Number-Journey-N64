#!/bin/bash
# run through job.sh: cd ${N64DK_LAB}/tools && ./job.sh ses_ledger 2G 600 bash ${N64DK_LAB}/sesassets/ledger/tools/run.sh
R=${N64DK_LAB}/roms
cd ${N64DK_LAB}/sesassets/ledger
exec ${N64DK_LAB}/venv/bin/python tools/merge_ledger.py . \
  --rom "letter=$R/Sesame Street - Elmo's Letter Adventure (USA).z64" \
  --rom "number=$R/Sesame Street - Elmo's Number Journey (USA).z64"
