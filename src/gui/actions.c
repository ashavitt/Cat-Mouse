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

int pause_resume_action(Widget* widget, game_state* state) {
	state->catormouse = (state->catormouse == PLAYING) ? PAUSED : PLAYING;
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

int load_game_action(Widget* widget, game_state* state) {
	game_state* old_state = (game_state*) malloc (sizeof(game_state));
	if (state == NULL) {
		return ERROR_NO_STATE;
	}
	if (old_state == NULL) {
		return ERROR_MALLOC_FAILED;
	}
	memcpy(old_state, state, sizeof(game_state));
	state->previous_state = old_state;
	state->type = LOAD_GAME;
	state->focused = 0;
	state->game = old_state->game;
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
				if (state->previous_state->type == IN_GAME) {
					free_state(*state); // frees previous states before entering game
					state->type = IN_GAME;
					state->catormouse = PLAYING; // Maybe should be different
				} else { // proceed to cat selection
					state->catormouse = MOUSE;
				}
				state->game->num_steps_cat = 0; // HUMAN is 0 steps
				// assuming state is OK?
			} else { // machine_b
				state->type = CHOOSE_SKILL;
				state->number = 1; //start skill level choosing at 1
				// TODO start skill level at state->game->num_steps_cat
				// still cat
			}
		} else { // mouse
			if (widget->id == HUMAN_B) {
				free_state(*state); // frees previous states before entering game
				state->game->num_steps_mouse = 0; // human is 0 steps
				state->type = IN_GAME;
				state->catormouse = PLAYING;
			} else { // machine_b
				state->type = CHOOSE_SKILL;
				state->number = 1; //start skill level choosing at 1
				//still mouse
			}
		}
	} else if (state->type == CHOOSE_SKILL) {
		if (state->catormouse == CAT) {
			if (state->previous_state->previous_state->type == IN_GAME) {
				free_state(*state); // frees previous states before entering game
				state->type = IN_GAME;
				state->catormouse = PLAYING; // Maybe should be different
			} else { // proceed to mouse selection
				state->type = CHOOSE_PLAYER;
				state->catormouse = MOUSE;
			}
			state->game->num_steps_cat = state->number;
		} else { // mouse
			free_state(*state); // frees previous states before entering game
			state->game->num_steps_mouse = state->number;
			state->type = IN_GAME;
			state->catormouse = PLAYING;
		}
	} else if (state->type == LOAD_GAME) {
		state->world_id = state->number;
		Game* game = load_world(state->world_id);
		// free only the board inside the original game
		free(state->game->board);
		// copy the new game to the original one
		memcpy(state->game, game, sizeof(Game));
		// free only the game without it's board
		free(game);

		return new_game_action(widget, state);
	} else if (state->type == IN_GAME) {
		if (widget->id == RECONF_MOUSE_B) {
			state->catormouse = MOUSE;
			// maybe prior set to focus / number! TODO
		} else { // widget->id == RECONF_CAT_B
			state->catormouse = CAT;
		}
		state->type = CHOOSE_PLAYER;
	}
	
	return 0;
}

int do_nothing_action(Widget* widget, game_state* state) {
	return 0;
}

int restart_game_action(Widget* widget, game_state* state) {
	game_free(state->game);
	state->game = load_world(state->world_id);
	state->catormouse = PLAYING;
	if (state->game == NULL) {
		printf("Failed loading world number: %d\n", state->world_id);
		return 1; // TODO ERROR
	}
	return 0;
}

int goto_main_menu_action(Widget* widget, game_state* state) {
	free_state(*state); // frees previous states
	state->type = MAIN_MENU;
	state->focused = 0;
	state->previous_state = NULL;
	game_free(state->game);
	state->game = load_world(state->world_id);
	if (state->game == NULL) {
		printf("Failed loading world number: %d\n", state->world_id);
		return 1; // TODO ERROR
	}
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

int play_turn(int dir, game_state* state) {
	if (dir_is_valid(state->game, dir)) {
		move_obj(state->game, state->game->player, dir);
		state->game->player = SWITCH(state->game->player);
		state->game->turns--;
	}
	return 0;
}

int in_game_action(game_state* state, SDLKey key) {
	if (state->catormouse != PLAYING) {
		return 0;
	}
	int dir;
	if (!((state->game->player == CAT && state->game->num_steps_cat == 0) || (state->game->player == MOUSE && state->game->num_steps_mouse == 0))) {
		return 0;
	}
	switch (key) {
		case SDLK_UP:
			dir = UP;
			break;
		case SDLK_RIGHT:
			dir = RIGHT;
			break;
		case SDLK_DOWN:
			dir = DOWN;
			break;
		case SDLK_LEFT:
			dir = LEFT;
			break;
		default:
			//TODO error code
			return 1;
	}
	return play_turn(dir, state);
}

int grid_mouse_action(Widget* fake_widget, game_state* state) {
	if (state->catormouse != PLAYING) {
		return 0;
	}
	SDL_Rect mouse_pos = fake_widget->pos; // mouse position, relative to board
	int x,y;
	int dx[] = {0,1,0,-1};
	int dy[] = {-1,0,1,0};
	x = mouse_pos.x / (GRID_W / 7);
	y = mouse_pos.y / (GRID_H / 7);
	for (int i=0; i<4; ++i) {
		if (state->game->player == CAT && state->game->num_steps_cat == 0){
			if (state->game->cat_x + dx[i] == x && state->game->cat_y + dy[i] == y) {
				return play_turn(i, state);
			}
		} else if (state->game->player == MOUSE && state->game->num_steps_mouse == 0) {
			if (state->game->mouse_x + dx[i] == x && state->game->mouse_y + dy[i] == y) {
				return play_turn(i, state);
			}
		}
	}
	
	return 0; // nothing happens
}
