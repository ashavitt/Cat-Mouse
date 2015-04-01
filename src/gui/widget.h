#ifndef __WIDGET__
#define __WIDGET__

// includes
#include "main.h"
#include "../main/ListUtils.h"

typedef unsigned char byte;

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
#endif