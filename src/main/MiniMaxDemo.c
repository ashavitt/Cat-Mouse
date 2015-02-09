#include <stdio.h>
#include <stdlib.h>
#include "MiniMax.h"

static const int evaluations[] = {50, 50, 50, 50, -4, 10, 12, -3, 2, 5};

int evaluate(void* state) {
	return evaluations[*(int*) state];
}

ListRef appendNumber(ListRef list, int number) {
	int* p = malloc(sizeof(int));
	*p = number;

	// this may actually return NULL if inner malloc call fails!
	return append(list, p);
}

ListRef getChildren(void* state) {
	int actualState = *(int*) state;

	// this may actually return NULL if inner malloc call fails!
	ListRef list = newList(NULL);
	ListRef tempList = NULL;

	switch (actualState) {
		case 0:
			tempList = appendNumber(list, 1);
			tempList = appendNumber(tempList, 2);
			appendNumber(tempList, 3);
			break;

		case 1:
			tempList = appendNumber(list, 4);
			appendNumber(tempList, 5);
			break;

		case 2:
			appendNumber(list, 6);
			break;

		case 3:
			tempList = appendNumber(list, 7);
			tempList = appendNumber(tempList, 8);
			appendNumber(tempList, 9);
			break;
	}

	return list;
}

int main() {
	int root = 0;
	struct MiniMaxResult best = getBestChild(&root, 2, getChildren, free, evaluate, 0);
	printf("The best child is at position %d with value %d.\n", best.index, best.value);
	return 0;
}
