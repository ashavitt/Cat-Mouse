#ifndef MINIMAX_H_
#define MINIMAX_H_

#include "ListUtils.h"

/**
 * The maximum allowed value to be returned by any evaluation function.
 */
#define MAX_EVALUATION 100000

/**
 * The minimum allowed value to be returned by any evaluation function.
 */
#define MIN_EVALUATION -100000

/**
 * Represents the result of a MiniMax computation.
 */
struct MiniMaxResult {
	/**
	 * The index of the best child.
	 */
	int index;

	/**
	 * The value of the best child (according to the evaluation function).
	 */
	int value;
};

/**
 * Returns the best child of the given state, resulting from the MiniMax computation up to maxDepth.
 * The getChildren function will be used to get each state's children.
 * The freeState function will be used to deallocate any lists returned by GetChildren.
 * The evaluate function will be used to evaluate the state score.
 * The player who's turn it is to play at the given state is assumed to be the MAX player if and only if isMaxPlayer is non-zero,
 * otherwise, it is assumed to be the MIN player.
 *
 * Will NOT crash in case one of the given functions fails but result will be undefined.
 */
struct MiniMaxResult getBestChild(
		void* state,
		unsigned int maxDepth,
		ListRef (*getChildren) (void* state),
		FreeFunc freeState,
		int (*evaluate) (void* state),
		int isMaxPlayer);

#endif /* MINIMAX_H_ */
