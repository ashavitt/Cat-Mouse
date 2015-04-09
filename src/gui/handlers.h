#ifndef __HANDLERS_H__
#define __HANDLERS_H__

#include "SDL.h"
#include "widget.h"
#include "shared.h"

byte get_obj_count(state_type type);

byte[] get_objs_arr(state_type type);

int handle_mouse_click(SDL_Event* event, Widget* window, game_state* state);

int handle_keyboard(SDL_Event* event, Widget* window, game_state* state);
#endif
