#include "actions.h"

int quit_action(Widget* widget, game_state* state) {
	return 1;
}

int back_action(Widget* widget, game_state* state) {
	*state = *(state->previous_state);
	return 0;
}
