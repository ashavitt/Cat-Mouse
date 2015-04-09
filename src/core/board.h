#ifndef BOARD_H_
#define BOARD_H_

/* Includes */
#include "../main/ListUtils.h"
#include <stdlib.h>

/* Defines */
#ifndef __BYTE__
#define __BYTE__
typedef unsigned char byte;
#endif

#define WALL 'W'
#define EMPTY '#'
#define CAT 'C'
#define MOUSE 'M'
#define CHEESE 'P'

#define BOARD_SIZE 7

//directions for the cat&mouse to move
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

//results to check_end_game
#define CAT_WIN 2
#define MOUSE_WIN 1
#define TIE 0


/* Macros */
#define LIFT(x) if ((x) == NULL) { return NULL; }
#define SWITCH(x) ((x) == CAT) ? MOUSE : CAT

/* Structures */

/** Game Board
 *
 */
struct board {
	/** Constant 7*7 size
	 ____________
	|[0,6]	[6,6]|
	|	     |
	|	     |
	|[0,0]	[6,0]|
	 ------------
	*/
	char board[BOARD_SIZE][BOARD_SIZE];
}; 

/** Game State
 *
 *
 */
typedef struct Game {
	// pointer to the board (constant)
	struct board* board;
	// cat mouse cheese coordinates
	// 7 or larger for doesn't appear
	byte cat_x;
	byte cat_y;
	byte mouse_x;
	byte mouse_y;
	byte cheese_x;
	byte cheese_y;
	// who's turn is it
	char player;
	// number of steps left
	byte turns;
	// number of steps in the minimax algorithm
	// 0 for human
	byte num_steps_cat;
	byte num_steps_mouse;
} Game;

/* Functions */

ListRef getChildren(void* gamep);

//int evaluateGame(Game* game);

int check_end_game(Game* gamep);

#endif /* GAME_UTILS_H_ */
