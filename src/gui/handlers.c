#include "handlers.h"

int handle_mouse_click_rec(SDL_Event* e, Widget* widget, game_state* state, SDL_Rect abs_pos) {
	ListRef children;
	int err;
	SDL_Rect dims = widget->dims;
	if (e == NULL) {
		return ERROR_NO_SDLEVENT;
	}
	if (window == NULL) {
		return ERROR_NO_WINDOW;
	}
	if ((err = add_rect(&abs_pos,&(widget->pos))) != 0) {
		printf("Error in add_rect, %d\n",err);
		return err;
	}

	if (widget->type == BUTTON) {
		if ((e.button.x > abs_pos.x) && (e.button.x < abs_pos.x + dims.w) 
			&& (e.button.y > abs_pos.y) && (e.button.y < abs_pos.y + dims.h)) {
			if ((err = (widget->onclick)(widget, state)) != 0) {
				printf("Error in onclick func of widget, code %d\n",err);
				return err;
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
		if ((err = handle_mouse_click_rec(e, (Widget*) headData(children), state, abs_pos) != 0) {
			printf("error handling click for child\n");
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
