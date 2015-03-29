#include "widget.h"

int draw_widget(Widget* widget) {
	if (widget == screen) {

	}
	if (SDL_BlitSurface(widget->imgsrc, &(widget->dims), window, &(widget->pos)) != 0) {
		SDL_FreeSurface(img);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return ERROR_BLIT_FAIL; //TODO
	}
}