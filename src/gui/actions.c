#include "actions.h"

int quit_action(Widget* widget, game_state* state) {
	return 1;
}

int back_action(Widget* widget, game_state* state) {
	game_state temp_state = *(state->previous_state);
	free(state->previous_state);
	*state = temp_state;
	return 0;
}

int new_game_action(Widget* widget, game_state* state) {
	game_state* old_state = (game_state*) malloc (sizeof(game_state));
	if (state == NULL) {
		return ERROR_NO_STATE;
	}
	if (old_state == NULL) {
		return ERROR_MALLOC_FAILED;
	}
	memcpy(old_state, state, sizeof(game_state));
	// TODO free previous game?
	//state->game = game_malloc(); // TODO should be function that loads some world
	state->previous_state = old_state;
	state->type = CHOOSE_PLAYER;
	state->focused = 0;
	state->catormouse = CAT;
	return 0;
}


int choose_action(Widget* widget, game_state* state) {
	game_state* old_state = (game_state*) malloc (sizeof(game_state));
	if (state == NULL) {
		return ERROR_NO_STATE;
	}
	if (state->game == NULL) {
		return ERROR_NO_GAME;
	}
	if (old_state == NULL) {
		return ERROR_MALLOC_FAILED;
	}
	memcpy(old_state, state, sizeof(game_state));
	state->previous_state = old_state;

	state->focused = 0;
	
	if (state->type == CHOOSE_PLAYER) {
		if (state->catormouse == CAT) {
			if (widget->id == HUMAN_B) {
				state->catormouse = MOUSE;
				state->game->num_steps_cat = 0; // HUMAN is 0 steps
				// assuming state is OK?
			} else { // machine_b
				state->type = CHOOSE_SKILL;
				state->number = 1; //start skill level choosing at 1
				// still cat
			}
		} else { // mouse
			if (widget->id == HUMAN_B) {
				state->game->num_steps_mouse = 0; // human is 0 steps
				state->type = IN_GAME;
			} else { // machine_b
				state->type = CHOOSE_SKILL;
				state->number = 1; //start skill level choosing at 1
				//still mouse
			}
		}
	} else if (state->type == CHOOSE_SKILL) {
		if (state->catormouse == CAT) {
			state->game->num_steps_cat = state->number;
			state->type = CHOOSE_PLAYER;
			state->catormouse = MOUSE;
		} else { // mouse
			state->game->num_steps_mouse = state->number;
			state->type = IN_GAME;
		}
	}
	
	return 0;
}

int do_nothing_action(Widget* widget, game_state* state) {
	return 0;
}


int arrow_action(Widget* widget, game_state* state) {
	if (widget->id == LEVEL_UP_B) {
		if (state->type == CHOOSE_SKILL) {
			if (state->number >= NUM_OF_LEVELS) {
				return 0;
			}
		} else if (state->type == LOAD_GAME || state->type == SAVE_GAME || state->type == EDIT_GAME) {
			if (state->number >= NUM_OF_WORLDS){
				return 0;
			}
		}
		state->number++;
	} else if (widget->id == LEVEL_DN_B) {
		if (state->number <= 1) {
			return 0;
		}
		state->number--;
	}
	return 0;
}
