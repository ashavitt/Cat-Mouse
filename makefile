CFLAGS=-Wall -g -std=c99 -pedantic-errors -lm
O_FILES=MiniMax.o ListUtils.o
H_FILES=ListUtils.h MiniMax.h
VPATH = test/io/:src/io/:src/main/:src/structs/:src/core/:src/gui/:error/:test/core/:test/gui/

all: ListUtilsDemo MiniMaxDemo 

clean:
	rm -f $(O_FILES) ListUtilsDemo.o MiniMaxDemo.o GameUtils.o file_io.o board.o file_io_test.o getChildren_test.o ListUtilsDemo MiniMaxDemo file_io_test getChildren_test

ListUtilsDemo: ListUtilsDemo.o ListUtils.o
	gcc -o $@ $^ $(CFLAGS)
               
MiniMaxDemo: MiniMaxDemo.o $(O_FILES)
	gcc -o $@ $^ $(CFLAGS)

test: file_io_test getChildren_test gui_main_test

gui_main_test: main_test.o main.o widget.o uitree.o board.o ListUtils.o
	gcc -o $@ $^ $(CFLAGS) `sdl-config --libs`

file_io_test: file_io_test.o file_io.o board.o ListUtils.o
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

main_test.o: main_test.c
	gcc -c $(CFLAGS) $< `sdl-config --cflags`

main.o: main.c main.h shared.h widget.h uitree.h error.h board.h ListUtils.h
	gcc -c $(CFLAGS) $< `sdl-config --cflags`

widget.o: widget.c widget.h shared.h ListUtils.h error.h
	gcc -c $(CFLAGS) $< 'sdl-config --cflags'

uitree.o: uitree.c uitree.h shared.h widget.h ListUtils.h error.h
	gcc -c $(CFLAGS) $< 'sdl-config --cflags'
