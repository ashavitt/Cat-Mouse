#include "shortest_path.h"

// FIXME: DEPRECATED
inline int max (int a, int b) { return a > b ? a : b; }

/** Manhattan Distance implemented
 *  FIXME: DEPRECATED
 */
int heuristic(byte x_1, byte y_1, byte x_2, byte y_2) {
	return max(abs(x_1-x_2),abs(y_1-y_2));
}

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
	byte dy[] = {1,0,-1,0};
	char sign[BOARD_SIZE][BOARD_SIZE];
	char depth = 0;
	coord start;
	coord* temp1, temp2;
	ListRef queue = newList(&start);
	start.x = start_x;
	start.y = start_y;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			sign[i][j] = FREE;
		}
	}
	sign[start_x][start_y] = 0;
	while (!isEmpty(queue)) {
		//dequeue
		temp1 = headData(queue);
		queue = tail(queue);
		//if we found the destination
		depth = sign[temp1->x][temp1->y];
		if (temp1->x == dest_x && temp1->y == dest_y) {
			return depth;
		}
		for (int i = 0; i < 4; i++) {
			temp2 = (coord*) malloc (sizeof(coord));
			if (temp2 == NULL) {
				perror("Error: malloc failed.\n");
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
				}
			}
		}
	}
	return INFINITY;
}
