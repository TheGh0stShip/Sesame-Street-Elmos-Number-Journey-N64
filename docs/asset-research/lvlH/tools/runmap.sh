#!/bin/bash
# runs the classifier in map mode through the lab job runner
cd ${N64DK_LAB}/tools && TAIL=30 ./job.sh ses_lvlH 2G 900 bash -c 'cd /tmp && ${N64DK_LAB}/venv/bin/python -I ${N64DK_LAB}/sesassets/lvlH/tools/extract_level_rest.py "${N64DK_LAB}/roms/Sesame Street - Elmo'"'"'s Number Journey (USA).z64" number '"${2:-/tmp}"' '"${1:-map}"' > ${N64DK_LAB}/sesassets/lvlH/out/run_'"${1:-map}"'.txt'
