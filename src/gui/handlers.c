#include "handlers.h"

int handle_mouse_click_rec(SDL_Event* e, Widget* widget, game_state* state, SDL_Rect abs_pos) {
	ListRef children;
	int err;
	SDL_Rect dims = widget->dims;
	if (e == NULL) {
		return ERROR_NO_SDLEVENT;
	}
	if ((err = add_rect(&abs_pos,&(widget->pos))) != 0) {
		printf("Error in add_rect, %d\n",err);
		return err;
	}

	if (widget->type == BUTTON) {
		if ((e->button.x > abs_pos.x) && (e->button.x < abs_pos.x + dims.w) 
			&& (e->button.y > abs_pos.y) && (e->button.y < abs_pos.y + dims.h)) {
			if ((err = (widget->onclick)(widget, state)) != 0) {
				printf("Error in onclick func of widget, code %d\n",err);
				return err;
			}
		}
	}
//???
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

int handle_keyboard(SDL_Event* event, Widget* window, game_state* state) {
	Widget* widget;
	int err;
	switch(event->key.keysym.sym) {
		case SDLK_ESCAPE:
			return 1;
		case SDLK_RETURN:
			widget = find_widget_by_id(window, state->focused);
			if (widget == NULL) {
				// focused widget must exist
				printf("ERROR no widget with id:%d was found",state->focused);
				return ERROR_FOCUSED_ID;
			}
			if ((err = (widget->onclick)(widget, state)) != 0) {
				// "NULL pointer exception"
				printf("Error in onclick func of widget, code %d\n",err);
				return err;
			}
			break;
		case SDLK_TAB:
			// move to next focusable
			break;
		case SDLK_UP:
		case SDLK_DOWN:
		case SDLK_RIGHT:
		case SDLK_LEFT:
			if (state->type == IN_GAME || state->type == GAME_EDIT) {
				//move cursor to direction event->key.keysym.sym
			}
			break;
		//cases for in-game or edit-game later
		default:
			break;
	}
	return 0;
}
