#include "uitree.h"

int build_main_menu(Widget* window, game_state* state) {
	ListRef children;
	if ((children = window->children) != NULL) {
		distroyList(children, &freeWidget);
	}

	children = newList(NULL);

	// Create the first graphic figure, taken from the spritesheet imgsrc - TODO add it somewhere
	SDL_Rect dims = {30,30,100,20}; //x y w h
	SDL_Rect pos = {30,30,100,20};
	append(children, newGraphic(id, dims, pos, SDL_Surface* imgsrc));
}

 //another parameter should be (array) SDL_Surface** sprite sheets? Or they should be global
int build_ui(Widget* window, game_state* state) {
	switch (state->type) {
		case MAIN_MENU:
			return build_main_menu(window, state);
	}
}