#include "shared.h"


void free_state(game_state state) { // doesn't free state!!
	if (state.previous_state != NULL) {
		free_state(*(state.previous_state));
	}
	free(state.previous_state);
	state.previous_state = NULL;
	return;
}

void free_prev_states(game_state* state) {
	if (state == NULL) {
		return;
	}
	game_state* p = state->previous_state, *q;
	while (p != NULL) {
		q = p->previous_state;			
		free(p);
		p = q;
	}
	state->previous_state = NULL;
	return;
}