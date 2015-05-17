#include "../../src/io/file_io.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	char dest_child_name[128];
	int child_num;
	ListRef children;
	FILE* testfile;
	char* dest_file_name;
	Game* game_test = (Game*) malloc (sizeof(Game));
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
		children = getChildren(game_test);
		//check everything is valid
		if (children == NULL) {
			perror("Error: list is NULL.\n");
			return 4;
		}
		if (isEmpty(children)) {
			perror("Error: list is empty.\n");
			return 5;
		}
		//save the children
		child_num = 0;
		do {
			sprintf(dest_child_name, "%d", child_num);
			dest_file_name = strrchr(argv[i], '/') + 1; //returns the file name without the path
			dest_file_name = strcat(dest_child_name, dest_file_name); //dest file name: $child_num$dest_file_name
			testfile = fopen(dest_file_name, "w");
			if (testfile == NULL) {
				perror("Error: fopen failed.\n");
				return 2;
			}
			save_game((Game*)headData(children), testfile);
			if (fclose(testfile) != 0) {
				perror("Error: fclose failed.\n");
				return 3;
			}
			children = tail(children);
			child_num++;
		} while (children != NULL);
	}
	return 0;
}
