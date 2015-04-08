#include "uitree.h"

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
