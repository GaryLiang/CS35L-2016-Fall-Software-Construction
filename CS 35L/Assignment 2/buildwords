#!/bin/bash
sed -e 's/<[^>]*>//g' $1 | 
    tr '[:upper:]' '[:lower:]'|
    grep -v -i '[cbdfgjqrstvxyz.]'|
    tr ',' '\n' |sed "s/\`/\'/g"| 
    tr -d '\ '|
    sort -u

exit 0