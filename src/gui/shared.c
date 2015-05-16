#include "shared.h"

/*** shared.c
 * General functions for use in gui
 ***/

/** State Freeing
 * Frees the given state's predecessors
 * Two versions: one gets the actual state, one gets a pointer
 **/
void free_state(game_state state) { // doesn't free state!!
	if (state.previous_state != NULL) { // if the previous state is not NULL
		free_state(*(state.previous_state)); // free it's predecessors
	}
	free(state.previous_state); // free it
	state.previous_state = NULL; // set pointer to NULL
	return;
}
void free_prev_states(game_state* state) {
	if (state == NULL) { // state points to NULL so we do nothing
		return;
	}
	game_state* p = state->previous_state, *q;
	while (p != NULL) { // run over previous states and free them
		q = p->previous_state;			
		free(p);
		p = q;
	}
	state->previous_state = NULL; // no there is no previous state
	return;
}
