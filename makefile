CFLAGS=-Wall -g -std=c99 -pedantic-errors -lm
O_FILES=MiniMax.o ListUtils.o
H_FILES=ListUtils.h MiniMax.h
VPATH = test/io/:src/io/:src/main/:src/structs/

all: ListUtilsDemo MiniMaxDemo 

clean:
	rm -f $(O_FILES) ListUtilsDemo.o MiniMaxDemo.o GameUtils.o file_io.o board.o file_io_test.o ListUtilsDemo MiniMaxDemo file_io_test

ListUtilsDemo: ListUtilsDemo.o ListUtils.o
	gcc -o $@ $^ $(CFLAGS)
               
MiniMaxDemo: MiniMaxDemo.o $(O_FILES)
	gcc -o $@ $^ $(CFLAGS)

test: file_io_test getChildren_test

file_io_test: file_io_test.o file_io.o board.o
	gcc -o $@ $^ $(CFLAGS)

getChildren_test: getChildren_test.o file_io.o board.o ListUtils.o
	gcc -o $@ $^ $(CFLAGS)

ListUtilsDemo.o: ListUtilsDemo.c  ListUtils.h
	gcc -c $(CFLAGS) $<

MiniMaxDemo.o: MiniMaxDemo.c MiniMax.h ListUtils.h
	gcc -c $(CFLAGS) $<

MiniMax.o: MiniMax.c MiniMax.h ListUtils.h
	gcc -c $(CFLAGS) $<

ListUtils.o: ListUtils.c ListUtils.h
	gcc -c $(CFLAGS) $<

file_io_test.o: file_io_test.c
	gcc -c $(CFLAGS) $<

file_io.o: file_io.c file_io.h board.h
	gcc -c $(CFLAGS) $<

board.o: board.c board.h
	gcc -c $(CFLAGS) $<

getChildren_test.o: getChildren_test.c
	gcc -c $(CFLAGS) $<
