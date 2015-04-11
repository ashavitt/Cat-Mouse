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
	int n = 0;
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
	//x_offset = zero_dims.w;
	offset_rect.x = zero_dims.w;
	offset_rect.y = zero_dims.h;
	sub_rect(&pos, &offset_rect); // change offset for negative coordinates
	
	if (size == TEXT_SIZE_SMALL) {
		dims = zero_dims;
		dims.x -= S_NUM_T_W; // ')'
		if (append(digits, new_graphic(UNFOCUSABLE, dims, pos, texts, text)) == NULL) {
			printf("Error in append: 1");
			return NULL;
		}
		pos.x -= offset_rect.x;
		n++;
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
		n++; // count digits
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
		n++;
	}
	
	width = n*offset_rect.x;
	height = offset_rect.y;
	
	text->pos.x += width; // fix for negative coordinates
	add_rect(&(text->pos), &offset_rect); // fix for negative coordinates offset
	text->dims.w = (text->pos.w = width); // DANGER
	text->dims.h = (text->pos.h = height);
	
	return text;
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

int build_main_menu(Widget* window, game_state* state) {
	int err;
	Widget *panel, *button, *title;
	//height is height of 5 buttons + 0.5 button per spacing between buttons
	SDL_Rect panel_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H*7};
	SDL_Rect button_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect button_pos = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect button_offset = {0, WL_BUTTON_H * 1.5, 0, 0};
	SDL_Rect text_dims;
	SDL_Rect title_pos;
	
	ListRef children, buttons;
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
	panel = new_panel(UNFOCUSABLE, get_center(window->dims, panel_dims), window);
	if (append(children, panel) == NULL) {
		return ERROR_APPEND_FAILED;
	}
	
	/*-----creating main menu buttons-----*/
	//new game
	button_dims.x = 0;
	button_dims.y = WL_BUTTON_H;
	text_dims.x = MAIN_MENU_T_X_START;
	text_dims.y = MAIN_MENU_T_Y_START;
	button = build_text_button(NEW_GAME_B, button_pos, button_dims, text_dims, panel, new_game_action);
	if (append(panel->children, button) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//load game
	if ((err = add_rect(&button_pos, &button_offset)) != 0) {
		printf("Error in add_rect: %d\n", err);
		return ERROR_ADD_RECT_FAILED;
	}
	text_dims.x = MAIN_MENU_T_X_START + WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START;
	button = build_text_button(LOAD_GAME_B, button_pos, button_dims, text_dims, panel, NULL);
	if (append(panel->children, button) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//create game
	if ((err = add_rect(&button_pos, &button_offset)) != 0) {
		printf("Error in add_rect: %d\n", err);
		return ERROR_ADD_RECT_FAILED;
	}
	text_dims.x = MAIN_MENU_T_X_START + 2*WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START;
	button = build_text_button(CREATE_GAME_B, button_pos, button_dims, text_dims, panel, NULL);
	if (append(panel->children, button) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//edit game
	if ((err = add_rect(&button_pos, &button_offset)) != 0) {
		printf("Error in add_rect: %d\n", err);
		return ERROR_ADD_RECT_FAILED;
	}
	text_dims.x = MAIN_MENU_T_X_START;
	text_dims.y = MAIN_MENU_T_Y_START + WL_T_H;
	button = build_text_button(EDIT_GAME_B, button_pos, button_dims, text_dims, panel, NULL);
	if (append(panel->children, button) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//quit
	if ((err = add_rect(&button_pos, &button_offset)) != 0) {
		printf("Error in add_rect: %d\n", err);
		return ERROR_ADD_RECT_FAILED;
	}
	text_dims.x = MAIN_MENU_T_X_START + WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START + WL_T_H;
	button = build_text_button(QUIT_B, button_pos, button_dims, text_dims, panel, quit_action);
	if (append(panel->children, button) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//change the background color of the focused button
	buttons = panel->children;
	button_dims.y *= 2;
	while (buttons != NULL) {
		if (headData(buttons) != NULL) {
			button = ((Widget*)headData(buttons));
			if (button->id == main_menu_ids[state->focused]) {
				//set the correct color for the button
				((Widget*)headData(button->children))->dims = button_dims;
			}
		}
		buttons = tail(buttons);
	}

	window->children = children;
	return 0;
}

int build_choose_player(Widget* window, game_state* state) {
	int err;
	Widget *panel, *button, *title;
	//height is height of 3 buttons + 0.5 button per spacing between buttons
	SDL_Rect panel_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H*4};
	SDL_Rect button_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect button_pos = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect button_offset = {0, WL_BUTTON_H * 1.5, 0, 0};
	SDL_Rect text_dims;
	SDL_Rect title_pos;
	
	ListRef children, buttons;
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
	panel = new_panel(UNFOCUSABLE, get_center(window->dims, panel_dims), window);
	if (append(children, panel) == NULL) {
		return ERROR_APPEND_FAILED;
	}
	
	/*-----creating choose player buttons-----*/
	//human
	button_dims.x = 0;
	button_dims.y = WL_BUTTON_H;
	text_dims.x = MAIN_MENU_T_X_START + 2*WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START + WL_T_H;
	
	button = build_text_button(HUMAN_B, button_pos, button_dims, text_dims, panel, choose_action);
	if (append(panel->children, button) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//machine
	if ((err = add_rect(&button_pos, &button_offset)) != 0) {
		printf("Error in add_rect: %d\n", err);
		return ERROR_ADD_RECT_FAILED;
	}
	text_dims.x = 0;
	text_dims.y = MAIN_MENU_T_Y_START + 2*WL_T_H;
	button = build_text_button(MACHINE_B, button_pos, button_dims, text_dims, panel, choose_action);
	if (append(panel->children, button) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//back
	if ((err = add_rect(&button_pos, &button_offset)) != 0) {
		printf("Error in add_rect: %d\n", err);
		return ERROR_ADD_RECT_FAILED;
	}
	text_dims.x = MAIN_MENU_T_X_START + WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START + 2*WL_T_H;
	button = build_text_button(BACK_B, button_pos, button_dims, text_dims, panel, back_action);
	if (append(panel->children, button) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//change the background color of the focused button
	buttons = panel->children;
	button_dims.y *= 2; // ONLY APPLIES FOR MAIN MENUS
	while (buttons != NULL) {
		if (headData(buttons) != NULL) {
			button = ((Widget*)headData(buttons));
			if (button->id == choose_player_ids[state->focused]) {
				//set the correct color for the button
				((Widget*)headData(button->children))->dims = button_dims;
			}
		}
		buttons = tail(buttons);
	}

	window->children = children;
	return 0;
}

int build_choose(Widget* window, game_state* state) {
	int err;
	Widget *panel, *button, *title;
	//height is height of 3 buttons + 0.5 button per spacing between buttons
	SDL_Rect panel_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H*4};
	SDL_Rect button_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect button_pos = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect button_offset = {0, WL_BUTTON_H * 1.5, 0, 0};
	SDL_Rect text_dims;
	SDL_Rect title_pos;
	
	ListRef children, buttons;
	if ((children = window->children) != NULL) {
		destroyList(children, &freeWidget);
	}

	children = newList(NULL);

	text_dims.w = WL_T_W;
	text_dims.h = WL_T_H;
	
	/* Create the title */
	if (state->type == CHOOSE_SKILL) {
		if (state->catormouse == CAT) { // "Choose cat skill level"
			text_dims.x = TITLES_T_X_START + WL_T_W;
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
	panel = new_panel(UNFOCUSABLE, get_center(window->dims, panel_dims), window);
	if (append(children, panel) == NULL) {
		return ERROR_APPEND_FAILED;
	}
	
	/*-----creating choose number buttons-----*/
	//chooser
	button_dims.x = 0;
	button_dims.y = WL_BUTTON_H;
	text_dims.x = MAIN_MENU_T_X_START + 2*WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START + WL_T_H;
	
	//button = build_text_button(HUMAN_B, button_pos, button_dims, text_dims, panel, choose_action);
	if (state->type == CHOOSE_SKILL) {
		button = build_chooser(LEVEL_CHOOSER, button_pos, button_dims, panel, state);
	} else { // world chooser
		button = build_chooser(WORLD_CHOOSER, button_pos, button_dims, panel, state);
	}
	if (append(panel->children, button) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//done button
	if ((err = add_rect(&button_pos, &button_offset)) != 0) {
		printf("Error in add_rect: %d\n", err);
		return ERROR_ADD_RECT_FAILED;
	}
	text_dims.x = MAIN_MENU_T_X_START + 2*WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START + 2*WL_T_H;
	button = build_text_button(MACHINE_B, button_pos, button_dims, text_dims, panel, choose_action);
	if (append(panel->children, button) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//back button
	if ((err = add_rect(&button_pos, &button_offset)) != 0) {
		printf("Error in add_rect: %d\n", err);
		return ERROR_ADD_RECT_FAILED;
	}
	text_dims.x = MAIN_MENU_T_X_START + WL_T_W;
	text_dims.y = MAIN_MENU_T_Y_START + 2*WL_T_H;
	button = build_text_button(BACK_B, button_pos, button_dims, text_dims, panel, back_action);
	if (append(panel->children, button) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//change the background color of the focused widget
	buttons = panel->children;
	button_dims.y *= 2; // ONLY APPLIES FOR MAIN MENUS
	while (buttons != NULL) {
		if (headData(buttons) != NULL) {
			button = ((Widget*)headData(buttons));
			if (state->type == CHOOSE_SKILL) {
				if (button->id == choose_skill_ids[state->focused]) {
					//set the correct color for the button
					((Widget*)headData(button->children))->dims = button_dims;
				}
			} else { //world chooser
				if (button->id == load_game_ids[state->focused]) {
					//set the correct color for the button
					((Widget*)headData(button->children))->dims = button_dims;
				}
			}
		}
		buttons = tail(buttons);
	}

	window->children = children;
	return 0;
}

int build_ui(Widget* window, game_state* state) {
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
		default:
			return 0;
	}
	return 0;
}
