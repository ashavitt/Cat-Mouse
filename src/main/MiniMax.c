#include <stdio.h>
#include <stdlib.h>
#include "MiniMax.h"

typedef struct MiniMaxResult MiniMaxResult;

/** maxMinResult
  * params: MiniMaxResult array, its length, isMaxPlayer
  * returns the best MiniMaxResult for the player:
  * if(isMaxPlayer) maximal value and otherwise minimal value.
  */
MiniMaxResult getBetter(MiniMaxResult best, MiniMaxResult result, int isMaxPlayer) {
	if(best.index < 0) {
		return result;
	} else {
		if (isMaxPlayer) { //this is instructive
			if (best.value < result.value){
				return result;
			}
		} else {
			if (best.value > result.value){
				return result;
			}
		}
	}
	return best;
}

int getBetterInt(int best, int result, int isMaxPlayer) {
	if (isMaxPlayer) { //this is instructive
		if (best < result){
			return result;
		}
	} else {
		if (best > result){
			return result;
		}
	}
	return best;
}

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
MiniMaxResult getBestChildAlphaBeta(void* state,
		unsigned int maxDepth,
		ListRef (*getChildren) (void*),
		FreeFunc freeState,
		int (*evaluate) (void*),
		int isMaxPlayer,
		int alpha,
		int beta) {

	MiniMaxResult best;
	best.index = -1;
	if (isMaxPlayer) {
		best.value = MIN_EVALUATION;
	} else {
		best.value = MAX_EVALUATION;
	}
	ListRef originalChildren = (*getChildren) (state); //load state's children
	ListRef children = originalChildren; //save ALL the state's children for later

	int myeval = evaluate(state);
	if (myeval == MAX_EVALUATION || myeval == MIN_EVALUATION) {
		children = NULL;
		best.value = myeval;
	}

	int index = 0; //counting how many children were inserted to childrenResults by now
	//Assuming maxDepth>0
	while (children != NULL && !isEmpty(children)) { //we run over the list of children
		if (maxDepth == 1) { //then this is the last recursion level
			MiniMaxResult bottomChildResult;
			bottomChildResult.index = index;

			int eval = (*evaluate) (headData(children)); //evaluation of the child
			//overflow check, Probably useless
			eval = eval > MAX_EVALUATION ? MAX_EVALUATION : eval;
			eval = eval < MIN_EVALUATION ? MIN_EVALUATION : eval;
			
			bottomChildResult.value = eval;
			best = getBetter(best,bottomChildResult,isMaxPlayer);
		} else {
			MiniMaxResult bestChild = getBestChildAlphaBeta(headData(children),
				maxDepth - 1, getChildren, freeState, evaluate, !isMaxPlayer,
				alpha, beta);
			bestChild.index = index;
			best = getBetter(best,bestChild,isMaxPlayer);
			
			if (isMaxPlayer) {
				alpha = getBetterInt(alpha, best.value, isMaxPlayer);
			} else {
				beta = getBetterInt(beta, best.value, isMaxPlayer);
			}

			if (beta <= alpha) {
				break;
			}
		}
		children = tail(children); //cutting the head
		index++;
	} // now index is the length of originalChildren

	//Memory deallocation
	//no need to deallocate childrenResults - automatically deallocates when function finishes
	destroyList(originalChildren, freeState);

	return best;
}

MiniMaxResult getBestChild(void* state,
		unsigned int maxDepth,
		ListRef (*getChildren) (void*),
		FreeFunc freeState,
		int (*evaluate) (void*),
		int isMaxPlayer) {

	return getBestChildAlphaBeta(state, maxDepth, getChildren, freeState,
		 evaluate, isMaxPlayer, MIN_EVALUATION, MAX_EVALUATION);
}
