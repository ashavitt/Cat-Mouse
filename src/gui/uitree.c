#include "uitree.h"

int build_main_menu(Widget* window, game_state* state) {
	ListRef children;
	if ((children = window->children) != NULL) {
		distroyList(children, &freeWidget);
	}

	children = newList(NULL);
}

int build_ui(Widget* window, game_state* state) {
	switch (state->type) {
		case MAIN_MENU:
			return build_main_menu(window, state);
	}
}