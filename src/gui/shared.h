#ifndef __SHARED_H__
#define __SHARED_H__
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

#ifndef __main_c__
extern SDL_Surface* texts;
extern SDL_Surface* buttons;
extern SDL_Surface* cat;
extern SDL_Surface* mouse;
extern SDL_Surface* tiles;
extern SDL_Surface* grid;
#endif
#endif
