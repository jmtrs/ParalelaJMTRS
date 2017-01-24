#! /bin/bash
DIRECTORY_TO_OBSERVE=$(dirname "$0")   
function block_for_change {
  inotifywait -r \
    -e modify,move,create,delete \
    $DIRECTORY_TO_OBSERVE
}
BUILD_SCRIPT=clone.sh          
function clone {
  bash $BUILD_SCRIPT
}

git add -A
git commit -m "jmtrs commit"
git push
echo
while block_for_change; do
  clone
done