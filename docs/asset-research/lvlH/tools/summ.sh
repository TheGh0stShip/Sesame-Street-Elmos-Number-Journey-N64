#!/bin/bash
# summarise a map file: bytes per class and the unknown ranges
awk -F'\t' '{split($2,a,":");split($3,b,":"); s[$4]+=strtonum(b[2])-strtonum(a[2])} END{for(k in s) print k, s[k]}' $1
awk -F'\t' '$4=="unknown"{split($2,a,":");split($3,b,":"); n=strtonum(b[2])-strtonum(a[2]); if(n>='${2:-16}') print $2,n,$6}' $1
