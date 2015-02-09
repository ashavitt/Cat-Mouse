#include <stdio.h>
#include <stdlib.h>
#include "ListUtils.h"

void printList(ListRef list) {
	if (isEmpty(list)) {
		printf("List is empty.\n");
	}
	else {
		do {
			printf("%d ", *(int*) headData(list));
			list = tail(list);
		} while (list != NULL);

		printf("\n");
	}
}

ListRef createList(int n) {
	// this may actually return NULL if inner malloc call fails!
	ListRef list = newList(NULL);

	ListRef tempList = list;
	for (int i = 0; i < n; ++i) {
		int* data = malloc(sizeof(int));
		*data = i;

		// this may return NULL as well:
		tempList = append(tempList, data);
	}

	return list;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("The number of elements must be given via command line argument.\n");
		return 1;
	}

	int n;
	sscanf(argv[1], "%d", &n);

	ListRef list = createList(n);
	printList(list);
	destroyList(list, free);

	return 0;
}

