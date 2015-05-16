#include "actions.h"

/*** actions.c
 * This file includes action functions to be called by some GUI interaction.
 * All actions are formatted as:
 ** int action_name(Widget*,game_state*)
 * Usually returns 0 when OK and else when an error occured
 **/

/* Quit Game Action */
int quit_action(Widget* widget, game_state* state) {
	return 1; // 1 (specifically) will tell the main loop to quit the game cleanly
}

/* Go Back Action */
int back_action(Widget* widget, game_state* state) {
	game_state temp_state = *(state->previous_state); // save the previous state's data
	free(state->previous_state); // frees previous state
	*state = temp_state; // place the previous state instead of the current
	return 0;
}

/* Pause / Resume when IN_GAME */
int pause_resume_action(Widget* widget, game_state* state) {
	if (state->catormouse == PLAYING || state->catormouse == PAUSED) { // if someone has won, don't un/pause
		state->catormouse = (state->catormouse == PLAYING) ? PAUSED : PLAYING; // instructive
	}
	return 0;
}

/* Start game helper function
 * Not called from outside
 */
int start_game_action(Widget* widget, game_state* state) { // TODO main menu freeing?
	// Allocating space and adding the last state to the state stack
	game_state* old_state = (game_state*) malloc (sizeof(game_state));
	if (state == NULL) {
		fprintf(stderr, "Error: start_game_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	if (old_state == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return ERROR_MALLOC_FAILED;
	}
	memcpy(old_state, state, sizeof(game_state));
	// TODO free previous game?
	// we've already loaded the world in the function calling this
	state->previous_state = old_state;
	state->type = CHOOSE_PLAYER;
	state->focused = 0;
	state->catormouse = CAT; // TODO
	return 0;
}

/* New Game Action
 * Called by the New Game button in the main menu
 * Loads world 1 and starts the game
 */
int new_game_action(Widget* widget, game_state* state) { 
	if (state == NULL) {
		fprintf(stderr, "Error: new_game_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	state->world_id = 1;
	Game* game = load_world(state->world_id);
	if (game == NULL) {
		fprintf(stderr, "Error: load_world failed. world_id=%d\n",state->world_id);
		return ERROR_LOAD_WORLD_FAILED;
	}
	state->game = game;
	return start_game_action(widget, state);
}

/* Load Game Action
 * Starts the load game screen
 */
int load_game_action(Widget* widget, game_state* state) {
	// Allocating space and adding the last state to the state stack
	game_state* old_state = (game_state*) malloc (sizeof(game_state));
	if (old_state == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
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

/* Starts saving screen */
int save_game_action(Widget* widget, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: save_game_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	Game* game = state->game;
	game_state* old_state = (game_state*) malloc (sizeof(game_state));
	if (old_state == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return ERROR_MALLOC_FAILED;
	}
	memcpy(old_state, state, sizeof(game_state));
	state->previous_state = old_state;
	
	// Check if world is valid for saving:
	if (!(game->cat_x < BOARD_SIZE && game->cat_x >= 0) || !(game->cat_y < BOARD_SIZE && game->cat_y >= 0)) {
		state->number = CAT_IS_MISSING_IND;
		state->type = ERROR_DIALOG;
	} else if (!(game->mouse_x < BOARD_SIZE && game->mouse_x >= 0) || !(game->mouse_y < BOARD_SIZE && game->mouse_y >= 0)) {
		state->number = MOUSE_IS_MISSING_IND;
		state->type = ERROR_DIALOG;
	} else if (!(game->cheese_x < BOARD_SIZE && game->cheese_x >= 0) || !(game->cheese_y < BOARD_SIZE && game->cheese_y >= 0)) {
		state->number = CHEESE_IS_MISSING_IND;
		state->type = ERROR_DIALOG;
	} else { // world is valid
		state->type = SAVE_GAME;
		if (state->world_id == 0) {
			state->world_id = DEFAULT_WORLD_INDEX;
		}
		state->number = state->world_id;
	}
	state->focused = 0;
	return 0;
}

/* Starts reonconfigure state */
int reconf_action(Widget* widget, game_state* state) {
	game_state* old_state = (game_state*) malloc (sizeof(game_state));
	if (state == NULL) {
		fprintf(stderr, "Error: reconf_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	if (old_state == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return ERROR_MALLOC_FAILED;
	}
	memcpy(old_state, state, sizeof(game_state));
	state->previous_state = old_state;
	state->focused = 0; 
	
	if (widget->id == RECONF_MOUSE_B) {
		state->catormouse = MOUSE;
		if (state->game->num_steps_mouse == 0) { // human
			// state->focused = 0; // set at the top - human button
			state->type = CHOOSE_PLAYER;
		} else { // machine
			state->type = CHOOSE_PLAYER;
			state->focused = 1; // machine is focused
			// now we start another window on the stack, before showing the first one
			old_state = (game_state*) malloc (sizeof(game_state));
			if (old_state == NULL) {
				fprintf(stderr, "Error: malloc failed\n");
				return ERROR_MALLOC_FAILED;
			}
			memcpy(old_state, state, sizeof(game_state));
			state->previous_state = old_state;
			state->focused = 0;
			state->type = CHOOSE_SKILL;
			state->number = state->game->num_steps_mouse;
		}
	} else { // widget->id == RECONF_CAT_B
		state->catormouse = CAT;
		if (state->game->num_steps_cat == 0) { // human
			// state->focused = 0; // already set - human button
			state->type = CHOOSE_PLAYER;
		} else { // machine
			state->type = CHOOSE_PLAYER;
			state->focused = 1; // machine is focused
			// now we start another window on the stack, before showing the first one
			old_state = (game_state*) malloc (sizeof(game_state));
			if (old_state == NULL) {
				fprintf(stderr, "Error: malloc failed\n");
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

/* Action to be called when Done is pressed in either SAVE_GAME, LOAD_WORLD, EDIT_GAME, CHOOSE_SKILL, CHOOSE_PLAYER */
int choose_action(Widget* widget, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: choose_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	game_state* old_state;
	if (state->type == SAVE_GAME) { // we don't have to start a new window, so we handle SAVE_GAME first
		old_state = state->previous_state;
		old_state->world_id = state->number;
		// world validity already tested
		// save the world to file
		if (save_world(old_state->world_id, old_state->game) != 0) {
			fprintf(stderr, "Error: save_world failed\n");
			return ERROR_SAVE_WORLD_FAILED;
		}
		// free this state and go back to game editing
		memcpy(state, old_state, sizeof(game_state));
		free(old_state);
		return 0;
	}
	
	old_state = (game_state*) malloc (sizeof(game_state));
	if (old_state == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return ERROR_MALLOC_FAILED;
	}
	memcpy(old_state, state, sizeof(game_state));
	state->previous_state = old_state;
	state->focused = 0; // set focused for some of the windows
	if (state->type == CHOOSE_PLAYER) {
		if (state->catormouse == CAT) {
			if (widget->id == HUMAN_B) {
				if (old_state->previous_state->type == IN_GAME) {
					free_prev_states(state); // frees previous states before entering game
					state->type = IN_GAME;
					state->catormouse = PAUSED; // According to the forum
				} else { // proceed to cat selection
					state->catormouse = MOUSE;
				}
				state->game->num_steps_cat = 0; // HUMAN is 0 steps
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
				state->catormouse = PLAYING; // TODO FIXME!!!!!!!
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
				state->catormouse = PAUSED; // According to the forum
			} else { // proceed to mouse selection
				state->type = CHOOSE_PLAYER;
				state->catormouse = MOUSE;
			}
			state->game->num_steps_cat = state->number;
		} else { // mouse
			free_prev_states(state); // frees previous states before entering game
			state->game->num_steps_mouse = state->number;
			state->type = IN_GAME;
			state->catormouse = PLAYING;  // TODO FIXME!!!!!!!
		}
	} else if (state->type == LOAD_GAME) {
		state->world_id = state->number;
		Game* game = load_world(state->world_id);
		if (game == NULL) {
			fprintf(stderr, "Error: load_world failed. world_id=%d\n",state->world_id);
			return ERROR_LOAD_WORLD_FAILED;
		}
		state->game = game;
		return start_game_action(widget, state);
	} else if (state->type == EDIT_GAME) {
		state->type = GAME_EDIT;
		state->world_id = state->number;
		state->game = load_world(state->world_id);
		if (state->game == NULL) {
			fprintf(stderr, "Error: load_world failed. world_id=%d\n",state->world_id);
			return ERROR_LOAD_WORLD_FAILED;
		}
	}
	return 0;
}

/* Instructive */
int do_nothing_action(Widget* widget, game_state* state) {
	return 0;
}

/* Restart the game */
int restart_game_action(Widget* widget, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: restart_game_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	// saves current config of the game
	int num_steps_cat = state->game->num_steps_cat, num_steps_mouse = state->game->num_steps_mouse;
	game_free(state->game); // frees the game
	state->game = load_world(state->world_id); // reloads the game from file
	if (state->game == NULL) {
		fprintf(stderr, "Failed loading world number: %d\n", state->world_id);
		return 1; // TODO ERROR
	}
	state->catormouse = PLAYING;
	state->game->num_steps_cat = num_steps_cat; // sets the config like before
	state->game->num_steps_mouse = num_steps_mouse;
	return 0;
}

/* Go to main menu */
int goto_main_menu_action(Widget* widget, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: goto_main_menu_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	free_state(*state); // frees previous states
	/* set up a new main_menu state */
	state->type = MAIN_MENU;
	state->focused = 0;
	state->previous_state = NULL;
	game_free(state->game); // frees the game
	return 0;
}

/* Arrow clicked in chooser widget */
int arrow_action(Widget* widget, game_state* state) {
	if (state == NULL || widget == NULL) {
		fprintf(stderr, "Error: arrow_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	if (widget->id == LEVEL_UP_B) { // if the found widget is the up button
		if (state->type == CHOOSE_SKILL) {
			if (state->number >= NUM_OF_LEVELS) { // do not exceed max number of levels allowd
				return 0;
			}
		} else if (state->type == LOAD_GAME || state->type == SAVE_GAME || state->type == EDIT_GAME) {
			if (state->number >= NUM_OF_WORLDS){ // same for worlds
				return 0;
			}
		}
		state->number++; // increase number
	} else if (widget->id == LEVEL_DN_B) { // if the found widget is the down button
		if (state->number <= 1) { // min is 1
			return 0;
		}
		state->number--; // decrease number
	}
	return 0;
}

/* Helper fuction - make a move */
int play_turn(int dir, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: play_turn given NULL pointer");
		return ERROR_NO_STATE;
	}
	if (dir_is_valid(state->game, dir)) { // given a direction (defined in /core), tests if it a valid move
		move_obj(state->game, state->game->player, dir); // makes the move
		state->game->player = SWITCH(state->game->player); // switches the player
		state->game->turns--; // decreases number of turns left
	}
	return 0;
}

/* Helper function - make a move in_game */
int in_game_action(game_state* state, SDLKey key) {
	if (state == NULL) {
		fprintf(stderr, "Error: in_game_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	if (state->catormouse != PLAYING) { // if PAUSED or WIN do nothing
		return 0;
	}
	int dir;
	if (!((state->game->player == CAT && state->game->num_steps_cat == 0) || (state->game->player == MOUSE && state->game->num_steps_mouse == 0))) {
		// if not a human move, do nothing
		return 0;
	}
	switch (key) { // possible key inputs
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
			fprintf(stderr, "Error: invalid key: %d; needs arrow keys\n", (int) key);
			return ERROR_INVALID_KEY;
	}
	return play_turn(dir, state); // TRY to make the move
}

/* Helper function - make a move (of the cursor) game_edit
 * Moves the editing cursor according to key given
 */
int game_editing_action(game_state* state, SDLKey key) {
	if (state == NULL) {
		fprintf(stderr, "Error: game_editing_action given NULL pointer");
		return ERROR_NO_STATE;
	}
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
			fprintf(stderr, "Error: invalid key: %d; needs arrow keys\n", (int) key);
			return ERROR_INVALID_KEY;
	}
	return 0;
}

/*  Handle mouse input in grid window: IN_GAME
 * fake_widget is a widget used to pass ONLY the mouse position data to the action,
 * without altering the action header. fake_widget->pos holds the mouse position, relative to board.
 */
int grid_mouse_action(Widget* fake_widget, game_state* state) {
	if (state == NULL || fake_widget == NULL) {
		fprintf(stderr, "Error: grid_mouse_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	if (state->catormouse != PLAYING) { // if game is PAUSED or WIN do nothing
		return 0;
	}
	SDL_Rect mouse_pos = fake_widget->pos; // mouse position, relative to board
	int x,y;
	int dx[] = {0,1,0,-1};
	int dy[] = {-1,0,1,0};
	x = mouse_pos.x / (GRID_W / 7);
	y = mouse_pos.y / (GRID_H / 7);
	for (int i=0; i<4; ++i) {
		// for every possible direction, test if it a possible move
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
	return 0;
}

/*  Handle mouse input in grid window: GAME_EDIT
 * fake_widget is a widget used to pass ONLY the mouse position data to the action,
 * without altering the action header. 
 */
int grid_edit_mouse_action(Widget* fake_widget, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: grid_edit_mouse_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	SDL_Rect mouse_pos = fake_widget->pos; // mouse position, relative to board
	int x = mouse_pos.x / (GRID_W / 7);
	int y = mouse_pos.y / (GRID_H / 7);
	state->focused = 10*x+y; // cursor location is held as xy in state->focused
	return 0;
}

/* Start a new game creation state */
int create_game_action(Widget* widget, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: create_game_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	game_state* old_state = (game_state*) malloc (sizeof(game_state));
	if (old_state == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return ERROR_MALLOC_FAILED;
	}
	memcpy(old_state, state, sizeof(game_state));
	// there is no allocated game in the main menu state so no need to free old_state->game
	state->previous_state = old_state;
	state->type = GAME_EDIT;
	state->focused = 0;

	Game* game = game_malloc(); // allocated memory for the new game
	if (game == NULL) {
		fprintf(stderr, "Error: game_malloc failed\n");
		return ERROR_MALLOC_FAILED;
	}
	state->game = game;
	// set up the new board as empty
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			game->board->board[i][j] = EMPTY;
		}
	}
	// put cat, mouse and cheese off the board
	game->cat_x = BOARD_SIZE;
	game->cat_y = BOARD_SIZE;
	game->mouse_x = BOARD_SIZE;
	game->mouse_y = BOARD_SIZE;
	game->cheese_x = BOARD_SIZE;
	game->cheese_y = BOARD_SIZE;
	game->turns = DEFAULT_NUMBER_OF_TURNS; // 20
	state->world_id = 0; // used to determine its a new world and not editing an old one
	return 0;
}

/* Start a new game editing SELECTION state */
int edit_game_action(Widget* widget, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: edit_game_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	game_state* old_state = (game_state*) malloc (sizeof(game_state));
	if (old_state == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return ERROR_MALLOC_FAILED;
	}
	memcpy(old_state, state, sizeof(game_state));
	// there is no allocated game in the main menu state so no need to free old_state->game
	state->previous_state = old_state;
	state->type = EDIT_GAME; // this is the "select world to edit" state
	state->focused = 0; // default focus is always first focusable widget
	state->number = DEFAULT_WORLD_INDEX;
	return 0;
}

/** Game editing actions **/
/* Helper function - remove object from game editing grid */
int empty_place(game_state* state, int x, int y) {
	if (state == NULL) {
		fprintf(stderr, "Error: empty_place given NULL pointer");
		return ERROR_NO_STATE;
	}
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

/* Place a wall action */
int place_wall_action(Widget* widget, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: place_wall_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	state->game->board->board[GET_X(state->focused)][GET_Y(state->focused)] = WALL;
	empty_place(state, GET_X(state->focused), GET_Y(state->focused));
	return 0;
}

/* Remove object action */
int place_empty_action(Widget* widget, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: place_empty_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	state->game->board->board[GET_X(state->focused)][GET_Y(state->focused)] = EMPTY;
	empty_place(state, GET_X(state->focused), GET_Y(state->focused)); // remove something else from the place
	return 0;
}

/* Place cat action (and remove previous cat) */
int place_cat_action(Widget* widget, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: place_cat_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	empty_place(state, GET_X(state->focused), GET_Y(state->focused));
	state->game->cat_x = GET_X(state->focused);
	state->game->cat_y = GET_Y(state->focused);
	state->game->board->board[state->game->cat_x][state->game->cat_y] = EMPTY;
	state->game->player = CAT; // set the first player
	return 0;
}

/* Place mouse action (and remove previous mouse) */
int place_mouse_action(Widget* widget, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: place_mouse_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	empty_place(state, GET_X(state->focused), GET_Y(state->focused));
	state->game->mouse_x = GET_X(state->focused);
	state->game->mouse_y = GET_Y(state->focused);
	state->game->board->board[state->game->mouse_x][state->game->mouse_y] = EMPTY;
	state->game->player = MOUSE; // set the first player
	return 0;
}

/* Place cheese action (and remove previous cheese) */
int place_cheese_action(Widget* widget, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: place_cheese_action given NULL pointer");
		return ERROR_NO_STATE;
	}
	empty_place(state, GET_X(state->focused), GET_Y(state->focused));
	state->game->cheese_x = GET_X(state->focused);
	state->game->cheese_y = GET_Y(state->focused);
	state->game->board->board[state->game->cheese_x][state->game->cheese_y] = EMPTY;
	return 0;
}
