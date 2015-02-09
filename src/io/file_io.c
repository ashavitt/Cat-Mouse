int save_game(game* gamep, FILE* file) {
	int error;
	struct board = (gamep->board)*;
	// print the number of turns left
	error = fprintf(file, "%d\n", gamep->turns);
	CHECK(error)
	// print the player
	error = fprintf(file, gamep->player == CAT ? "cat\n" : "mouse\n");
	CHECK(error)
	// add the mouse, cheese, and cat to the board
	board[gamep->cat_x][gamep->cat_y] = CAT;
	board[gamep->mouse_x][gamep->mouse_y] = MOUSE;
	board[gamep->cheese_x][gamep->cheese_y] = CHEESE;
	// print the board
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			error = fprintf(file, "%c", board[i][j];
			CHECK(error)
		}
		error = fprintf(file, "\n");  //FIXME what about the last line break?
		CHECK(error)
	}
	return 0;
}
