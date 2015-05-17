#include <stdlib.h>
#include <time.h>

srand(time(NULL));

void* nalloc(int size) {
	if (rand() > 0.9) {
		return NULL;
	}
	return malloc(size);
}
