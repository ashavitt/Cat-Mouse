#include "main.h"

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

int load_images(SDL_Surface* texts, SDL_Surface* buttons) {
	texts = SDL_LoadBMP("images/texts.bmp");
	buttons = SDL_LoadBMP("images/buttons.bmp");
	if (texts == NULL) {
		return -1;
	}
	if (buttons == NULL) {
		return -2;
	}
	return 0;
}


void cleanup() {
	//Quit SDL
	SDL_Quit();
	exit(0);
}


int run_gui() {
	//variables declaration
	SDL_Surface* screen = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	SDL_Surface* texts = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	SDL_Surface* buttons = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	SDL_Event* event = (SDL_Event*) malloc(sizeof(SDL_Event));
	int quit = 0;

	if (init(screen)) {
		return -1;
	}
	if (load_images(texts, buttons)) {
		return -2;
	}

	//main loop
	while (!quit) {
		//looping on the events
		while (SDL_PollEvent(event)) {
			switch (event->type) {
				case SDL_QUIT:
					quit = 1;
					break;
			}
		}

		SDL_Delay(10);
	}
	cleanup();
	return 0;
}