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
				printf("Error: malloc failed.\n");
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
	return INFINITY;
}

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
	if (x - 1 == 0) {
		count += 1;
	} else if (board->board[x-1][y] == WALL) {
		count += 1;
	}
	if (y - 1 == 0) {
		count += 1;
	} else if (board->board[x][y-1] == WALL) {
		count += 1;
	}
	return count;
}

int evaluateGame(void* gamep) {
	Game* game = (Game*) gamep;
	int mouse_cheese, cat_mouse, cat_cheese, end_game_val;
	if ((end_game_val = check_end_game(gamep)) != -1) {
		if (end_game_val == MOUSE_WIN) {
			return -10*INFINITY + game->turns;
		} else if (end_game_val == CAT_WIN) {
			return 10*INFINITY - game->turns;
		} else if (end_game_val == TIE) {
			return 0;
		}
	}
	mouse_cheese = bfs(game->board, game->mouse_x, game->mouse_y, game->cheese_x, game->cheese_y);
	cat_cheese = bfs(game->board, game->cat_x, game->cat_y, game->cheese_x, game->cheese_y);
	cat_mouse = bfs(game->board, game->cat_x, game->cat_y, game->mouse_x, game->mouse_y);
	int num_of_walls = get_num_of_walls(game->board, game->mouse_x, game->mouse_y);
	//TODO define params
	return 20*mouse_cheese -20*cat_cheese - 6*cat_mouse + num_of_walls;
}
