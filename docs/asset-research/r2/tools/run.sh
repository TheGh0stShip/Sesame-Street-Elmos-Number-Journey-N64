#!/bin/bash
# run through job.sh: cd ${N64DK_LAB}/tools && ./job.sh ses_r2 2G 600 bash ${N64DK_LAB}/sesassets/r2/tools/run.sh
cd ${N64DK_LAB}/sesassets/r2
exec ${N64DK_LAB}/venv/bin/python -I tools/resolve.py .
