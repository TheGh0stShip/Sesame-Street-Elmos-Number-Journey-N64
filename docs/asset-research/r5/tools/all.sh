#!/bin/bash
# all.sh: rebuild every r5 output in order (each step is one python process; run the whole thing through job.sh:
#   cd ${N64DK_LAB}/tools && ./job.sh ses_r5 3G 1200 bash ${N64DK_LAB}/sesassets/r5/tools/all.sh)
set -e
S=${N64DK_LAB}/sesassets/r5/tools/run.sh
for step in graph strings debug unused diff docs; do echo "== $step"; bash $S $step; done
