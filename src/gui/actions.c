#include "actions.h"

onclick quit_action(Widget* widget, game_state* state) {
	return 1;
}

onclick back_action(Widget* widget, game_state* state) {
	*state = *(state->previous_state);
	return 0;
}
