CFLAGS=-Wall -g -std=c99 -pedantic-errors -lm
O_FILES=MiniMax.o ListUtils.o
H_FILES=ListUtils.h MiniMax.h
VPATH = test/io/:src/io/:src/main/:src/structs/

all: ListUtilsDemo MiniMaxDemo 

clean:
	rm -f $(O_FILES) ListUtilsDemo.o MiniMaxDemo.o GameUtils.o io.o ListUtilsDemo MiniMaxDemo

ListUtilsDemo: ListUtilsDemo.o ListUtils.o
	gcc -o $@ $^ $(CFLAGS)
               
MiniMaxDemo: MiniMaxDemo.o $(O_FILES)
	gcc -o $@ $^ $(CFLAGS)

test: file_io_test

file_io_test: file_io_test.o file_io.o
	gcc -o $@ $^ $(CFLAGS)

Connect4: $(O_FILES) Connect4.o io.o GameUtils.o
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

io.o: io.c io.h GameUtils.h
	gcc -c $(CFLAGS) $<

GameUtils.o: GameUtils.c GameUtils.h ListUtils.h
	gcc -c $(CFLAGS) $<
