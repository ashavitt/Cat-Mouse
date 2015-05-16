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
SDL_Surface* red_surface;
SDL_Surface* logo_surface;

/** initializes SDL */
int init(SDL_Surface**  screen) {
	//initialize SDL
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == -1) {
		fprintf(stderr, "Error: Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	//set up screen
	*screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BPP, SDL_SWSURFACE);
	//check for errors
	if (*screen == NULL) {
		fprintf(stderr, "Error: Unable to set screen: %s\n", SDL_GetError());
		return 1;
	}
	
	//set window title
	SDL_WM_SetCaption("Cat & Mouse", NULL);

	return 0;
}

/** loads all images for the GUI */
int load_images() {
	Uint32 text_colorkey;
	Uint32 buttons_colorkey;
	//Uint32 cat_colorkey;
	//Uint32 mouse_colorkey;
	//Uint32 bricks_colorkey;
	Uint32 tiles_colorkey;
	Uint32 grid_colorkey;
	//Uint32 cheese_colorkey;
	
	texts = SDL_LoadBMP("images/texts.bmp");
	if (texts == NULL) {
		fprintf(stderr, "Error: SDL_LoadBMP failed: %s\n", SDL_GetError());
		return 1;
	}
	buttons = SDL_LoadBMP("images/buttons.bmp");
	if (buttons == NULL) {
		fprintf(stderr, "Error: SDL_LoadBMP failed: %s\n", SDL_GetError());
		return 2;
	}
	cat = SDL_LoadBMP("images/cat.bmp");
	if (cat == NULL) {
		fprintf(stderr, "Error: SDL_LoadBMP failed: %s\n", SDL_GetError());
		return 2;
	}
	mouse = SDL_LoadBMP("images/mouse.bmp");
	if (mouse == NULL) {
		fprintf(stderr, "Error: SDL_LoadBMP failed: %s\n", SDL_GetError());
		return 2;
	}
	bricks = SDL_LoadBMP("images/bricks.bmp");
	if (bricks == NULL) {
		fprintf(stderr, "Error: SDL_LoadBMP failed: %s\n", SDL_GetError());
		return 2;
	}
	cheese = SDL_LoadBMP("images/cheese.bmp");
	if (cheese == NULL) {
		fprintf(stderr, "Error: SDL_LoadBMP failed: %s\n", SDL_GetError());
		return 2;
	}
	tiles = SDL_LoadBMP("images/tiles.bmp");
	if (tiles == NULL) {
		fprintf(stderr, "Error: SDL_LoadBMP failed: %s\n", SDL_GetError());
		return 2;
	}
	grid_surface = SDL_LoadBMP("images/grid.bmp");
	if (grid_surface == NULL) {
		fprintf(stderr, "Error: SDL_LoadBMP failed: %s\n", SDL_GetError());
		return 2;
	}
	logo_surface = SDL_LoadBMP("images/logo.bmp");
	if (logo_surface == NULL) {
		fprintf(stderr, "Error: SDL_LoadBMP failed: %s\n", SDL_GetError());
		return 2;
	}
	
	black_surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0x000000ff);
	if (black_surface == NULL) {
		fprintf(stderr, "Error: SDL_CreateRGBSurface failed: %s\n", SDL_GetError());
		return 3;
	}

	red_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0); // 0xc22b31
	if (red_surface == NULL) {
		fprintf(stderr, "Error: SDL_CreateRGBSurface failed: %s\n", SDL_GetError());
		return 3;
	}
	Uint32 beige_color = SDL_MapRGB(cat->format, 0xD3, 0xB9, 0x9F);//d3b99f
	Uint32 red_color = SDL_MapRGB(red_surface->format, 0xc2, 0x2b, 0x31);
	if (SDL_FillRect(red_surface, NULL, red_color) != 0) { // NULL - paints whole surface
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		return 2;
	}
	// TODO tidy up	
	text_colorkey = SDL_MapRGB(texts->format, 0x66, 0x66, 0x66);
	if (SDL_SetColorKey(texts,SDL_SRCCOLORKEY, text_colorkey) != 0) {
		fprintf(stderr, "Error: SDL_SetColorKey failed: %s\n", SDL_GetError());
		return 6;
	}
	buttons_colorkey = SDL_MapRGB(buttons->format, 0xFF, 0xFF, 0xFF);
	if (SDL_SetColorKey(buttons,SDL_SRCCOLORKEY, buttons_colorkey) != 0) {
		fprintf(stderr, "Error: SDL_SetColorKey failed: %s\n", SDL_GetError());
		return 6;
	}
	if (SDL_SetColorKey(cat,SDL_SRCCOLORKEY, beige_color) != 0) {
		fprintf(stderr, "Error: SDL_SetColorKey failed: %s\n", SDL_GetError());
		return 6;
	}
	if (SDL_SetColorKey(mouse,SDL_SRCCOLORKEY, beige_color) != 0) {
		fprintf(stderr, "Error: SDL_SetColorKey failed: %s\n", SDL_GetError());
		return 6;
	}
	tiles_colorkey = SDL_MapRGB(tiles->format, 0xC2, 0x2B, 0x31);
	if (SDL_SetColorKey(tiles,SDL_SRCCOLORKEY, tiles_colorkey) != 0) {
		fprintf(stderr, "Error: SDL_SetColorKey failed: %s\n", SDL_GetError());
		return 6;
	}
	if (SDL_SetColorKey(cheese,SDL_SRCCOLORKEY, beige_color) != 0) {
		fprintf(stderr, "Error: SDL_SetColorKey failed: %s\n", SDL_GetError());
		return 6;
	}
	if (SDL_SetColorKey(bricks,SDL_SRCCOLORKEY, red_color) != 0) {
		fprintf(stderr, "Error: SDL_SetColorKey failed: %s\n", SDL_GetError());
		return 6;
	}
	grid_colorkey = SDL_MapRGB(grid_surface->format, 0xFF, 0xFF, 0xFF);
	if (SDL_SetColorKey(grid_surface, SDL_SRCCOLORKEY, grid_colorkey) != 0) {
		fprintf(stderr, "Error: SDL_SetColorKey failed: %s\n", SDL_GetError());
		return 6;
	}
	return 0;
}

/** frees all the images and calls SDL_Quit*/
void cleanup() {
	SDL_FreeSurface(texts);
	SDL_FreeSurface(buttons);
	SDL_FreeSurface(cat);
	SDL_FreeSurface(mouse);
	SDL_FreeSurface(tiles);
	SDL_FreeSurface(grid_surface);
	SDL_FreeSurface(bricks);
	SDL_FreeSurface(cheese);
	SDL_FreeSurface(logo_surface);
	SDL_FreeSurface(black_surface);
	SDL_FreeSurface(red_surface);
	//Quit SDL
	SDL_Quit();
}

/** GUI main loop */
int run_gui() {
	//variables declaration
	SDL_Surface* screen;
	SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
	int quit = 0, while_flag = 0;
	game_state state;
	Widget* window = (Widget*) malloc(sizeof(Widget));
	SDL_Rect window_rect = {0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
	Uint32 bg_colorkey;
	if (event == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return 3;
	}
	if (window == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return 3;
	}

	if (init(&screen)) {
		fprintf(stderr, "Error: init failed\n");
		return 1;
	}
	if (load_images()) {
		fprintf(stderr, "Error: load_images failed\n");
		return 2;
	}

	if (widgetFactory(window, 0, WINDOW, window_rect, window_rect, NULL, NULL, NULL, 0, 1, NULL) != 0) {
		fprintf(stderr, "Error: widgetFactory failed\n");
		return ERROR_WIDGET_FACTORY_FAILED;
	}
	
	// set first state as MAIN_MENU
	state.type = MAIN_MENU;
	state.focused = 0;
	state.previous_state = NULL;
	state.world_id = 1;
	state.number = 1;

	//main loop
	while (quit == 0) {
		if (build_ui(window, &state) != 0) {
			fprintf(stderr, "Error: build_ui failed\n");
			quit = ERROR_BUILD_UI_FAILED;
			break;
		}
		//color the window white
		bg_colorkey = SDL_MapRGB(screen->format, 0xDD, 0xC8, 0xB6); 
		if (SDL_FillRect(screen, NULL, bg_colorkey) != 0) {
			fprintf(stderr, "Error: FillRect failed: %s\n", SDL_GetError());
			quit = ERROR_FILL_RECT_FAILED;
			break;
		}
		if (draw_widget(window, screen, window_rect) != 0) {
			fprintf(stderr, "Error: draw_widget failed\n");
			quit = ERROR_DRAW_WIDGET_FAILED;
			break;
		}
		if (SDL_Flip(screen) != 0) {
			fprintf(stderr, "Error: SDL_Flip failed: %s\n", SDL_GetError());
			quit = 25;
			break;
		}
		//move the ai
		if (state.type == IN_GAME) {
			if (state.catormouse == PLAYING) {
				if (check_end_game(state.game) != -1) {
					// set the end game value (tie or win)
					state.catormouse = check_end_game(state.game);
				} else if (ai_move(state.game) == 1) {
					//if the ai played, we delay the game by half a second
					state.catormouse = check_end_game(state.game);
					SDL_Delay(500); //TODO how much?!
				}
			}
		}
		//looping on the events
		while_flag = 0;
		while (SDL_PollEvent(event) && quit == 0 && while_flag == 0) {
			switch (event->type) {
				case SDL_QUIT:
					quit = 1;
					while_flag = 1;
					break;
				case SDL_MOUSEBUTTONUP:
					quit = handle_mouse_click(event, window, &state);
					while_flag = 1;
					break;
				case SDL_KEYUP:
					quit = handle_keyboard(event, window, &state);
					while_flag = 1;
					break;
			}
		}
		SDL_Delay(10);
	}
	free(event);
	freeWidget(window);
	SDL_FreeSurface(screen);
	free_state(state);
	cleanup();
	return quit;
}
