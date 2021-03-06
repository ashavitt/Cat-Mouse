#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include <string.h>
#include "SDL.h"
#include "widget.h"
#include "shared.h"
#include "../main/game_settings.h"
#include "../io/file_io.h"

/* Used for game invalidity error messages */
#define CAT_IS_MISSING_IND 0
#define MOUSE_IS_MISSING_IND 2
#define CHEESE_IS_MISSING_IND 1

/** onclick is defined by
  *   typedef int (*onclick)(Widget*, game_state*);
  * These actions are mostly used for widget clicking
  * and simulated widget clicking when a keyboard press occurs.
  */
  
onclick quit_action;
onclick back_action;
onclick pause_resume_action;
onclick new_game_action;
onclick load_game_action;
onclick save_game_action;
onclick choose_action;
onclick do_nothing_action;
onclick arrow_action;
onclick restart_game_action;
onclick grid_mouse_action;
onclick grid_edit_mouse_action;
onclick goto_main_menu_action;
onclick restart_game_action;
onclick reconf_action;
onclick edit_game_action;
onclick create_game_action;
onclick place_wall_action;
onclick place_empty_action;
onclick place_cat_action;
onclick place_mouse_action;
onclick place_cheese_action;

// other non-standard actions
int in_game_action(game_state* state, SDLKey key);
int game_editing_action(game_state* state, SDLKey key);

#endif
