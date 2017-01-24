#! /bin/bash
MAINDIRECTORY=$(dirname "$0")   
function refresh {
  inotifywait -r \
    -e modify,move,create,delete \
    $MAINDIRECTORY
}

SCRIPT=gpos.sh          
function gpos {
  bash $SCRIPT
}

git add -A
git commit -m "jmtrs commit"
git push


while refresh; do
  gpos
done