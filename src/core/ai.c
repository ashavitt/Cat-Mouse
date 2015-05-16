#include "ai.h"

/** calls the MiniMax algorithm and returns the direction of the best move */
int get_best_move(Game* game, int num_steps) {
	int is_max_player = (game->player == CAT) ? 1 : 0;
	int result_index = getBestChild(game, num_steps, getChildren, free, evaluateGame, is_max_player).index;

	//fix the index returned by the minimax (ignoring invalid directions)
	int i;
	for (i = 0; result_index >= 0; i++) {
		if (dir_is_valid(game,i)) {
			result_index--;
		}
	}
	// in case minimax failed we return index 0
	if (i <= 0) {
		//TODO print error message?
		i = 1;
	}
	return --i;
}

/** checks if AI should play and play the move */
int ai_move(Game* game) {
	int move = -1;
	if (game->player == CAT && game->num_steps_cat != 0) {
		move = get_best_move(game, game->num_steps_cat);
	} else if (game->player == MOUSE && game->num_steps_mouse != 0) {
		move = get_best_move(game, game->num_steps_mouse);
	}
	//if it isn't the computer turn
	if (move == -1) {
		return 0;
	}
	//update the turns left
	game->turns--;
	//commit the computer move
	move_obj(game, game->player, move);
	//swap the players
	game->player = (game->player == CAT) ? MOUSE : CAT;
	return 1;
}
