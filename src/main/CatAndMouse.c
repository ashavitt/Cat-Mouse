#include "CatAndMouse.h"

int main(int argc, char* argv[]) {
	// check if console mode
	if (argc == 2) {
		if (!strcmp(argv[1], "-console")) {
			printf("q\n");
		} else {
			return run_gui();
		}
	} else {
		return run_gui();
	}
	return 0;
}
