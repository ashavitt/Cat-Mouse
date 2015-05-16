#include "handlers.h"

/** returns the number of focusable widgets in each window */
byte get_obj_count(state_type type) {
	switch (type) {
		case MAIN_MENU:
			return MAIN_MENU_OBJS;
		case CHOOSE_PLAYER:
			return CHOOSE_PLAYER_OBJS;
		case CHOOSE_SKILL:
			return CHOOSE_SKILL_OBJS;
		case IN_GAME:
			return GAME_SCREEN_OBJS;
		case LOAD_GAME:
		case SAVE_GAME:
		case EDIT_GAME:
			return LOAD_GAME_OBJS;
		case GAME_EDIT:
			return CREATE_GAME_OBJS;
		case ERROR_DIALOG:
			return ERROR_MESSAGE_OBJS;
	}
	return 0;
}

/** returns the array of focusable widgets in each window */
byte* get_objs_arr(state_type type) {
	switch (type) {
		case MAIN_MENU:
			return main_menu_ids;
		case CHOOSE_PLAYER:
			return choose_player_ids;
		case CHOOSE_SKILL:
			return choose_skill_ids;
		case IN_GAME:
			return game_screen_ids;
		case LOAD_GAME:
		case SAVE_GAME:
		case EDIT_GAME:
			return load_game_ids;
		case GAME_EDIT:
			return create_game_ids;
		case ERROR_DIALOG:
			return error_message_ids;
	}
	return NULL;
}

/** a recursive function to handle a mouse click
  * goes through the tree beneath widget and checks which clickable widgets were clicked
  * calls the appropriate actions */
int handle_mouse_click_rec(SDL_Event* e, Widget* widget, game_state* state, SDL_Rect abs_pos) {
	ListRef children;
	int err, array_size;
	Widget* widget2;
	byte* ids;
	SDL_Rect dims = widget->dims;
	if (e == NULL) {
		return ERROR_NO_SDLEVENT;
	}
	if ((err = add_rect(&abs_pos,&(widget->pos))) != 0) {
		fprintf(stderr, "Error: in add_rect, %d\n",err);
		return err;
	}

	if (widget->type == BUTTON) {
		if ((e->button.x > abs_pos.x) && (e->button.x < abs_pos.x + dims.w) 
			&& (e->button.y > abs_pos.y) && (e->button.y < abs_pos.y + dims.h)) {
			if (state->type != GAME_EDIT) {
				ids = get_objs_arr(state->type);
				array_size = get_obj_count(state->type);
				// set the focus
				for (int i=0; i<array_size; i++) {
					if (ids[i] == widget->id && widget->id != UNFOCUSABLE) {
						state->focused = i;
						break;
					}
				}
			}
			if (widget->id == GRID_B) {
				// create a widget to hold the coordinates of the mouse click
				// we use the coordinates to determine which block inside the grid was pressed
				widget2 = new_panel(0, (SDL_Rect) {e->button.x - abs_pos.x, e->button.y - abs_pos.y,0,0}, NULL);
				if (widget2 == NULL) {
					fprintf(stderr, "Error: new_panel failed\n");
					return 1;
				}
			} else {
				widget2 = widget;
			}
			if ((err = (widget->onclick)(widget2, state)) != 0) {
				if (err != 1) {
					fprintf(stderr, "Error: onclick func of widget failed, code %d\n",err);
				}
				return err;
			}
			if (widget->id == GRID_B) {
				freeWidget(widget2);
			}
		}
	}
	//TODO later we should cut the graphics to fit in the panel ???
	if (widget->type == PANEL) {
		abs_pos.w = (widget->pos).w;
		abs_pos.h = (widget->pos).h;
	}
	
	children = widget->children;
	//run over children
	while (children != NULL) { //we run over the list of children
		if (isEmpty(children)) {
			break;
		}
		if ((err = handle_mouse_click_rec(e, (Widget*) headData(children), state, abs_pos)) != 0) {
			// err == 1 means quit was pressed
			if (err != 1) {
				fprintf(stderr, "Error: handling click for child failed\n");
			}
			return err;
		}
		children = tail(children); //cutting the head
	}
	
	return 0;
}

int handle_mouse_click(SDL_Event* event, Widget* window, game_state* state) {
	SDL_Rect zeros = {0,0,0,0};
	return handle_mouse_click_rec(event, window, state, zeros);
}

int handle_keyboard(SDL_Event* event, Widget* window, game_state* state) {
	if (state == NULL) {
		fprintf(stderr, "Error: state is null\n");
		return ERROR_NO_STATE;
	}
	Widget* widget;
	int err;
	int id = -1;
	switch(event->key.keysym.sym) {
		case SDLK_ESCAPE:
			//TODO when should this work?
			return 1;
		case SDLK_RETURN:
			if (state->type != IN_GAME && state->type != GAME_EDIT) {
				widget = find_widget_by_id(window, get_objs_arr(state->type)[state->focused]);
				if (widget == NULL) {
					// focused widget must exist
					fprintf(stderr, "Error: no widget with id:%d was found\n",state->focused);
					return ERROR_FOCUSED_ID;
				}
				if ((err = (widget->onclick)(widget, state)) != 0) {
					// "NULL pointer exception"
					if (err != 1) { // if it is 1, then its just clean closing
						fprintf(stderr, "Error: onclick func of widget failed, code %d\n",err);
					}
					return err;
				}
			}
			break;
		case SDLK_TAB:
			if (state->type != IN_GAME && state->type != GAME_EDIT) {
				// update the focused widget
				state->focused = (state->focused + 1) % get_obj_count(state->type);
			}
			break;
		case SDLK_UP:
		case SDLK_DOWN:
			if (state->type == CHOOSE_SKILL || state->type == LOAD_GAME || state->type == SAVE_GAME || state->type == EDIT_GAME) {
				if (state->focused == 0) {
					id = (event->key.keysym.sym == SDLK_UP) ? LEVEL_UP_B : LEVEL_DN_B;
				}
				break;
			}
		case SDLK_RIGHT:
		case SDLK_LEFT:
			if (state->type == IN_GAME) {
				return in_game_action(state, event->key.keysym.sym);
			}
			if (state->type == GAME_EDIT) {
				return game_editing_action(state, event->key.keysym.sym);
			}
			break;
		case SDLK_SPACE:
			if (state->type == IN_GAME) {
				return pause_resume_action(NULL, state);
			}
			if (state->type == GAME_EDIT) {
				return place_empty_action(NULL, state);
			}
			break;
		case SDLK_F1:
			if (state->type == IN_GAME && state->catormouse != PLAYING) {
				id = RECONF_MOUSE_B;
			} else if (state->type == GAME_EDIT) {
				id = GOTO_MAIN_MENU_B;
			}
			break;
		case SDLK_F2:
			if (state->type == IN_GAME && state->catormouse != PLAYING) {
				id = RECONF_CAT_B;
			}
			break;
		case SDLK_F3:
			if (state->type == IN_GAME && state->catormouse != PLAYING) {
				id = RESTART_GAME_B;
			}
			break;
		case SDLK_F4:
			if (state->type == IN_GAME && state->catormouse != PLAYING) {
				id = GOTO_MAIN_MENU_B;
			}
			break;
		case SDLK_m:
			if (state->type == GAME_EDIT) {
				return place_mouse_action(NULL, state);
			}
			break;
		case SDLK_p:
			if (state->type == GAME_EDIT) {
				return place_cheese_action(NULL, state);
			}
			break;
		case SDLK_c:
			if (state->type == GAME_EDIT) {
				return place_cat_action(NULL, state);
			}
			break;
		case SDLK_w:
			if (state->type == GAME_EDIT) {
				return place_wall_action(NULL, state);
			}
			break;
		case SDLK_s:
			if (state->type == GAME_EDIT) {
				id = SAVE_WORLD_B;
			}
			break;
		default:
			break;
	}
	// call the action of the pressed widget
	if (id != -1) {
		widget = find_widget_by_id(window, id);
		if (widget == NULL) {
			// focused widget must exist
			fprintf(stderr, "Error: no widget with id:%d was found\n",state->focused);
			return ERROR_FOCUSED_ID;
		}
		if ((err = (widget->onclick)(widget, state)) != 0) {
			// "NULL pointer exception"
			if (err != 1) { // if it is 1, then its just clean closing
				fprintf(stderr, "Error: onclick func of widget failed, code %d\n",err);
			}
			return err;
		}
	}
	return 0;
}
