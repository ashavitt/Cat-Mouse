#define __main_c__
#include "main.h"


SDL_Surface* texts;
SDL_Surface* buttons;
SDL_Surface* cat;
SDL_Surface* mouse;
SDL_Surface* tiles;
SDL_Surface* grid;

int init(SDL_Surface* screen) {
	//initialize SDL
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == -1) {
		return -1;
	}

	//set up screen
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BPP, SDL_SWSURFACE);
	//check for errors
	if (screen == NULL) {
		return -1;
	}

	//set window title
	SDL_WM_SetCaption("Cat & Mouse", NULL);

	return 0;
}

int load_images() {
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
	return 0;
}


void cleanup() {
	//Quit SDL
	SDL_Quit();
}


int run_gui() {
	//variables declaration
	SDL_Surface* screen = (SDL_Surface*) malloc(sizeof(SDL_Surface));
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

	if (init(screen)) {
		return -1;
	}
	if (load_images()) {
		return -2;
	}

	if (widgetFactory(window, 0, WINDOW, window_rect, window_rect, NULL, NULL, NULL, 0, 1) != 0) {
		return ERROR_WIDGET_FACTORY_FAILED;
	}
	
	state.type = MAIN_MENU;

	//main loop
	while (quit != -1) {
		if (build_ui(window, &state) != 0) {
			quit = ERROR_BUILD_UI_FAILED;
			break;
		}
		//looping on the events
		while (SDL_PollEvent(event)) {
			switch (event->type) {
				case SDL_QUIT:
					quit = 0;
					break;
			}
		}

		SDL_Delay(10);
	}
	cleanup();
	return quit;
}
