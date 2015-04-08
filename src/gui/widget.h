#ifndef __WIDGET__
#define __WIDGET__

// includes
#include "shared.h"
#include "../main/ListUtils.h"
#include "../error/error.h"

#ifndef __BYTE__
#define __BYTE__
typedef unsigned char byte;
#endif

typedef enum {
	WINDOW, PANEL, GRAPHIC, BUTTON
} widget_type;

typedef struct widget
{
	int id;
	widget_type type;

	//unsigned int x,y,w,h; // from top left of parent
	SDL_Rect dims;
	SDL_Rect pos; // destination position - should be relative to parent
	SDL_Surface* imgsrc;

	struct widget* parent;

	ListRef children; // of struct widget

	byte focused;
	byte updated;

	int (*onclick)(struct widget*);
} Widget;

int add_rect(SDL_Rect* rect1, SDL_Rect* rect2);

void freeWidget(void* data);

SDL_Rect get_center(SDL_Rect parent_dims, SDL_Rect size);

Widget* new_graphic(int id,  SDL_Rect dims, SDL_Rect pos, SDL_Surface* imgsrc, Widget* parent);

Widget* new_button(int id,  SDL_Rect dims, SDL_Rect pos, Widget* parent, int (*onClick)(Widget*));

Widget* new_panel(int id, SDL_Rect pos, Widget* parent);

int draw_widget(Widget* widget, SDL_Surface* window, SDL_Rect abs_pos);

int widgetFactory(Widget* widget, int id, widget_type type, SDL_Rect dims, SDL_Rect pos, SDL_Surface* imgsrc,
	Widget* parent, ListRef children, byte focused, byte updated, int (*onClick)(Widget*));
#endif
