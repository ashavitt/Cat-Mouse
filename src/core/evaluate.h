#include <stdlib.h>
#include "../main/ListUtils.h"
#include "../core/board.h"
#include "../error/error.h"
#include <stdio.h>
#include <math.h>

#define FREE -1
#define INFTY 100000

#ifndef __BYTE__
#define __BYTE__
typedef unsigned char byte;
#endif

typedef struct coord {
	byte x;
	byte y;
} coord;

int evaluateGame(void* game);
