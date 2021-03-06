#ifndef _EVALUATE_H_
#define _EVALUATE_H_

#include <stdlib.h>
#include "../main/ListUtils.h"
#include "../core/board.h"
#include "../error/error.h"
#include <stdio.h>
#include <math.h>

#define FREE -1
#define INFTY 100
#define MAGIC_NUMBER 100001

#ifndef __BYTE__
#define __BYTE__
typedef unsigned char byte;
#endif

typedef struct coord {
	byte x;
	byte y;
} coord;

int evaluateGame(void* game);

#endif
