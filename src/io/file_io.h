#ifndef FILE_IO_H_
#define FILE_IO_H_

/* Includes */
#include <stdio.h>
#include "board.h"
#include <string.h>

/* Macros */

#define CHECK(num) if((num)<0) {return (num);}

/* Functions */

int save_game(Game* game, FILE* file);

int load_game(Game* game, FILE* file);
