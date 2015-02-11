#include "file_io.h"

int save_game(game* gamep, FILE* file) {
	int error;
	struct board tempboard = *(gamep->board);
	// print the number of turns left
	error = fprintf(file, "%d\r\n", gamep->turns);
	CHECK(error)
	// print the player
	error = fprintf(file, gamep->player == CAT ? "cat\r\n" : "mouse\r\n");
	CHECK(error)
	// add the mouse, cheese, and cat to the board
	tempboard.board[gamep->cat_x][gamep->cat_y] = CAT;
	tempboard.board[gamep->mouse_x][gamep->mouse_y] = MOUSE;
	tempboard.board[gamep->cheese_x][gamep->cheese_y] = CHEESE;
	// print the board
	for (int i = BOARD_SIZE-1; i >= 0; i--) { // Y coordinate
		for (int j = 0; j < BOARD_SIZE; j++) { // X coordinate
			error = fprintf(file, "%c", tempboard.board[j][i]);
			CHECK(error)
		}
		error = fprintf(file, (i != 0 ? "\r\n" : ""));
		CHECK(error)
	}
	return 0;
}

void find_obj(game* gamep, char obj) {
	int count = 0;
	int x = 0, y = 0;
	int flag = 1;
	for (int i = 0; i < BOARD_SIZE && flag; i++) { // Y coordinate
		for (int j = 0; j < BOARD_SIZE && flag; j++) { // X coordinate
			if (gamep->board->board[j][i] == obj) {
				count++;
				x = j;
				y = i;
				gamep->board->board[j][i] = EMPTY; //empty the cell
				flag = 0;
			}
		}
	}
	// set the obj coordinates
	if (obj == CAT) {
		gamep->cat_x = x;
		gamep->cat_y = y;
	}
	if (obj == MOUSE) {
		gamep->mouse_x = x;
		gamep->mouse_y = y;
	}
	if (obj == CHEESE) {
		gamep->cheese_x = x;
		gamep->cheese_y = y;
	}
}

#define BFLEN 10
int load_game(game* gamep, FILE* file) {
	int error;
	char* checker;
	char buffer[BFLEN]; // a buffer to read the file into
	buffer[BFLEN-1] = EOF;
	if (fgets(buffer, BFLEN, file) == NULL) {
		perror("Error: reading the world file failed\n");
		return 1;
	}
	// get the first line
	error = sscanf(buffer, "%hhu", &(gamep->turns)); //load the number of turns left
	CHECK(error)
	
	// get the second line
	if (fgets(buffer, BFLEN, file) == NULL) {
		perror("Error: reading the world file failed\n");
		return 1;
	}
	gamep->player = (strstr(buffer, "cat") == NULL) ? MOUSE : CAT; //set the next player

	// get the next lines
	checker = fgets(buffer, BFLEN, file);
	for (int i = BOARD_SIZE-1; i >= 0; i--) { // Y coordinate
		if (checker == NULL) { //this SHOULDN'T happen, we assume validity
			perror("Error: world file is not valid\n");
			return 1;
		}
		for (int j = 0; j < BOARD_SIZE; j++) { // X coordinate
			gamep->board->board[j][i] = buffer[j];
		}
		//we move to the next line of the board
		checker = fgets(buffer, BFLEN, file);
	}
		
	//now we will look for the mouse, cat, and cheese in the board
	find_obj(gamep, MOUSE);
	find_obj(gamep, CAT);
	find_obj(gamep, CHEESE);

	return 0;
}
