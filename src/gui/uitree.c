#include "uitree.h"

Widget* build_text_button(int id, SDL_Rect main_pos, SDL_Rect bg_dims, SDL_Rect text_dims, Widget* parent, onclick onClick) {
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
	Widget* panel;
	Widget* button;
	//height is height of 5 buttons + 0.5 button per spacing between buttons
	SDL_Rect panel_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H*7};
	SDL_Rect button_dims = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect button_pos = {0, 0, WL_BUTTON_W, WL_BUTTON_H};
	SDL_Rect button_offset = {0, WL_BUTTON_H * 1.5, 0, 0};
	SDL_Rect text_dims;
	
	ListRef children, buttons;
	if ((children = window->children) != NULL) {
		destroyList(children, &freeWidget);
	}

	children = newList(NULL);

	//creating a new panel for the buttons
	panel = new_panel(UNFOCUSABLE, get_center(window->dims, panel_dims), window);
	if (append(children, panel) == NULL) {
		return ERROR_APPEND_FAILED;
	}
	
	//-----creating the buttons-----
	//new game
	button_dims.x = 0;
	button_dims.y = WL_BUTTON_H;
	text_dims.x = MAIN_MENU_T_X_START;
	text_dims.y = MAIN_MENU_T_Y_START;
	text_dims.w = WL_T_W;
	text_dims.h = WL_T_H;
	button = build_text_button(NEW_GAME_B, button_pos, button_dims, text_dims, panel, NULL);
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
	text_dims.w = WL_T_W;
	text_dims.h = WL_T_H;
	button = build_text_button(LOAD_GAME_B, button_pos, button_dims, text_dims, panel, NULL);
	if (append(panel->children, button) == NULL) {
		return ERROR_APPEND_FAILED;
	}

	//change the background color of the focused button
	buttons = panel->children;
	button_dims.y *= 2;
	while (buttons != NULL) {
		if (headData(buttons) != NULL) {
			button = ((Widget*)headData(buttons));
			if (button->id == state->focused) {
				//set the correct color for the button
				((Widget*)headData(button->children))->dims = button_dims;
			}
		}
		buttons = tail(buttons);
	}



	//SDL_Rect dims = {30,30,100,20}; //x y w h
	//SDL_Rect pos = {30,30,100,20};
	//if (append(children, new_graphic(id, dims, pos, texts, window)) == NULL) {
	//	return ERROR_APPEND_FAILED;
	//}
	window->children = children;
	return 0;
}

 //another parameter should be (array) SDL_Surface** sprite sheets? Or they should be global
int build_ui(Widget* window, game_state* state) {
	switch (state->type) {
		case MAIN_MENU:
			return build_main_menu(window, state);
		default:
			return 0;
	}
	return 0;
}
