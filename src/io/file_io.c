#include "file_io.h"

/** saves a game to a file */
int save_game(Game* gamep, FILE* file) {
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
	for (int i = 0; i < BOARD_SIZE; i++) { // Y coordinate
		for (int j = 0; j < BOARD_SIZE; j++) { // X coordinate
			error = fprintf(file, "%c", tempboard.board[j][i]);
			CHECK(error)
		}
		//TODO should it be linux new line or windows new line?
		error = fprintf(file, (i != (BOARD_SIZE - 1) ? "\r\n" : ""));
		CHECK(error)
	}
	return 0;
}

/** finds an obj (CAT, MOUSE, CHEESE) in the board and sets its coordinates */
void find_obj(Game* gamep, char obj) {
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

int load_game(Game* gamep, FILE* file) {
	int error;
	char* checker;
	char buffer[BFLEN]; // a buffer to read the file into
	buffer[BFLEN-1] = EOF;
	if (fgets(buffer, BFLEN, file) == NULL) {
		fprintf(stderr, "Error: reading the world file failed\n");
		return 1;
	}
	if (buffer[0] == 'q') {
		return CONSOLE_QUIT_CODE;
	}
	// get the first line
	error = sscanf(buffer, "%hhu", &(gamep->turns)); //load the number of turns left
	CHECK(error)
	
	// get the second line
	if (fgets(buffer, BFLEN, file) == NULL) {
		fprintf(stderr, "Error: reading the world file failed\n");
		return 1;
	}
	gamep->player = (strstr(buffer, "cat") == NULL) ? MOUSE : CAT; //set the next player

	// get the next lines
	for (int i = 0; i < BOARD_SIZE; i++) { // Y coordinate
		//we move to the next line of the board
		checker = fgets(buffer, BFLEN, file);
		if (checker == NULL) { //this SHOULDN'T happen, we assume validity
			fprintf(stderr, "Error: world file is not valid\n");
			return 1;
		}
		for (int j = 0; j < BOARD_SIZE; j++) { // X coordinate
			gamep->board->board[j][i] = buffer[j];
		}
	}
		
	//now we will look for the mouse, cat, and cheese in the board
	find_obj(gamep, MOUSE);
	find_obj(gamep, CAT);
	find_obj(gamep, CHEESE);

	return 0;
}

/** Wrapper function for load_game
  * it recieves a world_id, mallocs the game and finds the correct world file */
Game* load_world(int id) {
	FILE* gamefile;
	char dest_file_name[128];
	Game* game = game_malloc();
	if (game == NULL) {
		fprintf(stderr, "Error: malloc failed.\n");
		return NULL;
	}
	// create the string of the world file path
	snprintf(dest_file_name, 128, "worlds/world_%d.txt", id);
	
	// open the world file
	gamefile = fopen(dest_file_name, "r");
	if (gamefile == NULL) {
		fprintf(stderr, "Error: fopen failed.\n");
		return NULL;
	}
	// call the load_game function
	if (load_game(game, gamefile) != 0) {
		fprintf(stderr, "Error: loading game %d from the file failed.\n", id);
		return NULL;
	}
	if (fclose(gamefile) != 0) {
		fprintf(stderr, "Error: fclose failed.\n");
		return NULL;
	}
	return game;
}

/** Wrapper function for save_game
  * it recieves a world_id, game, and finds the correct world file */
int save_world(int id, Game* game) {
	FILE* gamefile;
	char dest_file_name[128];
	// create the string of the world file path
	snprintf(dest_file_name, 128, "worlds/world_%d.txt", id);
	
	gamefile = fopen(dest_file_name, "w");
	if (gamefile == NULL) {
		fprintf(stderr, "Error: fopen failed.\n");
		return 1;
	}
	if (save_game(game, gamefile) != 0) {
		fprintf(stderr, "Error: saving game %d to the file failed.\n", id);
		return 1;
	}
	if (fclose(gamefile) != 0) {
		fprintf(stderr, "Error: fclose failed.\n");
		return 1;
	}
	return 0;
}
