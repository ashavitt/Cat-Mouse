#ifndef __HANDLERS_H__
#define __HANDLERS_H__

#include "SDL.h"
#include "widget.h"
#include "shared.h"

int handle_mouse_click(SDL_Event* event, Widget* window, game_state* state);

#endif
