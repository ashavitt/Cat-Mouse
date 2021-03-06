#include "evaluate.h"

/** checks if [x][y] is a valid place ignoring cat/mouse/cheese
 *
 */
int node_is_valid(struct board* board, byte x, byte y, byte ignore_x, byte ignore_y) {
	if (x > BOARD_SIZE-1) {
		return 0;
	}
	if (y > BOARD_SIZE-1) {
		return 0;
	}
	if (x < 0) {
		return 0;
	}
	if (y < 0) {
		return 0;
	}
	if (board->board[x][y] == WALL) {
		return 0;
	}
	// we can set ignore coordinates
	// we use it to ignore the third example
	// e.g. we ignore the cheese between the cat and the mouse
	if (ignore_x == x && ignore_y == y) {
		return 0;
	}
	return 1;
}

/** BFS algorithm
 * runs in O(|E|+|V|) = O(n^2)
 */
int bfs(struct board* board, byte start_x, byte start_y, byte dest_x, byte dest_y, byte ignore_x, byte ignore_y) {
	byte dx[] = {0,1,0,-1};
	byte dy[] = {-1,0,1,0};
	char sign[BOARD_SIZE][BOARD_SIZE];
	char depth = 0;
	coord *start = (coord*) malloc (sizeof(coord));
	if (start == NULL) {
		fprintf(stderr, "Error: malloc failed.\n");
		return -1;
	}
	coord *temp1, *temp2;
	ListRef queue = newList(start);
	ListRef temp_queue;
	start->x = start_x;
	start->y = start_y;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			sign[i][j] = FREE;
		}
	}
	sign[start_x][start_y] = 0;
	while (!isEmpty(queue)) {
		//dequeue
		temp1 = headData(queue);
		//check if queue contains one element
		temp_queue = tail(queue);
		if (temp_queue == NULL) {
			free(queue);
			queue = newList(NULL);
		} else { //otherwise we remove the first obj in the list
			free(queue);
			queue = temp_queue;
		}
		//if we found the destination
		depth = sign[temp1->x][temp1->y];
		if (temp1->x == dest_x && temp1->y == dest_y) {
			destroyList(queue, free);
			free(temp1);
			return depth;
		}
		for (int i = 0; i < 4; i++) {
			temp2 = (coord*) malloc (sizeof(coord));
			if (temp2 == NULL) {
				fprintf(stderr, "Error: malloc failed.\n");
				destroyList(queue, free);
				free(temp1);
				return -1;
			}
			temp2->x = temp1->x + dx[i];
			temp2->y = temp1->y + dy[i];
			//first check the coordinates are valid
			if (node_is_valid(board, temp2->x, temp2->y, ignore_x, ignore_y)) {
				//check if the coordinates were searched already
				if (sign[temp2->x][temp2->y] == FREE) {
					//set the appropriate depth
					sign[temp2->x][temp2->y] = depth + 1;
					//add the node to the queue
					append(queue, temp2);
				} else {
					free(temp2);
				}
			} else {
				free(temp2);
			}
		}
		free(temp1);
	}
	destroyList(queue, free);
	return INFTY;
}

/** returns the number of walls (counting the board boundries)
  * this is used as a tie breaker for the mouse move */
int get_num_of_walls(struct board* board, byte x, byte y) {
	int count = 0;
	if (x + 1 == BOARD_SIZE) {
		count += 1;
	} else if (board->board[x+1][y] == WALL) {
		count += 1;
	}
	if (y + 1 == BOARD_SIZE) {
		count += 1;
	} else if (board->board[x][y+1] == WALL) {
		count += 1;
	}
	if (x == 0) {
		count += 1;
	} else if (board->board[x-1][y] == WALL) {
		count += 1;
	}
	if (y == 0) {
		count += 1;
	} else if (board->board[x][y-1] == WALL) {
		count += 1;
	}
	return count;
}

/** returns euclidean dist between two points */
int get_euclidean_dist(byte x1, byte y1, byte x2, byte y2) {
	return (int)sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

int evaluateGame(void* gamep) {
	Game* game = (Game*) gamep;
	int mouse_cheese, cat_mouse, cat_cheese, end_game_val;
	// first we check if game ended
	if ((end_game_val = check_end_game(gamep)) != -1) {
		if (end_game_val == MOUSE_WIN) {
			// we will prefer an early win
			return -50*INFTY - game->turns;
		} else if (end_game_val == CAT_WIN) {
			// we will prefer an early win
			return 50*INFTY + game->turns;
		} else if (end_game_val == TIE) {
			return 0;
		}
	}
	mouse_cheese = bfs(game->board, game->mouse_x, game->mouse_y, game->cheese_x, game->cheese_y, BOARD_SIZE, BOARD_SIZE);
	cat_cheese = bfs(game->board, game->cat_x, game->cat_y, game->cheese_x, game->cheese_y, BOARD_SIZE, BOARD_SIZE);
	cat_mouse = bfs(game->board, game->cat_x, game->cat_y, game->mouse_x, game->mouse_y, game->cheese_x, game->cheese_y);
	if (mouse_cheese == -1 || cat_cheese == -1 || cat_mouse == -1) {
		printf("Error: bfs failed.\n");
		return MAGIC_NUMBER; // this magically indicates that an error occured, (mainly malloc)
	}
	int num_of_walls = get_num_of_walls(game->board, game->mouse_x, game->mouse_y);
	int euclidean_dist = get_euclidean_dist(game->cat_x, game->cat_y, game->mouse_x, game->mouse_y);
	// check if the mouse can get to the cheese
	int turns_left_mouse = (game->player == MOUSE) ? (game->turns / 2 + 1) : (game->turns / 2);
	// if the mouse can't get to the cheese we can ignore the distance between the mouse and the cheese
	if (turns_left_mouse < mouse_cheese - 1) {
		return (-6*cat_mouse) + num_of_walls - euclidean_dist + 10*turns_left_mouse;
	}
	// if the cat can still outrun the mouse to the cheese it should go towards the mouse
	if (mouse_cheese - 2 >= cat_cheese) {
		return (-6*cat_mouse) + num_of_walls - euclidean_dist + 10*turns_left_mouse;
	}

	return 10*mouse_cheese -20*cat_cheese - 6*cat_mouse + num_of_walls - euclidean_dist;
}
