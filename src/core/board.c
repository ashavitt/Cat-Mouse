#include "board.h"

Game* game_malloc() {
	Game* gamep = (Game*) malloc (sizeof(Game));
	if (gamep == NULL) {
		return NULL;
	}
	struct board* boardp = (struct board*) malloc (sizeof(struct board));
	if (boardp == NULL) {
		return NULL;
	}
	gamep->board = boardp;
	return gamep;
}

int check_bounds_walls_cheese(int dir, int x, int y, int cheese_x, int cheese_y, struct board* boardp) {
	switch (dir) {
		case UP:
			if (y + 1 == BOARD_SIZE) {
				return 0;
			}
			if (boardp->board[x][y+1] == WALL) {
				return 0;
			}
			if (x == cheese_x && y+1 == cheese_y) {
				return 0;
			}
			break;
		case RIGHT:
			if (x + 1 == BOARD_SIZE) {
				return 0;
			}
			if (boardp->board[x+1][y] == WALL) {
				return 0;
			}
			if (x+1 == cheese_x && y == cheese_y) {
				return 0;
			}
			break;
		case DOWN:
			if (y == 0) {
				return 0;
			}
			if (boardp->board[x][y-1] == WALL) {
				return 0;
			}
			if (x == cheese_x && y-1 == cheese_y) {
				return 0;
			}
			break;
		case LEFT:
			if (x == 0) {
				return 0;
			}
			if (boardp->board[x-1][y] == WALL) {
				return 0;
			}
			if (x-1 == cheese_x && y == cheese_y) {
				return 0;
			}
			break;
	}
	return 1;
}

int dir_is_valid(Game* gamep, int dir) {
	if (gamep->player == CAT) {
		//check if we are out of bounds or cell is a wall or cell is cheese
		if (!check_bounds_walls_cheese(dir, gamep->cat_x, gamep->cat_y, gamep->cheese_x, gamep->cheese_y, gamep->board)) {
			return 0;
		}
		switch (dir) {
			case UP:
				//check if the cell is empty
				if (gamep->mouse_x == gamep->cat_x && gamep->mouse_y == gamep->cat_y + 1) {
					return 0;
				}
				break;
			case RIGHT:
				//check if the cell is empty
				if (gamep->mouse_x == gamep->cat_x + 1 && gamep->mouse_y == gamep->cat_y) {
					return 0;
				}
				break;
			case DOWN:
				//check if the cell is empty
				if (gamep->mouse_x == gamep->cat_x && gamep->mouse_y == gamep->cat_y - 1) {
					return 0;
				}
				break;
			case LEFT:
				if (gamep->mouse_x == gamep->cat_x - 1 && gamep->mouse_y == gamep->cat_y) {
					return 0;
				}
				break;
		}
		return 1;
	} else {
		//check if we are out of bounds or cell is a wall or cell is cheese
		if (!check_bounds_walls_cheese(dir, gamep->mouse_x, gamep->mouse_y, gamep->cheese_x, gamep->cheese_y, gamep->board)) {
			return 0;
		}
		switch (dir) {
			case UP:
				//check if the cell is empty
				if (gamep->cat_x == gamep->mouse_x && gamep->cat_y == gamep->mouse_y + 1) {
					return 0;
				}
				break;
			case RIGHT:
				//check if the cell is empty
				if (gamep->cat_x == gamep->mouse_x + 1 && gamep->cat_y == gamep->mouse_y) {
					return 0;
				}
				break;
			case DOWN:
				//check if the cell is empty
				if (gamep->cat_x == gamep->mouse_x && gamep->cat_y == gamep->mouse_y - 1) {
					return 0;
				}
				break;
			case LEFT:
				if (gamep->cat_x == gamep->mouse_x - 1 && gamep->cat_y == gamep->mouse_y) {
					return 0;
				}
				break;
		}
		return 1;
	}
}

ListRef getChildren(void* gameVoid) {
	Game* gamep = (Game*) gameVoid;
	ListRef children = newList(NULL); // Creates an empty list

	LIFT(children); // if malloc fails
	ListRef tempList = children; // save original list for later

	// Run over all directions
	for(int i = 0; i < 4; i++) {
		if (dir_is_valid(gamep, i)) { // Direction is valid
			Game* child = game_malloc(); // Allocate memory for child
			if (child == NULL) { // malloc failed
				destroyList(children, free);
				return NULL;
		  	}
			// Game settings
			child->num_steps = gamep->num_steps;
			// Switch the player for next turn
			child->player = SWITCH(gamep->player);

			//reduce the number of turns left
			child->turns = gamep->turns - 1;
	
			// Copy the current board state to the child board state
			child->board = gamep->board;
			child->cheese_x = gamep->cheese_x;
			child->cheese_y = gamep->cheese_y;

			// set the new coordinates
			if (gamep->player == CAT) {
				// The mouse doesn't move
				child->mouse_x = gamep->mouse_x;
				child->mouse_y = gamep->mouse_y;
				switch (i) {
					case UP:
						child->cat_x = gamep->cat_x;
						child->cat_y = gamep->cat_y + 1;
						break;
					case RIGHT:
						child->cat_x = gamep->cat_x + 1;
						child->cat_y = gamep->cat_y;
						break;
					case DOWN:
						child->cat_x = gamep->cat_x;
						child->cat_y = gamep->cat_y - 1;
						break;
					case LEFT:
						child->cat_x = gamep->cat_x - 1;
						child->cat_y = gamep->cat_y;
						break;
				}
			}
			else {
				// The cat doesn't move
				child->cat_x = gamep->cat_x;
				child->cat_y = gamep->cat_y;
				switch (i) {
					case UP:
						child->mouse_x = gamep->mouse_x;
						child->mouse_y = gamep->mouse_y + 1;
						break;
					case RIGHT:
						child->mouse_x = gamep->mouse_x + 1;
						child->mouse_y = gamep->mouse_y;
						break;
					case DOWN:
						child->mouse_x = gamep->mouse_x;
						child->mouse_y = gamep->mouse_y - 1;
						break;
					case LEFT:
						child->mouse_x = gamep->mouse_x - 1;
						child->mouse_y = gamep->mouse_y;
						break;
				}
			}
			// Add child to the linked list
			tempList = append(children, child);
			LIFT(tempList); // malloc failed inside append()
		}
	}
	return children;
}

/** checks if game has end
 * returns negative if game hasn't ended yet
 */
int check_end_game(Game* gamep) {
	int dx[] = {0,1,0,-1};
	int dy[] = {1,0,-1,0};
	for (int i = 0; i < 4; i++) {
		//check if cat won
		if (gamep->cat_x + dx[i] == gamep->mouse_x) {
			if (gamep->cat_y + dy[i] == gamep->mouse_y) {
				return CAT_WIN;
			}
		}
		//check if mouse won
		if (gamep->cheese_x + dx[i] == gamep->mouse_x) {
			if (gamep->cheese_y + dy[i] == gamep->mouse_y) {
				return MOUSE_WIN;
			}
		}
	}
	//tie, no more turns left
	if (gamep->turns == 0) {
		return TIE;
	}
	return -1;
}
