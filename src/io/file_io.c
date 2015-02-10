#include "file_io.h"

int save_game(game* gamep, FILE* file) {
	int error;
	struct board tempboard = (gamep->board)*;
	// print the number of turns left
	error = fprintf(file, "%d\n", gamep->turns);
	CHECK(error)
	// print the player
	error = fprintf(file, gamep->player == CAT ? "cat\n" : "mouse\n");
	CHECK(error)
	// add the mouse, cheese, and cat to the board
	tempboard[gamep->cat_x][gamep->cat_y] = CAT;
	tempboard[gamep->mouse_x][gamep->mouse_y] = MOUSE;
	tempboard[gamep->cheese_x][gamep->cheese_y] = CHEESE;
	// print the board
	for (int i = SIZE-1; i >= 0; i++) { // Y coordinate
		for (int j = 0; j < SIZE; j++) { // X coordinate
			error = fprintf(file, "%c", tempboard[j][i];
			CHECK(error)
		}
		error = fprintf(file, "\n");  //FIXME what about the last line break?
		CHECK(error)
	}
	return 0;
}

void find_obj(game* gamep, char obj) {
	int x = 0, y = 0;
	for (int i = 0; i < SIZE; i++) { // Y coordinate
		for (int j = 0; j < SIZE; j++) { // X coordinate
			if (gamep->board->board[j][i] == obj) {
				x = j;
				y = i;
				gamep->board->board[j][i] = EMPTY //empty the cell
			}
			break;
		}
	}
	// set the obj coordinates
	if (obj == CAT) {
		cat_x = x;
		cat_y = y;
	}
	if (obj == MOUSE) {
		mouse_x = x;
		mouse_y = y;
	}
	if (obj == CHEESE) {
		cheese_x = x;
		cheese_y = y;
	}
}

int load_game(game* gamep, FILE* file) {
	int error;
	const char delim[2] = "\n";
	char* line;
	char buffer[128]; // a buffer to read the file into
	error = fgets(buffer, 128, file);
	CHECK(error)
	// get the first line
	line = strtok(buffer, delim);
	if (line == NULL) { //this SHOULDN'T happen, we assume validity
		perror("Error: world file is not valid");
		return 1;
	}
	error = sscanf(line, "%d", &(gamep->turns)) //load the number of turns left
	CHECK(error)
	
	// get the second line
	line = strtok(NULL, delim);
	if (line == NULL) { //this SHOULDN'T happen, we assume validity
		perror("Error: world file is not valid");
		return 1;
	}
	gamep->player = strcmp("cat\n",line) ? MOUSE : CAT; //set the next player

	// get the next lines
	line = strtok(NULL, delim);
	for (int i = SIZE-1; i >= 0; i--) { // Y coordinate
		if (line == NULL) { //this SHOULDN'T happen, we assume validity
			perror("Error: world file is not valid");
			return 1;
		}
		for (int j = 0; j < SIZE; j++) { // X coordinate
			gamep->board->board[j][i] = line[j];
		}
		//we move to the next line of the board
		line = strtok(NULL, delim);
	}
		
	//now we will look for the mouse, cat, and cheese in the board
	find_obj(gamep, MOUSE);
	find_obj(gamep, CAT);
	find_obj(gamep, CHEESE);

	return 0;
}
