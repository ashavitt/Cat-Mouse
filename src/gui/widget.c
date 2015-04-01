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


// TODOOOOO!!!!
void freeWidget(void* data) {
	Widget* widget = (Widget*) data;
	ListRef children;
	if (data==NULL) {
		return;
	}

	children = widget->children;
	if (children == NULL) {
		free(widget);
		return;
	}
	// while (children != NULL) { //we run over the list of children
	// 	freeWidget((void*) headData(children));
	// 	children = tail(children); //cutting the head
	// }
	distroyList(children,&freeWidget);

	free(widget);
	return;
}

// widget is pre-malloced for size(Widget)
// returns widget
Widget* widgetFactory(Widget* widget, int id, widget_type type, SDL_Rect dims, SDL_Rect pos, SDL_Surface* imgsrc,
	Widget* parent, ListRef children, byte focused, byte updated) {
	//Widget* widget = (Widget*) malloc(sizeof(Widget));
	widget->id = id;
	widget->type = type;
	widget->dims = dims;
	widget->pos = pos;
	widget->imgsrc = imgsrc;
	widget->parent = parent;
	widget->children = children;
	widget->focused = focused;
	widget->updated = updated;
	
	// errors?
	return widget;
}

// widget is pre-malloced for size(Widget)
Widget* graphicFactory(Widget* widget, int id,  SDL_Rect dims, SDL_Rect pos, SDL_Surface* imgsrc,
	Widget* parent, ListRef children, byte focused, byte updated) {
	widget_type type = GRAPHIC;
	return widgetFactory(widget, id, type, dims, pos, imgsrc, parent, children, focused, updated);
}

// mallocs the widget!
Widget* newGraphic(int id,  SDL_Rect dims, SDL_Rect pos, SDL_Surface* imgsrc) {
	Widget* widget = (Widget*) malloc(sizeof(Widget));
	return graphicFactory(widget, id, GRAPHIC, dims, pos, imgsrc, NULL, NULL, 0, 0); // WHAT IS UPDATED?
}