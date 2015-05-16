CFLAGS=-Wall -g -std=c99 -pedantic-errors -lm
LINKER=gcc -o $@ $^ $(CFLAGS)
GUI_LINKER=gcc -o $@ $^ $(CFLAGS) `sdl-config --libs`
COMPILER=gcc -c $(CFLAGS) $<
GUI_COMPILER=gcc -c $(CFLAGS) $< `sdl-config --cflags`
O_FILES=MiniMax.o ListUtils.o main.o widget.o uitree.o handlers.o actions.o board.o file_io.o ai.o evaluate.o shared.o CatAndMouse.o
TEST_O_FILES=ListUtilsDemo.o MiniMaxDemo.o file_io_test.o getChildren_test.o gui_main_test.o
H_FILES=ListUtils.h MiniMax.h
VPATH = test/io/:src/io/:src/main/:src/core/:src/gui/:src/error/:test/core/:test/gui/

all: ListUtilsDemo MiniMaxDemo CatAndMouse test

clean:
	rm -f $(O_FILES) $(TEST_O_FILES) ListUtilsDemo MiniMaxDemo file_io_test getChildren_test gui_main_test CatAndMouse

test: file_io_test getChildren_test gui_main_test

ListUtilsDemo: ListUtilsDemo.o ListUtils.o
	$(LINKER)
               
MiniMaxDemo: MiniMaxDemo.o ListUtils.o MiniMax.o
	$(LINKER)

gui_main_test: main_test.o main.o widget.o uitree.o handlers.o actions.o board.o ListUtils.o file_io.o ai.o evaluate.o MiniMax.o shared.o
	$(GUI_LINKER)

file_io_test: file_io_test.o file_io.o board.o ListUtils.o
	$(LINKER)

getChildren_test: getChildren_test.o file_io.o board.o ListUtils.o
	$(LINKER)

CatAndMouse: $(O_FILES)
	$(GUI_LINKER)

CatAndMouse.o: CatAndMouse.c CatAndMouse.h main.h
	$(GUI_COMPILER)

ListUtilsDemo.o: ListUtilsDemo.c  ListUtils.h
	$(COMPILER)

MiniMaxDemo.o: MiniMaxDemo.c MiniMax.h ListUtils.h
	$(COMPILER)

MiniMax.o: MiniMax.c MiniMax.h ListUtils.h
	$(COMPILER)

ListUtils.o: ListUtils.c ListUtils.h
	$(COMPILER)

file_io_test.o: file_io_test.c
	$(COMPILER)

file_io.o: file_io.c file_io.h board.h
	$(COMPILER)

board.o: board.c board.h
	$(COMPILER)

ai.o: ai.c ai.h board.h MiniMax.h evaluate.h
	$(COMPILER)

evaluate.o: evaluate.c evaluate.h board.h
	$(COMPILER)

getChildren_test.o: getChildren_test.c
	$(COMPILER)

main_test.o: main_test.c
	$(GUI_COMPILER)

main.o: main.c main.h shared.h widget.h uitree.h handlers.h error.h board.h ListUtils.h file_io.h ai.h
	$(GUI_COMPILER)

widget.o: widget.c widget.h shared.h ListUtils.h error.h
	$(GUI_COMPILER)

uitree.o: uitree.c uitree.h shared.h widget.h ListUtils.h error.h actions.h
	$(GUI_COMPILER)

handlers.o: handlers.c shared.h widget.h ListUtils.h error.h
	$(GUI_COMPILER)

actions.o: actions.c shared.h widget.h ListUtils.h error.h game_settings.h file_io.h
	$(GUI_COMPILER)

shared.o: shared.c shared.h board.h
	$(GUI_COMPILER)
