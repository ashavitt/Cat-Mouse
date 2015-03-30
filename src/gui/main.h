#include "SDL.h"
#include "../core/board.h"
#include "../main/ListUtils.h"

//Screen attributes
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define BPP 32 //bits per pixel


typedef enum  {
	MAIN_MENU, CHOOSE_PLAYER, CHOOSE_SKILL, IN_GAME, SELECT_GAME, GAME_EDIT, ERROR_DIALOG
} state_type;

typedef struct  {
	state_type type;
	int focused; 
	// focused 0-99 is regular buttons
	// if focused>99 then focused = 1xy in world editing
	int number; //skill_level or world_id
	Game* game; // for world editing
} game_state;

int run_gui();