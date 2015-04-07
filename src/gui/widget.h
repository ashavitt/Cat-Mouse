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

void freeWidget(void* data);

Widget* new_graphic(int id,  SDL_Rect dims, SDL_Rect pos, SDL_Surface* imgsrc, Widget* parent);

int draw_widget(Widget* widget, SDL_Surface* window, SDL_Rect abs_pos);
#endif
