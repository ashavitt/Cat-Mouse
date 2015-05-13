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

int start_game_action(Widget* widget, game_state* state) { // TODO main menu freeing?
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

int new_game_action(Widget* widget, game_state* state) { 
	state->world_id = 1;
	Game* game = load_world(state->world_id);
	state->game = game;
	return start_game_action(widget, state);
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
	state->number = DEFAULT_WORLD_INDEX;
	return 0;
}

int save_game_action(Widget* widget, game_state* state) {
	game_state* old_state = (game_state*) malloc (sizeof(game_state));
	if (state == NULL) {
		return ERROR_NO_STATE;
	}
	if (old_state == NULL) {
		return ERROR_MALLOC_FAILED;
	}
	memcpy(old_state, state, sizeof(game_state));
	state->previous_state = old_state;
	state->type = SAVE_GAME;
	state->focused = 0;
	state->number = DEFAULT_WORLD_INDEX;
	return 0;
}

int reconf_action(Widget* widget, game_state* state) {
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
	state->focused = 0; // set?
	
	if (widget->id == RECONF_MOUSE_B) {
		state->catormouse = MOUSE;
		if (state->game->num_steps_mouse == 0) {
			// TODO set focus to human button
			// state->focused = 0;
			// Already done..
			state->type = CHOOSE_PLAYER;
		} else { // machine
			state->type = CHOOSE_PLAYER;
			state->focused = 1; // machine is focused
			old_state = (game_state*) malloc (sizeof(game_state));
			if (state == NULL) {
				return ERROR_NO_STATE;
			}
			if (old_state == NULL) {
				return ERROR_MALLOC_FAILED;
			}
			memcpy(old_state, state, sizeof(game_state));
			state->previous_state = old_state;
			
			state->focused = 0;
			state->type = CHOOSE_SKILL;
			state->number = state->game->num_steps_mouse;
		}
		// maybe prior set to focus / number! TODO
	} else { // widget->id == RECONF_CAT_B
		state->catormouse = CAT;
		if (state->game->num_steps_cat == 0) {
			// state->focused = 0; // human
			// TODO set focus to human button
			state->type = CHOOSE_PLAYER;
		} else { // machine
			state->type = CHOOSE_PLAYER;
			state->focused = 1; // machine is focused
			old_state = (game_state*) malloc (sizeof(game_state));
			if (state == NULL) {
				return ERROR_NO_STATE;
			}
			if (old_state == NULL) {
				return ERROR_MALLOC_FAILED;
			}
			memcpy(old_state, state, sizeof(game_state));
			state->previous_state = old_state;
			
			state->focused = 0;
			state->type = CHOOSE_SKILL;
			state->number = state->game->num_steps_cat;
		}
	}

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
				//printf("state->previous_state->type: %d\n", state->previous_state->type);
				//printf("state->previous_state->previous_state->type: %d\n", state->previous_state->previous_state->type);
				if (old_state->previous_state->type == IN_GAME) {
					free_prev_states(state); // frees previous states before entering game
					state->type = IN_GAME;
					state->catormouse = PLAYING; // Maybe should be different
				} else { // proceed to cat selection
					state->catormouse = MOUSE;
				}
				state->game->num_steps_cat = 0; // HUMAN is 0 steps
				// assuming state is OK?
			} else { // machine_b
				state->type = CHOOSE_SKILL;
				state->number = DEFAULT_SKILL_LEVEL; //start skill level choosing at 5
				if (old_state->previous_state->type == IN_GAME && state->game->num_steps_cat != 0) { // TODO if setting default_skill_level in new game than this can change..
					state->number = state->game->num_steps_cat;
				}
				// still cat
			}
		} else { // mouse
			if (widget->id == HUMAN_B) {
				free_prev_states(state); // frees previous states before entering game
				state->game->num_steps_mouse = 0; // human is 0 steps
				state->type = IN_GAME;
				state->catormouse = PLAYING;
			} else { // machine_b
				state->type = CHOOSE_SKILL;
				state->number = DEFAULT_SKILL_LEVEL; //start skill level choosing at 5
				if (old_state->previous_state->type == IN_GAME && state->game->num_steps_cat != 0) {
					state->number = state->game->num_steps_mouse;
				}
				//still mouse
			}
		} 
	} else if (state->type == CHOOSE_SKILL) {
		if (state->catormouse == CAT) {
			if (old_state->previous_state->previous_state->type == IN_GAME) {
				free_prev_states(state); // frees previous states before entering game
				state->type = IN_GAME;
				state->catormouse = PLAYING; // Maybe should be different
			} else { // proceed to mouse selection
				state->type = CHOOSE_PLAYER;
				state->catormouse = MOUSE;
			}
			state->game->num_steps_cat = state->number;
		} else { // mouse
			free_prev_states(state); // frees previous states before entering game
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

		return start_game_action(widget, state);
	} else if (state->type == SAVE_GAME) {
		game_state* old_state = state->previous_state;
		old_state->world_id = state->number;
		// save the world to file
		if (save_world(old_state->world_id, state->game) != 0) {
			//TODO error code
			return 2;
		}
		// free this state
		free(state);
		state = old_state;
	} else if (state->type == EDIT_GAME) {
		state->type = GAME_EDIT;
		state->world_id = state->number;
		state->game = load_world(state->world_id);
	}
	return 0;
}

int do_nothing_action(Widget* widget, game_state* state) {
	return 0;
}

int restart_game_action(Widget* widget, game_state* state) {
	int num_steps_cat = state->game->num_steps_cat, num_steps_mouse = state->game->num_steps_mouse;
	game_free(state->game);
	state->game = load_world(state->world_id);
	state->catormouse = PLAYING;
	state->game->num_steps_cat = num_steps_cat;
	state->game->num_steps_mouse = num_steps_mouse;
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
	/*state->game = load_world(state->world_id); // FIXME
	if (state->game == NULL) {
		printf("Failed loading world number: %d\n", state->world_id);
		return 1; // TODO ERROR
	}*/
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

int game_editing_action(game_state* state, SDLKey key) {
	switch (key) {
		case SDLK_UP:
			if (GET_Y(state->focused) != 0) {
				state->focused -= 1;
			}
			break;
		case SDLK_RIGHT:
			if (GET_X(state->focused) != BOARD_SIZE-1) {
				state->focused += 10;
			}
			break;
		case SDLK_DOWN:
			if (GET_Y(state->focused) != BOARD_SIZE-1) {
				state->focused += 1;
			}
			break;
		case SDLK_LEFT:
			if (GET_X(state->focused) != 0) {
				state->focused -= 10;
			}
			break;
		default:
			//TODO error code
			return 1;
	}
	return 0;
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

int grid_edit_mouse_action(Widget* fake_widget, game_state* state) {
	SDL_Rect mouse_pos = fake_widget->pos; // mouse position, relative to board
	int x = mouse_pos.x / (GRID_W / 7);
	int y = mouse_pos.y / (GRID_H / 7);
	state->focused = 10*x+y;
	return 0;
}

int create_game_action(Widget* widget, game_state* state) {
	game_state* old_state = (game_state*) malloc (sizeof(game_state));
	if (state == NULL) {
		return ERROR_NO_STATE;
	}
	if (old_state == NULL) {
		return ERROR_MALLOC_FAILED;
	}
	memcpy(old_state, state, sizeof(game_state));
	// TODO free previous game?
	state->game = load_world(state->world_id);
	state->previous_state = old_state;
	state->type = GAME_EDIT;
	state->focused = 0;

	Game* game = game_malloc();
	if (game == NULL) {
		//TODO print error
		return 1;
	}
	game->cat_x = BOARD_SIZE;
	game->cat_y = BOARD_SIZE;
	game->mouse_x = BOARD_SIZE;
	game->mouse_y = BOARD_SIZE;
	game->cheese_x = BOARD_SIZE;
	game->cheese_y = BOARD_SIZE;
	//TODO magic number
	state->game->turns = 20;
	state->world_id = 0;
	state->game = game;
	return 0;
}

int edit_game_action(Widget* widget, game_state* state) {
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
	state->type = EDIT_GAME;
	state->focused = 0;
	return 0;
}

int empty_place(game_state* state, int x, int y) {
	if (state->game->cat_x == x && state->game->cat_y == y) {
		state->game->cat_x = BOARD_SIZE;
		state->game->cat_y = BOARD_SIZE;
	}
	if (state->game->mouse_x == x && state->game->mouse_y == y) {
		state->game->mouse_x = BOARD_SIZE;
		state->game->mouse_y = BOARD_SIZE;
	}
	if (state->game->cheese_x == x && state->game->cheese_y == y) {
		state->game->cheese_x = BOARD_SIZE;
		state->game->cheese_y = BOARD_SIZE;
	}
	return 0;
}

int place_wall_action(Widget* widget, game_state* state) {
	state->game->board->board[GET_X(state->focused)][GET_Y(state->focused)] = WALL;
	empty_place(state, GET_X(state->focused), GET_Y(state->focused));
	return 0;
}

int place_empty_action(Widget* widget, game_state* state) {
	state->game->board->board[GET_X(state->focused)][GET_Y(state->focused)] = EMPTY;
	empty_place(state, GET_X(state->focused), GET_Y(state->focused));
	return 0;
}

int place_cat_action(Widget* widget, game_state* state) {
	//state->game->board->board[state->game->cat_x][state->game->cat_y] = EMPTY;
	empty_place(state, GET_X(state->focused), GET_Y(state->focused));
	state->game->cat_x = GET_X(state->focused);
	state->game->cat_y = GET_Y(state->focused);
	state->game->board->board[state->game->cat_x][state->game->cat_y] = EMPTY;
	//set the first player
	state->game->player = CAT;
	return 0;
}

int place_mouse_action(Widget* widget, game_state* state) {
	//state->game->board->board[state->game->mouse_x][state->game->mouse_y] = EMPTY;
	empty_place(state, GET_X(state->focused), GET_Y(state->focused));
	state->game->mouse_x = GET_X(state->focused);
	state->game->mouse_y = GET_Y(state->focused);
	state->game->board->board[state->game->mouse_x][state->game->mouse_y] = EMPTY;
	//set the first player
	state->game->player = MOUSE;
	return 0;
}

int place_cheese_action(Widget* widget, game_state* state) {
	//state->game->board->board[state->game->cheese_x][state->game->cheese_y] = EMPTY;
	empty_place(state, GET_X(state->focused), GET_Y(state->focused));
	state->game->cheese_x = GET_X(state->focused);
	state->game->cheese_y = GET_Y(state->focused);
	state->game->board->board[state->game->cheese_x][state->game->cheese_y] = EMPTY;
	return 0;
}
