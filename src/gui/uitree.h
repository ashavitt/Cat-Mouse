#ifndef __UITREE__
#define __UITREE__

/* Includes */
#include "shared.h"
#include "widget.h"
#include "../error/error.h"

int build_ui(Widget*, game_state*);

/*** Sprites ***/

/** Buttons **/
//WL - Wide long, NL - Narrow long, WS - (very) Wide short, NS - Narrow short
#define WL_BUTTON_W 282
#define WL_BUTTON_H 57
#define NL_BUTTON_W 141
#define NL_BUTTON_H 57
#define WS_BUTTON_W WL_BUTTON_W
#define WS_BUTTON_H 25

/** Text **/
#define WL_T_W 264
#define WL_T_H 51
#define NL_T_W 132
#define NL_T_H 51

/* Titles */
#define TITLES_T_X_START 528
#define TITLES_T_Y_START 271

/* Numbers */
// Small Numbers (starts with braces () )
#define S_NUM_T_X_START 0
#define S_NUM_T_Y_START 102
#define S_NUM_T_W 10
#define S_NUM_T_H 16
// Medium Numbers (starts with 0)
#define M_NUM_T_X_START 0
#define M_NUM_T_Y_START 577
#define M_NUM_T_W 11
#define M_NUM_T_H 15
// Large Numbers (starts with 0)
#define L_NUM_T_X_START 0
#define L_NUM_T_Y_START 592
#define L_NUM_T_W 14
#define L_NUM_T_H 20

/* Main Menu */
// 5 buttons, 3 in a row
#define MAIN_MENU_T_X_START 0
#define MAIN_MENU_T_Y_START 118

/* Messages */

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
