#include "uitree.h"

// can only print positive numbers
// character spacing?
// TODO braces ()
#define SPACE_BETWEEN_WORLD_AND_NUMBER 20
Widget* number_to_graphic(int id, SDL_Rect main_pos, int number, int size, Widget* parent) {
	SDL_Rect zeros = {0,0,0,0};
	Widget* text = new_panel(id, main_pos, parent);
	SDL_Rect zero_dims;
	SDL_Rect dims;
	SDL_Rect pos = zeros; // TODO
	SDL_Rect offset_rect;
	int digit,width,height;
	int n;
	ListRef digits;

	if (text == NULL) {
		printf("Error text is null\n");
		return NULL;
	}
	digits = text->children;

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
			printf("Error - bad size\n");
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
		if (append(digits, new_graphic(UNFOCUSABLE, dims, pos, texts, text)) == NULL) {
			printf("Error in append: 1");
			return NULL;
		}
		pos.x -= offset_rect.x;
	}

	while (number != 0) {
		digit = number % 10;
		dims = zero_dims; // TODO memcpy?
		dims.x += offset_rect.x * digit;
		if (append(digits, new_graphic(UNFOCUSABLE, dims, pos, texts, text)) == NULL) {
			printf("Error in append: 2");
			return NULL;
		}
		//add_rect(&pos, &offset_rect);
		pos.x -= offset_rect.x; // move position left for next number (negative coordinates)
		number /= 10;
	}

	if (size == TEXT_SIZE_SMALL) {
		dims = zero_dims;
		dims.x -= 2*S_NUM_T_W; // '('
		if (append(digits, new_graphic(UNFOCUSABLE, dims, pos, texts, text)) == NULL) {
			printf("Error in append: 3");
			return NULL;
		}
		pos.x -= offset_rect.x;
	}

	width = n*offset_rect.x;
	height = offset_rect.y;

	add_rect(&(text->pos), &offset_rect); // fix for negative coordinates offset
	text->dims.w = (text->pos.w = width); // DANGER
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
	//printf("%d,%d\n", text_pos.x, text_pos.y);

	//create the widgets
	clickable = new_button(id, main_pos, main_pos, parent, onClick);
	bg = new_graphic(UNFOCUSABLE, bg_dims, zeros, buttons, clickable);
	text = new_graphic(UNFOCUSABLE, text_dims, text_pos, texts, bg);
	//check everything worked
	if (clickable == NULL || bg == NULL || text == NULL) {
		//TODO print error
		return NULL;
	}

	//add the widgets to the list of childrens
	if (append(clickable->children, bg) == NULL) {
		return NULL;
	}
	if (append(bg->children, text) == NULL) {
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
		//TODO print error
		return NULL;
	}

	chooser = new_button(id, main_pos, main_pos, parent, do_nothing_action); // TODO do_nothing
	bg = new_graphic(UNFOCUSABLE, bg_dims, zeros, buttons, chooser);

	if (chooser == NULL || bg == NULL) {
		//TODO print error
		return NULL;
	}

	//text = new_panel(UNFOCUSABLE, text_dims, bg); // text_pos = zeros
	if (state->type == CHOOSE_SKILL) {
		// numbers
		text = number_to_graphic(UNFOCUSABLE, zeros, state->number, TEXT_SIZE_MEDIUM, bg);
		if (text == NULL) {
			//TODO print error
			return NULL;
		}
		text->pos = get_center(text_dims, text->dims);
	} else if (state->type == EDIT_GAME || state->type == LOAD_GAME || state->type == SAVE_GAME) {
		// "World " + number
		text = new_panel(UNFOCUSABLE, text_dims, bg);

		// adds the "World "
		if (append(text->children, new_graphic(UNFOCUSABLE, world_t_dims, zeros, texts, text)) != 0) {
			return NULL;
		}
		// pos is zeros
		pos.x += SPACE_BETWEEN_WORLD_AND_NUMBER + world_t_dims.w;
		if ((number = number_to_graphic(UNFOCUSABLE, pos, state->number, TEXT_SIZE_MEDIUM, text)) == NULL){
			// TODO
			return NULL;
		}
		// adds number
		if (append(text->children, number) != 0) {
			return NULL;
		}
		text->dims.w = world_t_dims.w + number->dims.w + SPACE_BETWEEN_WORLD_AND_NUMBER;
		text->dims.h = world_t_dims.h;
		text->pos = get_center(text_dims, text->dims);
	}

	if (text == NULL) {
		//TODO print error
		return NULL;
	}

	arrow_pos.x = text_dims.w;
	up = new_button(LEVEL_UP_B, arrow_dims, arrow_pos, bg, arrow_action);
	up_arrow = new_graphic(UNFOCUSABLE, arrow_dims, zeros, buttons, up);
	if (append(up->children, up_arrow) == NULL) {
		printf("Error appending up_arrow\n");
		return NULL;
	}
	// Move to down arrow
	arrow_pos.y += arrow_dims.h;
	arrow_dims.y += arrow_dims.h;
	down = new_button(LEVEL_DN_B, arrow_dims, arrow_pos, bg, arrow_action);
	down_arrow = new_graphic(UNFOCUSABLE, arrow_dims, zeros, buttons, down);
	if (append(down->children, down_arrow) == NULL) {
		printf("Error appending down_arrow\n");
		return NULL;
	}

	if (append(chooser->children, bg) == NULL) {
		printf("Error appending bg\n");
		return NULL;
	}
	if (append(bg->children, text) == NULL) {
		printf("Error appending text\n");
		return NULL;
	}
	if (append(bg->children, up) == NULL) {
		printf("Error appending up\n");
		return NULL;
	}
	if (append(bg->children, down) == NULL) {
		printf("Error appending down\n");
		return NULL;
	}

	return chooser;
}

Widget* create_menu(SDL_Rect button_dims, Widget* parent) {
	SDL_Rect pos = {0,0,0,0};
	pos.w = button_dims.w;
	Widget* menu = new_panel(UNFOCUSABLE, pos, parent);
	if (menu == NULL) {
		return NULL;
	}
	//set the panel dims as the button dims for later usage
	menu->dims = button_dims;
	return menu;
}

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
	if (append(menu->children, button) == NULL) {
			return ERROR_APPEND_FAILED;
	}
		
	return 0;
}

int set_focus_bg(Widget* window, SDL_Rect new_button_dims, int focused) {
	Widget* button;
	if ((button = find_widget_by_id(window, focused)) == NULL) {
		return ERROR_NO_FOCUS;
	}
	((Widget*)headData(button->children))->dims = new_button_dims;
	return 0;
}

Widget* build_grid(int id, Widget* parent, game_state* state) { 
	Widget *grid, *obj;
	SDL_Rect dims = {0,0,GRID_W,GRID_H};
	SDL_Rect pos = get_center(parent->dims, dims);
	grid = new_graphic(UNFOCUSABLE, dims, pos, grid_surface, parent);
	SDL_Rect tile_dims = {0,0,GRID_W/7,GRID_H/7};
	SDL_Rect tile_pos = {0,0,GRID_W/7,GRID_H/7};

	// add mouse, cat, walls
	Game* game = state->game;
	//cat
	tile_pos.x = game->cat_x * tile_dims.w;
	tile_pos.y = game->cat_y * tile_dims.h;
	obj = new_graphic(UNFOCUSABLE, tile_dims, tile_pos, cat, grid);
	if (append(grid->children, obj) == NULL) {
		return NULL;
	}
	//mouse
	tile_pos.x = game->mouse_x * tile_dims.w;
	tile_pos.y = game->mouse_y * tile_dims.h;
	obj = new_graphic(UNFOCUSABLE, tile_dims, tile_pos, mouse, grid);
	if (append(grid->children, obj) == NULL) {
		return NULL;
	}
	//cheese
	tile_pos.x = game->cheese_x * tile_dims.w;
	tile_pos.y = game->cheese_y * tile_dims.h;
	obj = new_graphic(UNFOCUSABLE, tile_dims, tile_pos, cheese, grid);
	if (append(grid->children, obj) == NULL) {
		return NULL;
	}
	//walls
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (game->board->board[j][i] == WALL) {
				tile_pos.x = i * tile_dims.w;
				tile_pos.y = j * tile_dims.h;
				obj = new_graphic(UNFOCUSABLE, tile_dims, tile_pos, bricks, grid);
				if (append(grid->children, obj) == NULL) {
					return NULL;
				}
			}
		}
	}
	return grid;
}
 
int build_game_scheme(Widget* window, game_state* state) {  
	Widget *title_panel, *top_buttons, *left_panel, *grid_panel, *menu;
	//int y_offset;
	SDL_Rect pos = window->dims;
	SDL_Rect button_dims = {0,0,NL_BUTTON_W,NL_BUTTON_H}, text_dims = {0,0,NL_T_W,NL_T_H};
		
	pos.x = pos.y = 0;
	pos.h = S_NUM_T_H * 4;
	title_panel = new_panel(UNFOCUSABLE, pos, window);

	pos.y += pos.h;
	pos.h = NL_BUTTON_H*1.5;
	top_buttons = new_panel(UNFOCUSABLE, pos, window);

	pos.y += pos.h;
	pos.h = window->dims.h - pos.y;
	pos.w = NL_BUTTON_W*1.15;
	left_panel = new_panel(UNFOCUSABLE, pos, window);

	pos.x += pos.w;
	pos.w = window->dims.w - pos.x;
	grid_panel = new_panel(UNFOCUSABLE, pos, window);

	if (grid_panel == NULL || title_panel == NULL || top_buttons == NULL || left_panel == NULL) {
		printf("Error: Some panel is null\n");
		return ERROR_NO_WIDGET;
	}
	
	/* Top Panel */
	
	/* Top Buttons */
	
	/* Left Panel */ // NOTE: this should be different according to game state
	menu = create_menu(button_dims, left_panel);
	if (menu == NULL) {
		return ERROR_NO_WIDGET;
	}
	if (append_menu(menu, RECONF_MOUSE_B, text_dims, choose_action, state) != 0) {
		printf("Error: appending button\n");
		return ERROR_APPEND_FAILED;
	}
	
	text_dims.x += NL_T_W;
	if (append_menu(menu, RECONF_CAT_B, text_dims, choose_action, state) != 0) {
		printf("Error: appending button\n");
		return ERROR_APPEND_FAILED;
	}
	
	text_dims.x += NL_T_W;
	if (append_menu(menu, RESTART_GAME_B, text_dims, do_nothing_action, state) != 0) {
		printf("Error: appending button\n");
		return ERROR_APPEND_FAILED;
	}

	text_dims.x += NL_T_W;
	if (append_menu(menu, GOTO_MAIN_MENU_B, text_dims, do_nothing_action, state) != 0) {
		printf("Error: appending button\n");
		return ERROR_APPEND_FAILED;
	}

	text_dims.x += NL_T_W;
	if (append_menu(menu, QUIT_B, text_dims, quit_action, state) != 0) {
		printf("Error: appending button\n");
		return ERROR_APPEND_FAILED;
	}

	menu->pos = get_center(left_panel->dims,menu->pos);
	if (append(left_panel->children, menu) == NULL) {
		printf("Error: appending build_grid\n");
		return ERROR_APPEND_FAILED;
	}
	
	/* Grid Panel */	
	if (append(grid_panel->children,build_grid(GRID_B, grid_panel, state)) == 0) {
		printf("Error: appending build_grid\n");
		return ERROR_APPEND_FAILED;
	}

	/* Append panels to window */
	if (append(window->children, title_panel) == 0) {
		printf("Error appending title_panel\n");
		return ERROR_APPEND_FAILED;
	}

	if (append(window->children, top_buttons) == 0) {
		printf("Error appending top_buttons\n");
		return ERROR_APPEND_FAILED;
	}

	if (append(window->children, left_panel) == 0) {
		printf("Error appending left_panel\n");
		return ERROR_APPEND_FAILED;
	}

	if (append(window->children, grid_panel) == 0) {
		printf("Error appending grid_panel\n");
		return ERROR_APPEND_FAILED;
	}

	return 0;
}

int build_main_menu(Widget* window, game_state* state) {
	Widget *panel, *title;
	SDL_Rect button_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect text_dims;
	SDL_Rect title_pos;

	ListRef children;
	if ((children = window->children) != NULL) {
		destroyList(children, &freeWidget);
	}

	children = newList(NULL);

	//create the title
	text_dims.w = WL_T_W;
	text_dims.h = WL_T_H;
	text_dims.x = TITLES_T_X_START;
	text_dims.y = TITLES_T_Y_START;
	title_pos = get_center(window->dims, text_dims);
	//title_pos.y -= 5*WL_BUTTON_H;
	title_pos.y = WL_BUTTON_H;
	title = new_graphic(UNFOCUSABLE, text_dims, title_pos, texts, window);
	if (append(children, title) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//creating a new panel for the buttons
	button_dims.x = 0;
	button_dims.y = WL_BUTTON_H;
	panel = create_menu(button_dims, window);
	if (append(children, panel) == NULL) {
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
	if (append_menu(panel, LOAD_GAME_B, text_dims, NULL, state) != 0) {
		return ERROR_APPEND_FAILED;
	}

	//create game
	text_dims.x = MAIN_MENU_T_X_START + 2*WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START;
	if (append_menu(panel, CREATE_GAME_B, text_dims, NULL, state) != 0) {
		return ERROR_APPEND_FAILED;
	}

	//edit game
	text_dims.x = MAIN_MENU_T_X_START;
	text_dims.y = MAIN_MENU_T_Y_START + WL_T_H;
	if (append_menu(panel, EDIT_GAME_B, text_dims, NULL, state) != 0) {
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
	window->children = children;

	if (set_focus_bg(window, button_dims, main_menu_ids[state->focused]) != 0) {
		return ERROR_NO_FOCUS;
	}
	return 0;
}


int build_choose_player(Widget* window, game_state* state) {
	Widget *panel, *title;
	SDL_Rect button_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect text_dims;
	SDL_Rect title_pos;

	ListRef children;
	if ((children = window->children) != NULL) {
		destroyList(children, &freeWidget);
	}

	children = newList(NULL);

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
	title = new_graphic(UNFOCUSABLE, text_dims, title_pos, texts, window);
	if (append(children, title) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//creating a new panel for the buttons
	button_dims.x = 0;
	button_dims.y = WL_BUTTON_H;
	panel = create_menu(button_dims, window);
	if (append(children, panel) == NULL) {
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
	window->children = children;

	if (set_focus_bg(window, button_dims, choose_player_ids[state->focused]) != 0) {
		return ERROR_NO_FOCUS;
	}
	return 0;
}

int build_choose(Widget* window, game_state* state) {
	Widget *panel, *title;
	SDL_Rect button_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect text_dims;
	SDL_Rect title_pos;

	ListRef children;
	if ((children = window->children) != NULL) {
		destroyList(children, &freeWidget);
	}

	children = newList(NULL);

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
	title = new_graphic(UNFOCUSABLE, text_dims, title_pos, texts, window);
	if (append(children, title) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	/* creating a new panel for the buttons */
	button_dims.x = 0;
	button_dims.y = WL_BUTTON_H;
	panel = create_menu(button_dims, window);
	if (append(children, panel) == NULL) {
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
	window->children = children;

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
			return build_game_scheme(window, state);
		default:
			return 0;
	}
	return 0;
}
