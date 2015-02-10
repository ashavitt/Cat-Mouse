#!/bin/csh
make file_io_test
chmod +x "file_io_test"
foreach x ( 1 2 3 4 5 )
	echo "world $x :"
	./file_io_test worlds/world_$x.txt
	echo $status
	diff worlds/world_$x.txt ./world_$x.txt
	rm ./world_$x.txt
end
