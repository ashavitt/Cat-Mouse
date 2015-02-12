#!/bin/csh
make getChildren_test
chmod +x "getChildren_test"
./getChildren_test test/core/getChildren_test/world.txt
echo return value: $status
diff -q test/core/getChildren_test/child.txt ./0world.txt
rm ./0world.txt
