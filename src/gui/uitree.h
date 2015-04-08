#ifndef __UITREE__
#define __UITREE__

/* Includes */
#include "shared.h"
#include "widget.h"
#include "../error/error.h"

int build_ui(Widget*, game_state*);

/** General **/
#define BACK_B 1
#define QUIT_B 6
#define DONE_B 12

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
