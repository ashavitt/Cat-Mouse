#include "uitree.h"

Widget* build_text_button(int id, SDL_Rect main_pos, SDL_Rect bg_dims, SDL_Rect text_dims, Widget* parent, int (*onClick)(Widget*)) {
	Widget* clickable;
	Widget* bg;
	Widget* text;
	SDL_Rect zeros = {0,0,0,0};
	//set the position of the text in the center of the button graphic
	SDL_Rect text_pos = get_center(bg_dims, text_dims);
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
	append(clickable->children, bg);
	append(bg->children, text);
	return clickable;
}

int build_main_menu(Widget* window, game_state* state) {
	int id = 2; //TODO change this
	ListRef children;
	if ((children = window->children) != NULL) {
		destroyList(children, &freeWidget);
	}

	children = newList(NULL);

	SDL_Rect dims = {30,30,100,20}; //x y w h
	SDL_Rect pos = {30,30,100,20};
	if (append(children, new_graphic(id, dims, pos, texts, window)) == NULL) {
		return ERROR_APPEND_FAILED;
	}
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
