#define __main_c__
#include "main.h"

const byte main_menu_ids[MAIN_MENU_OBJS] = {NEW_GAME_B, LOAD_GAME_B, CREATE_GAME_B, EDIT_GAME_B, QUIT_B};
const byte choose_player_ids[CHOOSE_PLAYER_OBJS] = {HUMAN_B, MACHINE_B, BACK_B};
const byte choose_skill_ids[CHOOSE_SKILL_OBJS] = {LEVEL_CHOOSER, DONE_B, BACK_B};
const byte game_screen_ids[GAME_SCREEN_OBJS] = {PAUSE_B, RECONF_MOUSE_B, RECONF_CAT_B, RESTART_GAME_B, GOTO_MAIN_MENU_B, QUIT_B, GRID_B};
const byte create_game_ids[CREATE_GAME_OBJS] = {GRID_B, PLACE_MOUSE_B, PLACE_CAT_B, PLACE_CHEESE_B, PLACE_WALL_B, PLACE_SPACE_B, SAVE_WORLD_B, GOTO_MAIN_MENU_B, QUIT_B};
const byte load_game_ids[LOAD_GAME_OBJS] = {WORLD_CHOOSER, DONE_B, BACK_B};
const byte error_message_ids[ERROR_MESSAGE_OBJS] = {BACK_B};

SDL_Surface* texts;
SDL_Surface* buttons;
SDL_Surface* cat;
SDL_Surface* mouse;
SDL_Surface* cheese;
SDL_Surface* bricks;
SDL_Surface* tiles;
SDL_Surface* grid_surface;
SDL_Surface* black_surface;

int init(SDL_Surface**  screen) {
	//initialize SDL
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == -1) {
		return -1;
	}

	//set up screen
	*screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BPP, SDL_SWSURFACE);
	//check for errors
	if (*screen == NULL) {
		return -1;
	}

	//set window title
	SDL_WM_SetCaption("Cat & Mouse", NULL);

	return 0;
}

int load_images() {
	Uint32 text_colorkey;
	Uint32 buttons_colorkey;
	Uint32 cat_colorkey;
	Uint32 mouse_colorkey;
	Uint32 bricks_colorkey;
	Uint32 tiles_colorkey;
	Uint32 grid_colorkey;
	Uint32 cheese_colorkey;
	texts = SDL_LoadBMP("images/texts.bmp");
	buttons = SDL_LoadBMP("images/buttons.bmp");
	cat = SDL_LoadBMP("images/cat.bmp");
	mouse = SDL_LoadBMP("images/mouse.bmp");
	bricks = SDL_LoadBMP("images/bricks.bmp");
	cheese = SDL_LoadBMP("images/cheese.bmp");
	tiles = SDL_LoadBMP("images/tiles.bmp");
	grid_surface = SDL_LoadBMP("images/grid.bmp");
	
	black_surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0x000000ff);
	if (texts == NULL) {
		return -1;
	}
	if (buttons == NULL) {
		return -2;
	}
	if (cat == NULL) {
		return -2;
	}
	if (mouse == NULL) {
		return -2;
	}
	if (bricks == NULL) {
		return -2;
	}
	if (tiles == NULL) {
		return -2;
	}
	if (cheese == NULL) {
		return -2;
	}
	if (grid_surface == NULL) {
		return -2;
	}
	if (black_surface == NULL) {
		return -2;
	}
	
	//text_colorkey = SDL_MapRGB(texts->format, 0xFF, 0xFF, 0xFF);
  text_colorkey = SDL_MapRGB(texts->format, 0x66, 0x66, 0x66);
	SDL_SetColorKey(texts,SDL_SRCCOLORKEY, text_colorkey);
	buttons_colorkey = SDL_MapRGB(buttons->format, 0xFF, 0xFF, 0xFF);
	SDL_SetColorKey(buttons,SDL_SRCCOLORKEY, buttons_colorkey);
	cat_colorkey = SDL_MapRGB(cat->format, 0xFF, 0xFF, 0xFF);
	SDL_SetColorKey(cat,SDL_SRCCOLORKEY, cat_colorkey);
	mouse_colorkey = SDL_MapRGB(mouse->format, 0xFF, 0xFF, 0xFF);
	SDL_SetColorKey(mouse,SDL_SRCCOLORKEY, mouse_colorkey);
	tiles_colorkey = SDL_MapRGB(tiles->format, 0xC2, 0x2B, 0x31);
	SDL_SetColorKey(tiles,SDL_SRCCOLORKEY, tiles_colorkey);
	cheese_colorkey = SDL_MapRGB(cheese->format, 0xFF, 0xFF, 0xFF);
	SDL_SetColorKey(cheese,SDL_SRCCOLORKEY, cheese_colorkey);
	bricks_colorkey = SDL_MapRGB(bricks->format, 0xFF, 0xFF, 0xFF);
	SDL_SetColorKey(bricks,SDL_SRCCOLORKEY, bricks_colorkey);
	grid_colorkey = SDL_MapRGB(grid_surface->format, 0xFF, 0xFF, 0xFF);
	SDL_SetColorKey(grid_surface,SDL_SRCCOLORKEY, grid_colorkey);
	return 0;
}

void free_state(game_state state) {
	if (state.previous_state != NULL) {
		free_state(*(state.previous_state));
	}
	free(state.previous_state);
	return;
}


void cleanup() {
	SDL_FreeSurface(texts);
	SDL_FreeSurface(buttons);
	SDL_FreeSurface(cat);
	SDL_FreeSurface(mouse);
	SDL_FreeSurface(tiles);
	SDL_FreeSurface(grid_surface);
	SDL_FreeSurface(bricks);
	//Quit SDL
	SDL_Quit();
}


int run_gui() {
	//variables declaration
	SDL_Surface* screen; // = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
	int quit = 0;
	game_state state;
	Widget* window = (Widget*) malloc(sizeof(Widget));
	SDL_Rect window_rect = {0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
	Uint32 white_colorkey;
	//TODO check mallocs

	if (init(&screen)) {
		return -1;
	}
	if (load_images()) {
		return -2;
	}

	if (widgetFactory(window, 0, WINDOW, window_rect, window_rect, NULL, NULL, NULL, 0, 1, NULL) != 0) {
		return ERROR_WIDGET_FACTORY_FAILED;
	}
	
	state.type = MAIN_MENU;
	state.focused = 0;
	state.previous_state = NULL;
	state.game = load_world(1);

	//main loop
	while (quit == 0) {
		if (build_ui(window, &state) != 0) {
			quit = ERROR_BUILD_UI_FAILED;
			break;
		}
		//color the window white
		white_colorkey = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
		if (SDL_FillRect(screen, NULL, white_colorkey) != 0) {
			printf("Error: %s\n", SDL_GetError());
			quit = ERROR_FILL_RECT_FAILED;
			break;
		}
		if (draw_widget(window, screen, window_rect) != 0) {
			quit = ERROR_DRAW_WIDGET_FAILED;
			break;
		}
		if (SDL_Flip(screen) != 0) {
			quit = 25; // TODO
			break;
		}
		//move the ai
		if (state.type == IN_GAME) {
			if (state.catormouse == PLAYING) {
				if (check_end_game(state.game) != -1) {
					state.catormouse = check_end_game(state.game);
				} else if (ai_move(state.game) == 1) {
					//if the ai played, we delay the game by half a second
					state.catormouse = check_end_game(state.game);
					SDL_Delay(500);
				}
			}
		}
		//looping on the events
		if (SDL_PollEvent(event) && quit == 0) {
			switch (event->type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_MOUSEBUTTONUP:
					quit = handle_mouse_click(event, window, &state);
					break;
				case SDL_KEYUP:
					quit = handle_keyboard(event, window, &state);
					break;
			}
		}

		SDL_Delay(10);
	}
	free(event);
	freeWidget(window);
	SDL_FreeSurface(screen);
	game_free(state.game);
	free_state(state);
	cleanup();
	return quit;
}
