#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include "SDL.h"
#include "widget.h"
#include "shared.h"

//typedef int (*onclick)(Widget*, game_state*);

onclick quit_action(Widget*, game_state*);

onclick back_action(Widget*, game_state*);

#endif
