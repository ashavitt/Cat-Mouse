#include "SDL.h"

//Screen attributes
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define BPP 32 //bits per pixel


typedef enum  {
	MAIN_MENU
} state_type;

typedef struct  {
	state_type type;
	union state {
		main_menu_state;
	};
} game_state;



int run_gui();