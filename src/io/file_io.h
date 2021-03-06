#ifndef FILE_IO_H_
#define FILE_IO_H_

/* Includes */
#include <stdio.h>
#include "../core/board.h"
#include <string.h>
#include <stdlib.h>

/* Defines */
// Buffer Length
#define BFLEN 16
#define CONSOLE_QUIT_CODE 10

/* Macros */
#define CHECK(num) if((num)<0) {return (num);}

/* Functions */

int save_game(Game* gamep, FILE* file);

int load_game(Game* gamep, FILE* file);

Game* load_world(int id);

int save_world(int id, Game* game);
#endif /* FILE_IO_H_ */
