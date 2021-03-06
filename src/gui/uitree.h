#ifndef __UITREE__
#define __UITREE__

/* Includes */
#include "shared.h"
#include "widget.h"
#include "../error/error.h"
#include "actions.h"
#include <math.h>

int build_ui(Widget*, game_state*);

Widget* number_to_graphic(int id, SDL_Rect main_pos, int number, int size, Widget* parent);

/*** General Defines ***/
#define TEXT_SIZE_SMALL 0
#define TEXT_SIZE_MEDIUM 1
#define TEXT_SIZE_LARGE 2


/*** Sprites ***/

/** Buttons **/
//WL - Wide long, NL - Narrow long, WS - (very) Wide short, NS - Narrow short
#define WL_BUTTON_W 282
#define WL_BUTTON_H 57
#define NL_BUTTON_W 141
#define NL_BUTTON_H 57
#define WS_BUTTON_W WL_BUTTON_W
#define WS_BUTTON_H 25

#define UP_ARROW_B_X 247
#define UP_ARROW_B_Y 171
#define UP_ARROW_B_W 35
#define UP_ARROW_B_H 29

/** Text **/
#define WL_T_W 264
#define WL_T_H 51
#define NL_T_W 132
#define NL_T_H 51
#define NS_T_W 219
#define NS_T_H 17

/* Titles */
#define TITLES_T_X_START 0
#define TITLES_T_Y_START 322

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

/* Game Titles */
#define G_TITLES_X_START 0
#define G_TITLES_Y_START 612
#define G_TITLES_H 17
#define G_TITLES_W 185

/* Main Menu */
// 5 buttons, 3 in a row
#define MAIN_MENU_T_X_START 0
#define MAIN_MENU_T_Y_START 118

/* Messages */
#define NS_B_MESSAGE_X_START 185
#define NS_B_MESSAGE_Y_START 612

/* Logo */
#define LOGO_W 600
#define LOGO_H 156

/* Error Messages */
#define ERR_T_X_START 264
#define ERR_T_Y_START 373
#define ERR_T_W 264
#define ERR_T_H 51

#endif
