#include "shared.h"


void free_state(game_state state) { // doesn't free state!!
	if (state.previous_state != NULL) {
		free_state(*(state.previous_state));
	}
	free(state.previous_state);
	return;
}