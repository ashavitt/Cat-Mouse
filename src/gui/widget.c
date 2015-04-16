#include "widget.h"
#include <stdio.h>

int sub_rect(SDL_Rect* rect1, SDL_Rect* rect2) {
	if (rect1 == NULL) {
		return ERROR_NO_RECT1;
	}
	if (rect2 == NULL) {
		return ERROR_NO_RECT2;
	}
	rect1->x = rect1->x - rect2->x;
	rect1->y = rect1->y - rect2->y;
	return 0;

	//maybe change w,h
}

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

int draw_widget(Widget* widget, SDL_Surface* window, SDL_Rect abs_pos) {
	ListRef children;
	int err = 0;
	if (widget == NULL) {
		return ERROR_NO_WIDGET;
	}
	if (window == NULL) {
		return ERROR_NO_WINDOW;
	}
	if ((err = add_rect(&abs_pos,&(widget->pos))) != 0) {
		return err;
	}
	if (widget->type == GRAPHIC) {
		//if (SDL_BlitSurface(widget->imgsrc, &(widget->dims), window, &(widget->pos)) != 0) {
		if (SDL_BlitSurface(widget->imgsrc, &(widget->dims), window, &abs_pos) != 0) {
			//SDL_FreeSurface(img);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			return ERROR_BLIT_FAIL; //TODO
		}
	}

	//TODO later we should cut the graphics to fit in the panel
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
		if ((err = draw_widget((Widget*) headData(children), window, abs_pos)) != 0) {
			printf("error drawing child\n");
			return err;
		}
		children = tail(children); //cutting the head
	}

	return err;
}

// TODO
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
	destroyList(children,&freeWidget);

	free(widget);
	return;
}

Widget* find_widget_by_id(Widget* root, int id) {
	ListRef children;
	Widget* found = NULL;
	if (root == NULL) {
		return NULL;
	}
	
	if (root->id == id) {
		return root;
	}
	
	children = root->children;
	
	while (children != NULL) {
		if (isEmpty(children)) {
			break;
		}
		if ((found = find_widget_by_id((Widget*) headData(children), id)) != NULL) {
			return found;
		}
		children = tail(children); //cutting the head
	}
	return NULL;
}

// returns a position which centerizes the child within the parent
SDL_Rect get_center(SDL_Rect parent_dims, SDL_Rect size) {
	SDL_Rect children_pos;
	children_pos.x = (parent_dims.w - size.w)/2;
	children_pos.y = (parent_dims.h - size.h)/2;
	children_pos.w = size.w;
	children_pos.h = size.h;
	return children_pos;
}

// widget is pre-malloced for size(Widget)
// returns widget
int widgetFactory(Widget* widget, int id, widget_type type, SDL_Rect dims, SDL_Rect pos, SDL_Surface* imgsrc,
	Widget* parent, ListRef children, byte focused, byte updated, onclick* onClick) {
	//Widget* widget = (Widget*) malloc(sizeof(Widget));
	widget->id = id;
	widget->type = type;
	widget->dims = dims;
	widget->pos = pos;
	widget->imgsrc = imgsrc;
	widget->parent = parent;
	if (children == NULL) {
		children = newList(NULL);
		if (children == NULL) {
			return ERROR_MALLOC_FAILED;
		}
	}
	widget->children = children;
	widget->focused = focused;
	widget->updated = updated;
	widget->onclick = onClick;
	
	return 0;
}

// widget is pre-malloced for size(Widget)
int graphicFactory(Widget* widget, int id,  SDL_Rect dims, SDL_Rect pos, SDL_Surface* imgsrc,
	Widget* parent, ListRef children, byte focused, byte updated) {
	widget_type type = GRAPHIC;
	return widgetFactory(widget, id, type, dims, pos, imgsrc, parent, children, focused, updated, NULL);
}

// mallocs the widget!
Widget* new_graphic(int id,  SDL_Rect dims, SDL_Rect pos, SDL_Surface* imgsrc, Widget* parent) {
	Widget* widget = (Widget*) malloc(sizeof(Widget));
	if (widget == NULL) {
		printf("Error: malloc failed.\n");
		return NULL;
	}
	if (graphicFactory(widget, id, dims, pos, imgsrc, parent, NULL, 0, 1) != 0) {
		printf("graphicFactory failed.\n");
		return NULL;
	}
	return widget;
}

int buttonFactory(Widget* widget, int id, SDL_Rect dims, SDL_Rect pos, Widget* parent,
	ListRef children, byte focused, byte updated, onclick* onClick) {
	widget_type type = BUTTON;
	return widgetFactory(widget, id, type, dims, pos, NULL, parent, children, focused, updated, onClick);
}

int panelFactory(Widget* widget, int id, SDL_Rect pos, Widget* parent, ListRef children, byte focused, byte updated) {
	widget_type type = PANEL;
	return widgetFactory(widget, id, type, pos, pos, NULL, parent, children, focused, updated, NULL);
}

Widget* new_button(int id, SDL_Rect dims, SDL_Rect pos, Widget* parent, onclick* onClick) {
	Widget* widget = (Widget*) malloc(sizeof(Widget));
	if (widget == NULL) {
		//TODO error message
		return NULL;
	}
	if (buttonFactory(widget, id, dims, pos, parent, NULL, 0, 1, onClick) != 0) {
		printf("buttonFactory failed.\n");
		return NULL;
	}
	return widget;
}

Widget* new_panel(int id, SDL_Rect pos, Widget* parent) {
	Widget* widget = (Widget*) malloc(sizeof(Widget));
	if (widget == NULL) {
		//TODO
		return NULL;
	}
	if (panelFactory(widget, id, pos, parent, NULL, 0, 1) != 0) {
		printf("panelFactory failed.\n");
		return NULL;
	}
	return widget;
}
