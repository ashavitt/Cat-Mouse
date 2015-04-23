#ifndef __SHARED_H__
#define __SHARED_H__
#include "SDL.h"
#include "../core/board.h"
typedef enum  {
	MAIN_MENU, CHOOSE_PLAYER, CHOOSE_SKILL, IN_GAME, LOAD_GAME, SAVE_GAME, EDIT_GAME, GAME_EDIT, ERROR_DIALOG
} state_type;

typedef struct game_state_struct {
	state_type type;
	int focused; 
	// focused 0-99 is regular buttons
	// if focused>99 then focused = 1xy in world editing
	int number; //skill_level or world_id
	Game* game; // for world editing
	struct game_state_struct* previous_state;
	char catormouse; // CAT 'C' or MOUSE 'M'
} game_state;

#define PAUSED -1
#define PLAYING -2

#define MAIN_MENU_OBJS 5
#define CHOOSE_PLAYER_OBJS 3
#define CHOOSE_SKILL_OBJS 3
#define GAME_SCREEN_OBJS 7
#define CREATE_GAME_OBJS 9
#define LOAD_GAME_OBJS 3
#define ERROR_MESSAGE_OBJS 1

#ifndef __main_c__
extern byte main_menu_ids[MAIN_MENU_OBJS];
extern byte choose_player_ids[CHOOSE_PLAYER_OBJS];
extern byte choose_skill_ids[CHOOSE_SKILL_OBJS];
extern byte game_screen_ids[GAME_SCREEN_OBJS];
extern byte create_game_ids[CREATE_GAME_OBJS];
extern byte load_game_ids[LOAD_GAME_OBJS];
extern byte error_message_ids[ERROR_MESSAGE_OBJS];

extern SDL_Surface* texts;
extern SDL_Surface* buttons;
extern SDL_Surface* cat;
extern SDL_Surface* cheese;
extern SDL_Surface* mouse;
extern SDL_Surface* bricks;
extern SDL_Surface* tiles;
extern SDL_Surface* grid_surface;
extern SDL_Surface* black_surface;
#endif
#endif
