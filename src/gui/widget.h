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
	byte disabled; // for buttons

	int (*onclick)(struct widget*, game_state*);
} Widget;

typedef int (onclick)(Widget*, game_state*);

Widget* find_widget_by_id(Widget* root, int id);

int add_rect(SDL_Rect* rect1, SDL_Rect* rect2);

void freeWidget(void* data);

SDL_Rect get_center(SDL_Rect parent_dims, SDL_Rect size);

Widget* new_graphic(int id,  SDL_Rect dims, SDL_Rect pos, SDL_Surface* imgsrc, Widget* parent);

Widget* new_button(int id,  SDL_Rect dims, SDL_Rect pos, Widget* parent, onclick onClick);

Widget* new_panel(int id, SDL_Rect pos, Widget* parent);

int draw_widget(Widget* widget, SDL_Surface* window, SDL_Rect abs_pos);

int widgetFactory(Widget* widget, int id, widget_type type, SDL_Rect dims, SDL_Rect pos, SDL_Surface* imgsrc,
	Widget* parent, ListRef children, byte focused, byte updated, onclick onClick);
	
	
/** General **/
#define BACK_B 1
#define QUIT_B 6
#define DONE_B 12
#define UNFOCUSABLE 100

/** Main Menu **/
#define NEW_GAME_B 2
#define LOAD_GAME_B 3
#define CREATE_GAME_B 4
#define EDIT_GAME_B 5

/** New Game **/
#define HUMAN_B 7
#define MACHINE_B 8

/** Choose Skill Level **/
#define LEVEL_CHOOSER 9
#define LEVEL_UP_B 10
#define LEVEL_DN_B 11

/** Game Screen **/
#define PAUSE_B 13
#define RECONF_MOUSE_B 14
#define RECONF_CAT_B 15
#define RESTART_GAME_B 16
#define GOTO_MAIN_MENU_B 17

/** Error Message **/
//back button

/** World Building **/
/* Load Game / Save World / Edit Game */
#define WORLD_CHOOSER 18
#define WORLD_UP_B LEVEL_UP_B
#define WORLD_DN_B LEVEL_DN_B

/* World Editing */
#define SAVE_WORLD_B 19
#define PLACE_MOUSE_B 20
#define PLACE_CAT_B 21
#define PLACE_CHEESE_B 22
#define PLACE_WALL_B 23
#define PLACE_SPACE_B 24

/** ETC **/
#define GRID_B 25

#endif
