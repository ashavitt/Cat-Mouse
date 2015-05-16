#ifndef __SHARED_H__
#define __SHARED_H__
#include "SDL.h"
#include "../core/board.h"

// All types of game states. Names are instructive
typedef enum  {
	MAIN_MENU, CHOOSE_PLAYER, CHOOSE_SKILL, IN_GAME, LOAD_GAME, SAVE_GAME, EDIT_GAME, GAME_EDIT, ERROR_DIALOG
} state_type;

/** Game State Definition **/
typedef struct game_state_struct {
	state_type type;
	int focused; // in game_edit: focsued = xy, for x,y coordinates of editing cursor
		     // else: number of focused object, counted between focusable objects
	int number; // Holding the current number to display in skill_level selection or world selection
	int world_id; // Holding the world_id for current play, edit etc.
	Game* game; // Pointer to a game - either for playing or editing
	struct game_state_struct* previous_state; // Pointer to previous state. Last state behaves as a stack
	char catormouse; // In player or skill selection: CAT 'C' or MOUSE 'M'
			 // In gameplay: PLAYING (defined below), PAUSED (defined below), CAT_WIN, MOUSE_WIN, TIE
} game_state;

#define PAUSED -2
#define PLAYING -1

// When opening a world selection (LOAD_GAME, EDIT_GAME), which world appears selected at first -
#define DEFAULT_WORLD_INDEX 1
// Same for skill level in CHOOSE_SKILL
#define DEFAULT_SKILL_LEVEL (5)
// count of focusable widgets in each state
#define MAIN_MENU_OBJS 5
#define CHOOSE_PLAYER_OBJS 3
#define CHOOSE_SKILL_OBJS 3
#define GAME_SCREEN_OBJS 7
#define CREATE_GAME_OBJS 9
#define LOAD_GAME_OBJS 3
#define ERROR_MESSAGE_OBJS 1

/** Game Scheme **/
#define GRID_W 634
#define GRID_H 634

// Used to get x,y coordinates saved as xy number.
#define GET_X(n) (((n) % 100) / 10)
#define GET_Y(n) ((n) % 10)

#ifndef __main_c__
// in case we are compiling main.c, we don't need to define externs
extern byte main_menu_ids[MAIN_MENU_OBJS];
extern byte choose_player_ids[CHOOSE_PLAYER_OBJS];
extern byte choose_skill_ids[CHOOSE_SKILL_OBJS];
extern byte game_screen_ids[GAME_SCREEN_OBJS];
extern byte create_game_ids[CREATE_GAME_OBJS];
extern byte load_game_ids[LOAD_GAME_OBJS];
extern byte error_message_ids[ERROR_MESSAGE_OBJS];

extern SDL_Surface *texts, *buttons, *cat, *mouse, *cheese, *bricks, *tiles, *grid_surface, *black_surface, *red_surface, *logo_surface;
#endif

/* Headers for general shared functions from shared.c */
void free_state(game_state state);
void free_prev_states(game_state* state);

#endif
