#include "../../src/io/file_io.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	FILE* testfile;
	char* dest_file_name;
	game* game_test = (game*) malloc (sizeof(game));
	struct board* board_test = (struct board*) malloc (sizeof(struct board));
	game_test->board = board_test;
	if (game_test == NULL) {
		perror("Error: malloc failed.\n");
		return 1;
	}
	//run on all file names
	for (int i = 1; i < argc; i++) {
		testfile = fopen(argv[i], "r");
		if (testfile == NULL) {
			perror("Error: fopen failed.\n");
			return 2;
		}
		load_game(game_test, testfile);
		if (fclose(testfile) != 0) {
			perror("Error: fclose failed.\n");
			return 3;
		}
		dest_file_name = strrchr(argv[i], '/') + 1; //returns the file name without the path
		testfile = fopen(dest_file_name, "w");
		if (testfile == NULL) {
			perror("Error: fopen failed.\n");
			return 2;
		}
		save_game(game_test, testfile);
		if (fclose(testfile) != 0) {
			perror("Error: fclose failed.\n");
			return 3;
		}
	}
	return 0;
}
