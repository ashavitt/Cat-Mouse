#!/bin/csh
make getChildren_test
chmod +x "getChildren_test"
../../getChildren_test getChildren_test/world.txt
echo return value: $status
diff -q -b getChildren_test/child.txt ../../0world.txt
rm ../../0world.txt
