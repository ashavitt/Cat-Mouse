#ifndef FILE_IO_H_
#define FILE_IO_H_

/* Includes */
#include <stdio.h>
#include "../structs/board.h"
#include <string.h>

/* Macros */

#define CHECK(num) if((num)<0) {return (num);}

/* Functions */

int save_game(game* gamep, FILE* file);

int load_game(game* gamep, FILE* file);
#endif /* FILE_IO_H_ */
