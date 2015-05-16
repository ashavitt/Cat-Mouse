#include "evaluate.h"

/** checks if [x][y] is a valid place ignoring cat/mouse/cheese
 *
 */
int node_is_valid(struct board* board, byte x, byte y) {
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
	return 1;
}

/** BFS algorithm
 * runs in O(|E|+|V|) = O(n^2)
 */
int bfs(struct board* board, byte start_x, byte start_y, byte dest_x, byte dest_y) {
	byte dx[] = {0,1,0,-1};
	byte dy[] = {-1,0,1,0};
	char sign[BOARD_SIZE][BOARD_SIZE];
	char depth = 0;
	coord *start = (coord*) malloc (sizeof(coord));
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
			if (node_is_valid(board, temp2->x, temp2->y)) {
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
			//printf("%d eval: %d\n", game->turns, -50*INFTY - game->turns);
			return -50*INFTY - game->turns;
		} else if (end_game_val == CAT_WIN) {
			//printf("%d eval: %d\n", game->turns, 50*INFTY + game->turns);
			return 50*INFTY + game->turns;
		} else if (end_game_val == TIE) {
			//printf("%d eval: %d\n", game->turns, 0);
			return 0;
		}
	}
	mouse_cheese = bfs(game->board, game->mouse_x, game->mouse_y, game->cheese_x, game->cheese_y);
	cat_cheese = bfs(game->board, game->cat_x, game->cat_y, game->cheese_x, game->cheese_y);
	cat_mouse = bfs(game->board, game->cat_x, game->cat_y, game->mouse_x, game->mouse_y);
	int num_of_walls = get_num_of_walls(game->board, game->mouse_x, game->mouse_y);
	int euclidean_dist = get_euclidean_dist(game->cat_x, game->cat_y, game->mouse_x, game->mouse_y);
	// check if the mouse can get to the cheese
	int turns_left_mouse = (game->player == MOUSE) ? (game->turns / 2 + 1) : (game->turns / 2);
	//printf("mouse_cheese: %d, cat_cheese %d, cat_mouse %d, euclidean_dist %d, turns_left_mouse %d\n", mouse_cheese, cat_cheese, cat_mouse, euclidean_dist, turns_left_mouse);
	// if the mouse can't get to the cheese we can ignore the distance between the mouse and the cheese
	if (turns_left_mouse < mouse_cheese) {
		//printf("%d eval: %d\n", game->turns, (20*cat_cheese - 6*cat_mouse + num_of_walls - euclidean_dist));
		return (-6*cat_mouse) + num_of_walls - euclidean_dist + 10*turns_left_mouse;
	}
	// if the cat can still outrun the mouse to the cheese it should go towards the mouse
	if (mouse_cheese - 2 >= cat_cheese) {
		//printf("%d eval: %d\n", game->turns, (-6*cat_mouse) + num_of_walls - euclidean_dist);
		return (-6*cat_mouse) + num_of_walls - euclidean_dist;
	}

	//TODO define params
	//printf("%d eval: %d\n", game->turns, 10*mouse_cheese -20*cat_cheese - 6*cat_mouse + num_of_walls - euclidean_dist);
	return 10*mouse_cheese -20*cat_cheese - 6*cat_mouse + num_of_walls - euclidean_dist;
}
