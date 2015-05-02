#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include <string.h>
#include "SDL.h"
#include "widget.h"
#include "shared.h"
#include "../main/game_settings.h"

//typedef int (*onclick)(Widget*, game_state*);

onclick quit_action;

onclick back_action;

onclick pause_resume_action;

onclick new_game_action;

onclick choose_action;

onclick do_nothing_action;

onclick arrow_action;

onclick restart_game_action;

onclick grid_mouse_action;

int in_game_action(game_state* state, SDLKey key);
#endif
