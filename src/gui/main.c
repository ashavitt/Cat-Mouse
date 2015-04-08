#define __main_c__
#include "main.h"

SDL_Surface* texts;
SDL_Surface* buttons;
SDL_Surface* cat;
SDL_Surface* mouse;
SDL_Surface* tiles;
SDL_Surface* grid;

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
	Uint32 tiles_colorkey;
	Uint32 grid_colorkey;
	texts = SDL_LoadBMP("images/texts.bmp");
	buttons = SDL_LoadBMP("images/buttons.bmp");
	cat = SDL_LoadBMP("images/cat.bmp");
	mouse = SDL_LoadBMP("images/mouse.bmp");
	tiles = SDL_LoadBMP("images/tiles.bmp");
	grid = SDL_LoadBMP("images/grid.bmp");
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
	if (tiles == NULL) {
		return -2;
	}
	if (grid == NULL) {
		return -2;
	}
	text_colorkey = SDL_MapRGB(texts->format, 0xFF, 0xFF, 0xFF);
	SDL_SetColorKey(texts,SDL_SRCCOLORKEY, text_colorkey);
	buttons_colorkey = SDL_MapRGB(buttons->format, 0xFF, 0xFF, 0xFF);
	SDL_SetColorKey(buttons,SDL_SRCCOLORKEY, buttons_colorkey);
	cat_colorkey = SDL_MapRGB(cat->format, 0xFF, 0x00, 0x00);
	SDL_SetColorKey(cat,SDL_SRCCOLORKEY, cat_colorkey);
	mouse_colorkey = SDL_MapRGB(mouse->format, 0xFF, 0x00, 0x00);
	SDL_SetColorKey(mouse,SDL_SRCCOLORKEY, mouse_colorkey);
	tiles_colorkey = SDL_MapRGB(tiles->format, 0x00, 0xFF, 0x00);
	SDL_SetColorKey(tiles,SDL_SRCCOLORKEY, tiles_colorkey);
	grid_colorkey = SDL_MapRGB(grid->format, 0xFF, 0xFF, 0xFF);
	SDL_SetColorKey(grid,SDL_SRCCOLORKEY, grid_colorkey);
	return 0;
}


void cleanup() {
	SDL_FreeSurface(texts);
	SDL_FreeSurface(buttons);
	SDL_FreeSurface(cat);
	SDL_FreeSurface(mouse);
	SDL_FreeSurface(tiles);
	SDL_FreeSurface(grid);
	//Quit SDL
	SDL_Quit();
}


int run_gui() {
	//variables declaration
	SDL_Surface* screen; // = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	texts = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	buttons = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	cat = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	mouse = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	tiles = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	grid = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
	int quit = -1;
	game_state state;
	Widget* window = (Widget*) malloc(sizeof(Widget));
	SDL_Rect window_rect = {0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
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

	//main loop
	while (quit == -1) {
		if (build_ui(window, &state) != 0) {
			quit = ERROR_BUILD_UI_FAILED;
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
		//looping on the events
		while (SDL_PollEvent(event)) {
			switch (event->type) {
				case SDL_QUIT:
					quit = 0;
					break;
				case SDL_MOUSEBUTTONUP:
					handle_mouse_click(event, window, state);
					break;
			}
		}

		SDL_Delay(100);
	}
	cleanup();
	return quit;
}
