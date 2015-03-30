#include "widget.h"

int add_rect(SDL_Rect* rect1, SDL_Rect* rect2) {
	if (rect1 == NULL) {
		return ERROR_NO_RECT1;
	}
	if (rect2 == NULL) {
		return ERROR_NO_RECT2;
	}
	rect1->x = rect1->x + rect2->x;
	rect1->y = rect1->y + rect2->y;
	return 0;

	//maybe change w,h
}

int draw_widget(Widget* widget, Widget* window, SDL_Rect abs_pos) {
	ListRef children;
	int err;
	if (widget == NULL) {
		return ERROR_NO_WIDGET;
	}
	if (window == NULL) {
		//check if I'm root
		if (widget->type == WINDOW) {
			return draw_widget(widget, widget, abs_pos);
		}
		else {
			return ERROR_NO_WINDOW;
		}
	}
	if (widget->type == GRAPHIC || widget->type == BUTTON) {
		if (SDL_BlitSurface(widget->imgsrc, &(widget->dims), window, &(widget->pos)) != 0) {
			SDL_FreeSurface(img);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			return ERROR_BLIT_FAIL; //TODO
		}
	}

	if ((err = add_rect(&abs_pos,widget->pos)) != 0) {
		return err;
	}
	
	children = widget->children;
	//run over children
	while (children != NULL) { //we run over the list of children
		if ((err = draw_widget((Widget*) headData(children), window, abs_pos)) != 0) {
			return err;
		}
		children = tail(children); //cutting the head
	}

	return 0;
}