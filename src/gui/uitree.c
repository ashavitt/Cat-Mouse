#include "uitree.h"

// can only print positive numbers
// character spacing?
#define SPACE_BETWEEN_WORLD_AND_NUMBER (50)
Widget* number_to_graphic(int id, SDL_Rect main_pos, int number, int size, Widget* parent) {
	SDL_Rect zeros = {0,0,0,0};
	Widget* text = new_panel(id, main_pos, NULL);
	SDL_Rect zero_dims;
	SDL_Rect dims;
	SDL_Rect pos = zeros; // TODO
	SDL_Rect offset_rect;
	Widget* temp_widget;
	int digit,width,height;
	int n;

	if (text == NULL) {
		fprintf(stderr, "Error: text is null\n");
		return NULL;
	}
	text->parent = parent;

	switch (size) { // "Load" data from defines
		case TEXT_SIZE_SMALL:
			zero_dims = (SDL_Rect){S_NUM_T_X_START + 2*S_NUM_T_W, S_NUM_T_Y_START, S_NUM_T_W, S_NUM_T_H};
			// both braces () for small font are at the begginning
			break;
		case TEXT_SIZE_MEDIUM:
			zero_dims = (SDL_Rect){M_NUM_T_X_START, M_NUM_T_Y_START, M_NUM_T_W, M_NUM_T_H};
			break;
		case TEXT_SIZE_LARGE:
			zero_dims = (SDL_Rect){L_NUM_T_X_START, L_NUM_T_Y_START, L_NUM_T_W, L_NUM_T_H};
			break;
		default:
			fprintf(stderr, "Error: bad size\n");
			freeWidget(text);
			return NULL;
	}

	n = floor(log10(number)) + 1;
	if (size == TEXT_SIZE_SMALL) {
		n += 2;
	}

	offset_rect.x = zero_dims.w;
	offset_rect.y = zero_dims.h;
	pos.x += n*offset_rect.x;

	if (size == TEXT_SIZE_SMALL) {
		dims = zero_dims;
		dims.x -= S_NUM_T_W; // ')'
		if ((temp_widget = new_graphic(UNFOCUSABLE, dims, pos, texts, text)) == NULL) {
			fprintf(stderr, "Error: new_graphic failed\n");
			freeWidget(text);
			return NULL;
		}
		pos.x -= offset_rect.x;
	}

	while (number != 0) {
		digit = number % 10;
		dims = zero_dims; // TODO memcpy?
		dims.x += offset_rect.x * digit;
		if ((temp_widget = new_graphic(UNFOCUSABLE, dims, pos, texts, text)) == NULL) {
			fprintf(stderr, "Error: new_graphic failed\n");
			freeWidget(text);
			return NULL;
		}
		//add_rect(&pos, &offset_rect);
		pos.x -= offset_rect.x; // move position left for next number (negative coordinates)
		number /= 10;
	}

	if (size == TEXT_SIZE_SMALL) {
		dims = zero_dims;
		dims.x -= 2*S_NUM_T_W; // '('
		if ((temp_widget = new_graphic(UNFOCUSABLE, dims, pos, texts, text)) == NULL) {
			fprintf(stderr, "Error: new_graphic failed\n");
			freeWidget(text);
			return NULL;
		}
		pos.x -= offset_rect.x;
	}

	width = n*offset_rect.x;
	height = offset_rect.y;

	add_rect(&(text->pos), &offset_rect); // fix for negative coordinates offset
	text->dims.w = (text->pos.w = width);
	text->dims.h = (text->pos.h = height);

	return text;
}

Widget* build_text_button(int id, SDL_Rect main_pos, SDL_Rect bg_dims, SDL_Rect text_dims, Widget* parent, onclick* onClick) {
	Widget* clickable;
	Widget* bg;
	Widget* text;
	SDL_Rect zeros = {0,0,0,0};

	//set the position of the text in the center of the button graphic
	SDL_Rect text_pos = get_center(bg_dims, text_dims);
	//fprintf(stderr, "%d,%d\n", text_pos.x, text_pos.y);

	//create the widgets
	clickable = new_button(id, main_pos, main_pos, NULL, onClick);
	if (clickable == NULL) {
		fprintf(stderr, "Error: new_button failed\n");
		return NULL;
	}
	clickable->parent = parent;
	bg = new_graphic(UNFOCUSABLE, bg_dims, zeros, buttons, clickable);
	if (bg == NULL) {
		fprintf(stderr, "Error: new_graphic failed\n");
		freeWidget(clickable);
		return NULL;
	}
	text = new_graphic(UNFOCUSABLE, text_dims, text_pos, texts, bg);
	if (text == NULL) {
		fprintf(stderr, "Error: new_graphic failed\n");
		freeWidget(clickable);
		return NULL;
	}
	return clickable;
}

Widget* build_chooser(int id, SDL_Rect main_pos, SDL_Rect bg_dims, Widget* parent, game_state* state) {
	Widget *chooser, *bg, *text, *number, *up, *up_arrow, *down, *down_arrow;
	SDL_Rect zeros = {0,0,0,0};
	SDL_Rect arrow_dims = {UP_ARROW_B_X, UP_ARROW_B_Y, UP_ARROW_B_W, UP_ARROW_B_H};
	SDL_Rect world_t_dims = {TITLES_T_X_START, TITLES_T_Y_START - WL_T_H, WL_T_W, WL_T_H};
	SDL_Rect arrow_pos = zeros;
	SDL_Rect text_dims = bg_dims; // TODO??
	SDL_Rect pos = zeros;
	// text_dims is the area that includes the number or "World "+number
	text_dims.w -= arrow_dims.w;
	text_dims.x = text_dims.y = 0; // text_dims and pos

	if (state == NULL) {
		fprintf(stderr, "Error: state is NULL\n");
		return NULL;
	}

	chooser = new_button(id, main_pos, main_pos, NULL, do_nothing_action); // TODO do_nothing
	if (chooser == NULL) {
		fprintf(stderr, "Error: new_button failed\n");
		freeWidget(chooser);
		return NULL;
	}
	chooser->parent = parent;

	bg = new_graphic(UNFOCUSABLE, bg_dims, zeros, buttons, chooser);
	if (bg == NULL) {
		fprintf(stderr, "Error: new_button failed\n");
		freeWidget(chooser);
		return NULL;
	}

	//text = new_panel(UNFOCUSABLE, text_dims, bg); // text_pos = zeros
	if (state->type == CHOOSE_SKILL) {
		// numbers
		text = number_to_graphic(UNFOCUSABLE, zeros, state->number, TEXT_SIZE_MEDIUM, bg);
		if (text == NULL) {
			fprintf(stderr, "Error: text widget is NULL\n");
			freeWidget(chooser);
			return NULL;
		}
		text->pos = get_center(text_dims, text->dims);
		if (append(bg->children, text) == NULL) {
			fprintf(stderr, "Error: append failed\n");
			freeWidget(chooser);
			freeWidget(text);
			return NULL;
		}
	} else if (state->type == EDIT_GAME || state->type == LOAD_GAME || state->type == SAVE_GAME) {
		// "World " + number
		text = new_panel(UNFOCUSABLE, text_dims, bg);
		if (text == NULL) {
			fprintf(stderr, "Error: panel widget is NULL\n");
			freeWidget(chooser);
			return NULL;
		}
		// adds the "World "
		if (new_graphic(UNFOCUSABLE, world_t_dims, zeros, texts, text) == NULL) {
			fprintf(stderr, "Error: new_graphic failed\n");
			freeWidget(chooser);
			return NULL;
		}
		// pos is zeros
		pos.x += SPACE_BETWEEN_WORLD_AND_NUMBER + world_t_dims.w; // TODO get rid of
		if ((number = number_to_graphic(UNFOCUSABLE, pos, state->number, TEXT_SIZE_MEDIUM, text)) == NULL){
			fprintf(stderr, "Error: number_to_graphic failed\n");
			freeWidget(chooser);
			return NULL;
		}
		if (append(text->children, number) == NULL) {
			fprintf(stderr, "Error: append failed\n");
			freeWidget(chooser);
			freeWidget(number);
			return NULL;
		}
		pos = get_center(world_t_dims, number->pos);
		pos.x += SPACE_BETWEEN_WORLD_AND_NUMBER; // fix number location
		number->pos = pos;

		text->dims.w = world_t_dims.w + number->dims.w + SPACE_BETWEEN_WORLD_AND_NUMBER;
		text->dims.h = world_t_dims.h;
		text->pos = get_center(text_dims, text->dims);
	}

	arrow_pos.x = text_dims.w;
	up = new_button(LEVEL_UP_B, arrow_dims, arrow_pos, bg, arrow_action);
	if (up == NULL) {
		fprintf(stderr, "Error: new_button failed\n");
		freeWidget(chooser);
		return NULL;
	}
	up_arrow = new_graphic(UNFOCUSABLE, arrow_dims, zeros, buttons, up);
	if (up_arrow == NULL) {
		fprintf(stderr, "Error: new_graphic failed\n");
		freeWidget(chooser);
		return NULL;
	}
	// Move to down arrow
	arrow_pos.y += arrow_dims.h;
	arrow_dims.y += arrow_dims.h;
	down = new_button(LEVEL_DN_B, arrow_dims, arrow_pos, bg, arrow_action);
	if (down == NULL) {
		fprintf(stderr, "Error: new_button failed\n");
		freeWidget(chooser);
		return NULL;
	}
	// TODO error
	down_arrow = new_graphic(UNFOCUSABLE, arrow_dims, zeros, buttons, down);
	if (down_arrow == NULL) {
		fprintf(stderr, "Error: new_graphic failed\n");
		freeWidget(chooser);
		return NULL;
	}
	return chooser;
}

/** creates a menu template to be filled later with append_menu */
Widget* create_menu(SDL_Rect button_dims, Widget* parent) {
	SDL_Rect pos = {0,0,0,0};
	pos.w = button_dims.w;
	Widget* menu = new_panel(UNFOCUSABLE, pos, NULL);
	if (menu == NULL) {
		fprintf(stderr, "Error: new_panel failed\n");
		return NULL;
	}
	menu->parent = parent;
	//set the panel dims as the button dims for later usage
	menu->dims = button_dims;
	return menu;
}

/** appends a button to the menu */
int append_menu(Widget* menu, int id, SDL_Rect text_dims, onclick* onClick, game_state* state) {
	SDL_Rect main_pos = {0,0,0,0};
	Widget* button;
	if (menu == NULL) {
		return ERROR_NO_WIDGET;
	}
	//check if this is the first button of the menu
	if (menu->pos.h == 0) {
		//add button height to menu
		menu->pos.h += menu->dims.h;
	} else {
		//set the position of the button with 0.5 spacing
		main_pos.y = menu->pos.h + (0.5 * (menu->dims.h));
		//set the new height of the menu
		menu->pos.h += 1.5 * (menu->dims.h);
	}
	SDL_Rect bg_dims = menu->dims;
	main_pos.w = bg_dims.w;
	main_pos.h = bg_dims.h;
	if (id != LEVEL_CHOOSER && id != WORLD_CHOOSER) {
		button = build_text_button(id, main_pos, bg_dims, text_dims, menu, onClick);
	} else {
		button = build_chooser(id, main_pos, bg_dims, menu, state);
	}
	//add the button to the menu children list
	if (button == NULL) {
		fprintf(stderr, "Error: button creation failed\n");
		return ERROR_MALLOC_FAILED;
	}
	if (append(menu->children, button) == NULL) {
		fprintf(stderr, "Error: append failed.\n");
		freeWidget(button);
		return ERROR_APPEND_FAILED;
	}
	return 0;
}

/** updates the color of the selected widget*/
int set_focus_bg(Widget* window, SDL_Rect new_button_dims, int focused) {
	Widget* button;
	if ((button = find_widget_by_id(window, focused)) == NULL) {
		return ERROR_NO_FOCUS;
	}
	((Widget*)headData(button->children))->dims = new_button_dims;
	return 0;
}

/** builds the grid and all the widgets inside it */
Widget* build_grid(int id, Widget* parent, game_state* state) {
	Widget *grid, *grid_button, *obj;
	SDL_Rect dims = {0,0,GRID_W,GRID_H};
	SDL_Rect pos = get_center(parent->dims, dims);

	// build the clickable widget
	if (state->type == GAME_EDIT) {
		grid_button = new_button(GRID_B, dims, pos, NULL, grid_edit_mouse_action);
	} else {
		grid_button = new_button(GRID_B, dims, pos, NULL, grid_mouse_action);
	}
	if (grid_button == NULL) {
		fprintf(stderr, "Error: new_button failed\n");
		return NULL;
	}
	grid_button->parent = parent;
	//build the graphic of the grid
	grid = new_graphic(UNFOCUSABLE, dims, dims, grid_surface, grid_button);
	if (grid == NULL) {
		fprintf(stderr, "Error: new_graphic failed\n");
		freeWidget(grid_button);
		return NULL;
	}
	SDL_Rect tile_dims = {0,0,GRID_W/7,GRID_H/7};
	SDL_Rect tile_pos = {0,0,GRID_W/7,GRID_H/7};

	// add mouse, cat, walls
	Game* game = state->game;
	//cat
	tile_pos.x = game->cat_x * tile_dims.w;
	tile_pos.y = game->cat_y * tile_dims.h;
	obj = new_graphic(UNFOCUSABLE, tile_dims, tile_pos, cat, grid);
	if (obj == NULL) {
		fprintf(stderr, "Error: new_graphic failed\n");
		freeWidget(grid_button);
		return NULL;
	}
	//mouse
	tile_pos.x = game->mouse_x * tile_dims.w;
	tile_pos.y = game->mouse_y * tile_dims.h;
	obj = new_graphic(UNFOCUSABLE, tile_dims, tile_pos, mouse, grid);
	if (obj == NULL) {
		fprintf(stderr, "Error: new_graphic failed\n");
		freeWidget(grid_button);
		return NULL;
	}
	//cheese
	tile_pos.x = game->cheese_x * tile_dims.w;
	tile_pos.y = game->cheese_y * tile_dims.h;
	obj = new_graphic(UNFOCUSABLE, tile_dims, tile_pos, cheese, grid);
	if (obj == NULL) {
		fprintf(stderr, "Error: new_graphic failed\n");
		freeWidget(grid_button);
		return NULL;
	}
	//walls
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (game->board->board[j][i] == WALL) {
				tile_pos.x = j * tile_dims.w;
				tile_pos.y = i * tile_dims.h;
				obj = new_graphic(UNFOCUSABLE, tile_dims, tile_pos, bricks, grid);
				if (obj == NULL) {
					fprintf(stderr, "Error: new_graphic failed\n");
					freeWidget(grid_button);
					return NULL;
				}
			}
		}
	}
	if (state->type == GAME_EDIT) {
		//cursor
		tile_pos.x = GET_X(state->focused) * tile_dims.w - 2; // TODO magic number
		tile_pos.y = GET_Y(state->focused) * tile_dims.h - 2;
		tile_dims.w += 6; // TODO magic number
		tile_dims.h += 6;
		obj = new_graphic(UNFOCUSABLE, tile_dims, tile_pos, tiles, grid);
		if (obj == NULL) {
			fprintf(stderr, "Error: new_graphic failed\n");
			freeWidget(grid_button);
			return NULL;
		}
	}
	return grid_button;
}

/** build the buttons and texts in the IN_GAME state */
int build_panels_in_game(Widget* title_panel, Widget* top_buttons, Widget* left_panel, game_state* state) {
 	Widget *widget, *menu;
 	SDL_Rect rect = {TITLES_T_X_START,TITLES_T_Y_START,WL_T_W,WL_T_H};
 	SDL_Rect pos = get_center(title_panel->dims, rect);
 	int offset;
 	SDL_Rect dims = rect, button_dims = {0,WL_BUTTON_H * 4, WS_BUTTON_W, WS_BUTTON_H}, text_dims;
 	/* Top Panel */ /* Top Buttons */
 	if (state->catormouse == PLAYING || state->catormouse == PAUSED) {
 		dims = (SDL_Rect) {G_TITLES_X_START, G_TITLES_Y_START, G_TITLES_W, G_TITLES_H}; // "Mouse's move"
		pos = get_center(title_panel->dims, dims);
		offset = pos.y = 50; // TODO top margin
		if (state->game->player == CAT) {
			dims.y += dims.h; // now "Cat's move"
		} 
		if (new_graphic(UNFOCUSABLE, dims, pos, texts, title_panel) == NULL) {
			fprintf(stderr, "Error: appending title widget failed\n");
			return ERROR_APPEND_FAILED;
		}

		pos.x += dims.w; // pos for number to the right of title
		widget = number_to_graphic(UNFOCUSABLE, pos, state->game->turns, TEXT_SIZE_SMALL, title_panel); //number of turns left, with small size and ()
		if (widget == NULL) {
			fprintf(stderr, "Error: number_to_graphic failed\n");
			return ERROR_NO_WIDGET;
		}
		if (append(title_panel->children, widget) == NULL) {
			fprintf(stderr, "Error: appending title widget failed\n");
			freeWidget(widget);
			return ERROR_APPEND_FAILED;
		}
		offset += dims.h;
 	}
	switch(state->catormouse) {
		case PLAYING:
			// add stuff before button
			// ALREADY ADDED STUFF BEFORE
			pos = get_center(title_panel->dims, dims);
			pos.y = offset;
			if ((state->game->player == CAT && state->game->num_steps_cat == 0) 
					|| (state->game->player == MOUSE && state->game->num_steps_mouse == 0)) {
				dims.y = G_TITLES_Y_START + 2*dims.h; // Human, waiting for next move
			} else { // machine
				dims.y = G_TITLES_Y_START + 4*dims.h; // Machine -computing
			}

			if (new_graphic(UNFOCUSABLE, dims, pos, texts, title_panel) == NULL) { // machine / human..
				fprintf(stderr, "Error: appending status2 title widget failed\n");
				return ERROR_APPEND_FAILED;
			}

			dims = (SDL_Rect) {NS_B_MESSAGE_X_START, NS_B_MESSAGE_Y_START, NS_T_W, NS_T_H};
			// in game stuff; disable widgets
			pos = get_center(top_buttons->pos, button_dims);
			if ((state->game->player == CAT && state->game->num_steps_cat == 0) ||
				(state->game->player == MOUSE && state->game->num_steps_mouse == 0)) {
				dims.y += NS_T_H;
			} else { // machine is now playing, dims is ok
			}
			widget = build_text_button(PAUSE_B, pos, button_dims, dims, top_buttons, pause_resume_action);
			if (widget == NULL) {
				fprintf(stderr, "Error: build_text_button failed\n");
				return ERROR_NO_WIDGET;
			}
			if (append(top_buttons->children, widget) == NULL) {
				fprintf(stderr, "Error: appending title button widget failed\n");
				freeWidget(widget);
				return ERROR_APPEND_FAILED;
			}
			break;
		case PAUSED:
			// disable grid
			// add stuff before buttons

			pos = get_center(title_panel->dims, dims);
			pos.y = offset;
			if ((state->game->player == CAT && state->game->num_steps_cat == 0) 
					|| (state->game->player == MOUSE && state->game->num_steps_mouse == 0)) {
				dims.y = G_TITLES_Y_START + 3*dims.h; // Human, waiting for next move
			} else { // machine
				dims.y = G_TITLES_Y_START + 5*dims.h; // Machine -computing
			}

			if (new_graphic(UNFOCUSABLE, dims, pos, texts, title_panel) == NULL) { // machine / human..
				fprintf(stderr, "Error: appending status2 title widget failed\n");
				return ERROR_APPEND_FAILED;
			}

			dims = (SDL_Rect) {NS_B_MESSAGE_X_START, NS_B_MESSAGE_Y_START + (2 * NS_T_H), NS_T_W, NS_T_H};
			pos = get_center(top_buttons->pos, button_dims);
			widget = build_text_button(PAUSE_B, pos, button_dims, dims, top_buttons, pause_resume_action);
			if (append(top_buttons->children, widget) == NULL) {
				fprintf(stderr, "Error: appending title widget failed\n");
				freeWidget(widget);
				return ERROR_APPEND_FAILED;
			}
			break;
		case CAT_WIN:
			dims.x += WL_T_W * 2;
			dims.y += WL_T_H * 2;
			if (new_graphic(UNFOCUSABLE, dims, pos, texts, title_panel) == NULL) {
				fprintf(stderr, "Error: appending title widget failed\n");
				return ERROR_APPEND_FAILED;
			}
			break;
		case MOUSE_WIN:
			dims.x += WL_T_W * 2;
			dims.y += WL_T_H * 3;
			if (new_graphic(UNFOCUSABLE, dims, pos, texts, title_panel) == NULL) {
				fprintf(stderr, "Error: appending title widget failed\n");
				return ERROR_APPEND_FAILED;
			}
			break;
		case TIE:
			dims.x += WL_T_W * 2;
			dims.y += WL_T_H * 4;
			if (new_graphic(UNFOCUSABLE, dims, pos, texts, title_panel) == NULL) {
				fprintf(stderr, "Error: appending title widget failed\n");
				return ERROR_APPEND_FAILED;
			}
			break;
		default:
			fprintf(stderr, "Error: bad value:%d, in catormouse field\n",state->catormouse);
			return ERROR_BAD_VALUE;
	}

	/* Left Panel */ // NOTE: this should be different according to game state
	button_dims = (SDL_Rect) {0,0,NL_BUTTON_W,NL_BUTTON_H}, text_dims = (SDL_Rect) {0,0,NL_T_W,NL_T_H};
	menu = create_menu(button_dims, left_panel);
	if (menu == NULL) {
		fprintf(stderr, "Error: create_menu failed\n");
		return ERROR_NO_WIDGET;
	}
	if (append(left_panel->children, menu) == NULL) {
		fprintf(stderr, "Error: appending menu failed\n");
		freeWidget(menu);
		return ERROR_APPEND_FAILED;
	}
	//onclickp choose_action2 = choose_action; // local<-global // useless
	onclickp quit_action2 = quit_action;	// local<-global
	onclickp reconf_action2 = reconf_action;
	if (state->catormouse == PLAYING) {
		//choose_action2 = do_nothing_action;
		quit_action2 = do_nothing_action;
		reconf_action2 = do_nothing_action;
		// all other actions
	}

	if (append_menu(menu, RECONF_MOUSE_B, text_dims, reconf_action2, state) != 0) {
		fprintf(stderr, "Error: appending button failed\n");
		freeWidget(menu);
		return ERROR_APPEND_FAILED;
	}

	text_dims.x += NL_T_W;
	if (append_menu(menu, RECONF_CAT_B, text_dims, reconf_action2, state) != 0) {
		fprintf(stderr, "Error: appending button failed\n");
		return ERROR_APPEND_FAILED;
	}

	text_dims.x += NL_T_W;
	if (append_menu(menu, RESTART_GAME_B, text_dims, restart_game_action, state) != 0) {
		fprintf(stderr, "Error: appending button failed\n");
		return ERROR_APPEND_FAILED;
	}

	text_dims.x += NL_T_W;
	if (append_menu(menu, GOTO_MAIN_MENU_B, text_dims, goto_main_menu_action, state) != 0) {
		fprintf(stderr, "Error: appending button failed\n");
		return ERROR_APPEND_FAILED;
	}

	text_dims.x += NL_T_W;
	if (append_menu(menu, QUIT_B, text_dims, quit_action2, state) != 0) { // TODO segfault on exit
		fprintf(stderr, "Error: appending button failed\n");
		return ERROR_APPEND_FAILED;
	}

	menu->pos = get_center(left_panel->dims,menu->pos);

	if (state->catormouse == PLAYING) {
		rect = left_panel->pos;
		rect.x = rect.y = 0;
		if (SDL_SetAlpha(black_surface, SDL_SRCALPHA, 128) != 0) {
			fprintf(stderr, "Error: setting alpha value failed\n");
			return 2;
		}
		if (new_graphic(UNFOCUSABLE, rect, rect, black_surface, left_panel) == NULL) {
			fprintf(stderr, "Error: appending black_surface widget failed\n");
			return ERROR_APPEND_FAILED;
		}
	}
	return 0;
 }
 
int build_panels_game_edit(Widget* title_panel, Widget* top_buttons, Widget* left_panel, game_state* state) {
 	Widget *widget, *menu;
 	SDL_Rect rect = {TITLES_T_X_START,TITLES_T_Y_START,WL_T_W,WL_T_H};
 	SDL_Rect pos = get_center(title_panel->pos, rect);
 	SDL_Rect dims = rect, button_dims = {0,WL_BUTTON_H * 4, WS_BUTTON_W, WS_BUTTON_H}, text_dims;
 	/* Title Panel */
 	if (state->world_id != 0) {
		// Dims for 'World' sprite
		dims.x += 2*dims.w;
		dims.y += dims.h;
		pos = get_center(title_panel->pos, dims);
		if (new_graphic(UNFOCUSABLE, dims, pos, texts, title_panel) == NULL) { // 'World'
			fprintf(stderr, "Error: appending world heading failed\n");
			return ERROR_APPEND_FAILED;
		}
		//offset = dims.w;
		//pos.x = dims.w;
		widget = number_to_graphic(UNFOCUSABLE, pos, state->world_id, TEXT_SIZE_LARGE, title_panel); // world number
		if (widget == NULL) {
			fprintf(stderr, "Error: number_to_graphic failed\n");
			return ERROR_NO_WIDGET;
		}
		pos = get_center(title_panel->pos, widget->pos); //widget->pos updated with w,h
		pos.x += dims.w / 8; // TODO FIXME
		pos.y -= 1;
		widget->pos = pos;
		if (append(title_panel->children, widget) == NULL) {
			fprintf(stderr, "Error: appending world number failed\n");
			freeWidget(widget);
			return ERROR_APPEND_FAILED;
		}
		/*offset += widget->pos.w; //FIXME
		dims = (SDL_Rect) {0,0,offset,title_panel->pos.h};
		title_text->pos = get_center(title_panel->pos, dims);*/ //FIXME
	} else { //world_id == 0		
		dims.y += dims.h;
		pos = get_center(title_panel->pos, dims);
		if (new_graphic(UNFOCUSABLE, dims, pos, texts, title_panel) == NULL) { // 'New World'
			fprintf(stderr, "Error: appending world heading failed\n");
			return ERROR_APPEND_FAILED;
		}
	}
 	/* Top Buttons */
	button_dims = (SDL_Rect) {0,0,NL_BUTTON_W,NL_BUTTON_H}, text_dims = (SDL_Rect) {0,0,NL_T_W,NL_T_H};
	text_dims.x = 5*text_dims.w;
	pos = get_center(top_buttons->pos, button_dims);
	widget = build_text_button(GOTO_MAIN_MENU_B, pos, button_dims, text_dims, top_buttons, goto_main_menu_action);
	if (widget == NULL) {
		fprintf(stderr, "Error: build_text_button failed\n");
		return ERROR_NO_WIDGET;
	}
	if (append(top_buttons->children, widget) == NULL) { 
		fprintf(stderr, "Error: appending GOTO_MAIN_MENU_B failed\n");
		freeWidget(widget);
		return ERROR_APPEND_FAILED;
	}

	pos.x -= button_dims.w * 1.7; // FIXME magic number
	text_dims.y += text_dims.h;
	widget = build_text_button(SAVE_WORLD_B, pos, button_dims, text_dims, top_buttons, save_game_action);
	if (widget == NULL) {
		fprintf(stderr, "Error: build_text_button failed\n");
		return ERROR_NO_WIDGET;
	}
	if (append(top_buttons->children, widget) == NULL) { 
		fprintf(stderr, "Error: appending SAVE_WORLD_B failed\n");
		freeWidget(widget);
		return ERROR_APPEND_FAILED;
	}

	pos.x += button_dims.w * 1.7 * 2;
	text_dims.y -= text_dims.h;
	text_dims.x -= text_dims.w;
	widget = build_text_button(QUIT_B, pos, button_dims, text_dims, top_buttons, quit_action);
	if (widget == NULL) {
		fprintf(stderr, "Error: build_text_button failed\n");
		return ERROR_NO_WIDGET;
	}
	if (append(top_buttons->children, widget) == NULL) { 
		fprintf(stderr, "Error: appending QUIT_B failed\n");
		freeWidget(widget);
		return ERROR_APPEND_FAILED;
	}

	/* Left Panel */
	button_dims = (SDL_Rect) {0,0,NL_BUTTON_W,NL_BUTTON_H}, text_dims = (SDL_Rect) {0,NL_T_H,NL_T_W,NL_T_H};
	menu = create_menu(button_dims, left_panel);
	if (menu == NULL) {
		fprintf(stderr, "Error: create_menu failed\n");
		return ERROR_NO_WIDGET;
	}
	if (append(left_panel->children, menu) == NULL) {
		fprintf(stderr, "Error: appending menu failed\n");
		freeWidget(menu);
		return ERROR_APPEND_FAILED;
	}
	
	if (append_menu(menu, PLACE_MOUSE_B, text_dims, place_mouse_action, state) != 0) {
		fprintf(stderr, "Error: appending button failed\n");
		return ERROR_APPEND_FAILED;
	}
	
	text_dims.x += NL_T_W;
	if (append_menu(menu, PLACE_CAT_B, text_dims, place_cat_action, state) != 0) {
		fprintf(stderr, "Error: appending button failed\n");
		return ERROR_APPEND_FAILED;
	}
	
	text_dims.x += NL_T_W;
	if (append_menu(menu, PLACE_CHEESE_B, text_dims, place_cheese_action, state) != 0) {
		fprintf(stderr, "Error: appending button failed\n");
		return ERROR_APPEND_FAILED;
	}

	text_dims.x += NL_T_W;
	if (append_menu(menu, PLACE_WALL_B, text_dims, place_wall_action, state) != 0) {
		fprintf(stderr, "Error: appending button failed\n");
		return ERROR_APPEND_FAILED;
	}

	text_dims.x += NL_T_W;
	if (append_menu(menu, PLACE_SPACE_B, text_dims, place_empty_action, state) != 0) {
		fprintf(stderr, "Error: appending button failed\n");
		return ERROR_APPEND_FAILED;
	}

	menu->pos = get_center(left_panel->dims,menu->pos);
	return 0;
}
 
int build_game_scheme(Widget* window, game_state* state) {  
	Widget *title_panel, *top_buttons, *left_panel, *grid_panel, *grid_widget;
	//int y_offset;
	int err;
	SDL_Rect dims;
	SDL_Rect pos = window->dims;

	pos.x = pos.y = 0;
	pos.h = S_NUM_T_H * 4;
	title_panel = new_panel(UNFOCUSABLE, pos, window);
	if (title_panel == NULL) {
		fprintf(stderr, "Error: new_panel failed\n");
		return ERROR_NO_WIDGET;
	}

	pos.y += pos.h;
	pos.h = NL_BUTTON_H*1.5;
	top_buttons = new_panel(UNFOCUSABLE, pos, window);
	if (top_buttons == NULL) {
		fprintf(stderr, "Error: new_panel failed\n");
		return ERROR_NO_WIDGET;
	}

	pos.y += pos.h;
	pos.h = window->dims.h - pos.y;
	pos.w = NL_BUTTON_W*1.15;
	left_panel = new_panel(UNFOCUSABLE, pos, window);
	if (left_panel == NULL) {
		fprintf(stderr, "Error: new_panel failed\n");
		return ERROR_NO_WIDGET;
	}
	
	dims = pos;
	dims.x = dims.y = 0;
	if (new_graphic(UNFOCUSABLE, dims, dims, red_surface, left_panel) == NULL) {
		fprintf(stderr, "Error: Appending bg to left_panel failed\n");
		return ERROR_APPEND_FAILED;
	}

	pos.x += pos.w;
	pos.w = window->dims.w - pos.x;
	grid_panel = new_panel(UNFOCUSABLE, pos, window);
	if (grid_panel == NULL) {
		fprintf(stderr, "Error: new_panel failed\n");
		return ERROR_NO_WIDGET;
	}
	dims = pos;
	dims.x = dims.y = 0;
	if (new_graphic(UNFOCUSABLE, dims, dims, red_surface, grid_panel) == NULL) {
		fprintf(stderr, "Error: Appending bg to left_panel failed\n");
		return ERROR_APPEND_FAILED;
	}

	/* Top panel, buttons and left panel */
	if (state->type == IN_GAME) {
		if ((err = build_panels_in_game(title_panel, top_buttons, left_panel, state)) != 0) {
			fprintf(stderr, "Error: building panels for in_game failed\n");
			return err;
		}
	} else if (state->type == GAME_EDIT) {
		if ((err = build_panels_game_edit(title_panel, top_buttons, left_panel, state)) != 0) {
			fprintf(stderr, "Error: building panels for game_edit failed\n");
			return err;
		}
	} else {
		fprintf(stderr, "Error: bad state->type\n");
		return -1; //TODO
	}

	/* Grid Panel */
	grid_widget = build_grid(UNFOCUSABLE, grid_panel, state);
	if (append(grid_panel->children, grid_widget) == NULL) {
		fprintf(stderr, "Error: appending build_grid failed\n");
		freeWidget(grid_widget);
		return ERROR_APPEND_FAILED;
	}
	return 0;
}

int build_main_menu(Widget* window, game_state* state) {
	Widget *panel;
	SDL_Rect button_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect text_dims, logo_dims, title_pos;

	//create the title Cat & Mouse logo
	logo_dims = (SDL_Rect) {0,0,LOGO_W,LOGO_H};
	title_pos = get_center(window->dims, logo_dims);	
	title_pos.y = 0.3*WL_BUTTON_H; // TODO magic number
	if (new_graphic(UNFOCUSABLE, logo_dims, title_pos, logo_surface, window) == NULL) {
		fprintf(stderr, "Error: new_graphic failed\n");
		return ERROR_APPEND_FAILED;
	}

	text_dims.w = WL_T_W;
	text_dims.h = WL_T_H;
	text_dims.x = TITLES_T_X_START;
	text_dims.y = TITLES_T_Y_START;
	
	//creating a new panel for the buttons
	button_dims.x = 0;
	button_dims.y = WL_BUTTON_H;
	panel = create_menu(button_dims, window);
	if (panel == NULL) {
		fprintf(stderr, "Error: create_menu failed\n");
		return ERROR_NO_WIDGET;
	}
	if (append(window->children, panel) == NULL) {
		fprintf(stderr, "Error: append failed\n");
		freeWidget(panel);
		return ERROR_APPEND_FAILED;
	}

	/*-----creating main menu buttons-----*/
	//new game
	text_dims.x = MAIN_MENU_T_X_START;
	text_dims.y = MAIN_MENU_T_Y_START;
	if (append_menu(panel, NEW_GAME_B, text_dims, new_game_action, state) != 0) {
		return ERROR_APPEND_FAILED;
	}

	//load game
	text_dims.x = MAIN_MENU_T_X_START + WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START;
	if (append_menu(panel, LOAD_GAME_B, text_dims, load_game_action, state) != 0) {
		return ERROR_APPEND_FAILED;
	}

	//create game
	text_dims.x = MAIN_MENU_T_X_START + 2*WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START;
	if (append_menu(panel, CREATE_GAME_B, text_dims, create_game_action, state) != 0) {
		return ERROR_APPEND_FAILED;
	}

	//edit game
	text_dims.x = MAIN_MENU_T_X_START;
	text_dims.y = MAIN_MENU_T_Y_START + WL_T_H;
	if (append_menu(panel, EDIT_GAME_B, text_dims, edit_game_action, state) != 0) {
		return ERROR_APPEND_FAILED;
	}

	//quit
	text_dims.x = MAIN_MENU_T_X_START + WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START + WL_T_H;
	if (append_menu(panel, QUIT_B, text_dims, quit_action, state) != 0) {
		return ERROR_APPEND_FAILED;
	}

	panel->pos = get_center(window->dims, panel->pos);

	button_dims.y += WL_BUTTON_H;

	if (set_focus_bg(window, button_dims, main_menu_ids[state->focused]) != 0) {
		return ERROR_NO_FOCUS;
	}
	return 0;
}

int build_choose_player(Widget* window, game_state* state) {
	Widget *panel;
	SDL_Rect button_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect text_dims;
	SDL_Rect title_pos;

	text_dims.w = WL_T_W;
	text_dims.h = WL_T_H;

	//create the title
	if (state->catormouse == CAT) { // "Choose your cat"
		text_dims.x = TITLES_T_X_START + WL_T_W;
		text_dims.y = TITLES_T_Y_START;
	} else { // "Choose your mouse"
		text_dims.x = TITLES_T_X_START;
		text_dims.y = TITLES_T_Y_START + 3*WL_T_H;
	}
	title_pos = get_center(window->dims, text_dims);
	title_pos.y = WL_BUTTON_H;
	if (new_graphic(UNFOCUSABLE, text_dims, title_pos, texts, window) == NULL) {
		fprintf(stderr, "Error: new_graphic failed\n");
		return ERROR_APPEND_FAILED;
	}

	//creating a new panel for the buttons
	button_dims.x = 0;
	button_dims.y = WL_BUTTON_H;
	panel = create_menu(button_dims, window);
	if (panel == NULL) {
		fprintf(stderr, "Error: create_menu failed\n");
		return ERROR_NO_WIDGET;
	}
	if (append(window->children, panel) == NULL) {
		fprintf(stderr, "Error: append failed\n");
		freeWidget(panel);
		return ERROR_APPEND_FAILED;
	}

	/*-----creating choose player buttons-----*/
	//human
	text_dims.x = MAIN_MENU_T_X_START + 2*WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START + WL_T_H;
	if (append_menu(panel, HUMAN_B, text_dims, choose_action, state) != 0) {
		return ERROR_APPEND_FAILED;
	}

	//machine
	text_dims.x = 0;
	text_dims.y = MAIN_MENU_T_Y_START + 2*WL_T_H;
	if (append_menu(panel, MACHINE_B, text_dims, choose_action, state) != 0) {
		return ERROR_APPEND_FAILED;
	}

	//back
	text_dims.x = MAIN_MENU_T_X_START + WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START + 2*WL_T_H;
	if (append_menu(panel, BACK_B, text_dims, back_action, state) != 0) {
		return ERROR_APPEND_FAILED;
	}

	panel->pos = get_center(window->dims, panel->pos);

	//change the background color of the focused button
	button_dims.y += WL_BUTTON_H;

	if (set_focus_bg(window, button_dims, choose_player_ids[state->focused]) != 0) {
		return ERROR_NO_FOCUS;
	}
	return 0;
}

int build_choose(Widget* window, game_state* state) {
	Widget *panel;
	SDL_Rect button_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect text_dims;
	SDL_Rect title_pos;

	text_dims.w = WL_T_W;
	text_dims.h = WL_T_H;

	/* Create the title */
	if (state->type == CHOOSE_SKILL) {
		if (state->catormouse == CAT) { // "Choose cat skill level"
			text_dims.x = TITLES_T_X_START;
			text_dims.y = TITLES_T_Y_START + 4*WL_T_H;
		} else { // "Choose mouse skill level"
			text_dims.x = TITLES_T_X_START + WL_T_W;
			text_dims.y = TITLES_T_Y_START + 4*WL_T_H;
		}
	} else if (state->type == LOAD_GAME) {
		text_dims.x = TITLES_T_X_START + 2*WL_T_W;
		text_dims.y = TITLES_T_Y_START - WL_T_H;
	} else if (state->type == SAVE_GAME) {
		text_dims.x = TITLES_T_X_START + 2*WL_T_W;
		text_dims.y = TITLES_T_Y_START;
	} else if (state->type == EDIT_GAME) {
		text_dims.x = TITLES_T_X_START;
		text_dims.y = TITLES_T_Y_START + 2*WL_T_H;
	} else {
		// TODO error
		return -1;
	}

	title_pos = get_center(window->dims, text_dims);
	title_pos.y = WL_BUTTON_H;
	if (new_graphic(UNFOCUSABLE, text_dims, title_pos, texts, window) == NULL) {
		fprintf(stderr, "Error: new_graphic failed\n");
		return ERROR_APPEND_FAILED;
	}

	/* creating a new panel for the buttons */
	button_dims.x = 0;
	button_dims.y = WL_BUTTON_H;
	panel = create_menu(button_dims, window);
	if (panel == NULL) {
		fprintf(stderr, "Error: create_menu failed\n");
		return ERROR_NO_WIDGET;
	}
	if (append(window->children, panel) == NULL) {
		fprintf(stderr, "Error: append failed\n");
		freeWidget(panel);
		return ERROR_APPEND_FAILED;
	}

	/*-----creating choose number buttons-----*/
	//chooser
	button_dims.x = 0;
	button_dims.y = WL_BUTTON_H;
	text_dims.x = MAIN_MENU_T_X_START + 2*WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START + WL_T_H;

	if (state->type == CHOOSE_SKILL) {
		if (append_menu(panel, LEVEL_CHOOSER, text_dims, NULL, state) != 0) {
			return ERROR_APPEND_FAILED;
		}
	} else { // world chooser
		if (append_menu(panel, WORLD_CHOOSER, text_dims, NULL, state) != 0) {
			return ERROR_APPEND_FAILED;
		}
	}

	//done button
	text_dims.x = MAIN_MENU_T_X_START + 2*WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START + 2*WL_T_H;
	if (append_menu(panel, DONE_B, text_dims, choose_action, state) != 0) {
		return ERROR_APPEND_FAILED;
	}

	//back button
	text_dims.x = MAIN_MENU_T_X_START + WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START + 2*WL_T_H;
	if (append_menu(panel, BACK_B, text_dims, back_action, state) != 0) {
		return ERROR_APPEND_FAILED;
	}

	panel->pos = get_center(window->dims, panel->pos);

	//change the background color of the focused widget
	button_dims.y += WL_BUTTON_H;

	if (state->type == CHOOSE_SKILL) {
		if (set_focus_bg(window, button_dims, choose_skill_ids[state->focused]) != 0) {
			return ERROR_NO_FOCUS;
		}
	} else {
		if (set_focus_bg(window, button_dims, load_game_ids[state->focused]) != 0) {
			return ERROR_NO_FOCUS;
		}
	}
	return 0;
}

int build_error_dialog(Widget* window, game_state* state) {
	Widget *panel, *button;
	SDL_Rect pos = {0,0,0,0};
	SDL_Rect button_dims = {0, 2*WL_BUTTON_H, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect dims = {ERR_T_X_START, ERR_T_Y_START, ERR_T_W, ERR_T_H};
	pos.h = dims.h * 1.7 + button_dims.h;
	pos.w = dims.w;
	
	// We set real pos later
	panel = new_panel(UNFOCUSABLE, pos, window);
	if (panel == NULL) {
		fprintf(stderr, "Error: new_panel failed\n");
		return ERROR_NO_WIDGET;
	}
	
	dims.y += state->number * ERR_T_H; // in state->number we save the error number in the sprite
	if (new_graphic(UNFOCUSABLE, dims, pos, texts, panel) == NULL) {
		fprintf(stderr, "Error: appending invalid world message\n");
		return ERROR_APPEND_FAILED;
	}
	
	pos.y += dims.h * 1.7;
	
	SDL_Rect text_dims = {MAIN_MENU_T_X_START + WL_T_W, MAIN_MENU_T_Y_START + 2*WL_T_H, WL_T_W, WL_T_H};
	button = build_text_button(BACK_B, pos, button_dims, text_dims, panel, back_action);
	if (button == NULL) {
		fprintf(stderr, "Error: creating button failed\n");
		return ERROR_NO_WIDGET;
	}
	if (append(panel->children, button) == NULL) {
		fprintf(stderr, "Error: appending back button failed\n");
		freeWidget(button);
		return ERROR_APPEND_FAILED;
	}
	panel->pos = get_center(window->dims, panel->pos); // fix position for whole panel
	return 0;
}

int build_ui(Widget* window, game_state* state) {
	ListRef children;
	if ((children = window->children) != NULL) {
		destroyList(children, &freeWidget);
	}

	children = newList(NULL);
	window->children = children; // note: if this is here, we must used window->children throughout the entire function

	switch (state->type) {
		case MAIN_MENU:
			return build_main_menu(window, state);
		case CHOOSE_PLAYER:
			return build_choose_player(window, state);
		case CHOOSE_SKILL:
		case EDIT_GAME:
		case LOAD_GAME:
		case SAVE_GAME:
			return build_choose(window, state);
		case IN_GAME:
		case GAME_EDIT:
			return build_game_scheme(window, state);
		case ERROR_DIALOG:
			return build_error_dialog(window, state);
		default:
			return 0;
	}
	return 0;
}
