#include "actions.h"

int quit_action(Widget* widget, game_state* state) {
	return 1;
}

int back_action(Widget* widget, game_state* state) {
	*state = *(state->previous_state);
	return 0;
}

int new_game_action(Widget* widget, game_state* state) {
	if (state == NULL) {
		return ERROR_NO_STATE;
	}
	game_state* old_state = (game_state*) malloc (sizeof(game_state));
	if (old_state == NULL) {
		return ERROR_MALLOC_FAILED;
	}
	memcpy(old_state, state, sizeof(game_state));
	state->previous_state = old_state;
	state->type = CHOOSE_PLAYER;
	state->focused = 0;
	state->catormouse = CAT;
	return 0;
}
