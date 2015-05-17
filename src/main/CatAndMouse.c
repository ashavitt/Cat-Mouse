#include "CatAndMouse.h"

/**
                Y.                      _   
                YiL                   .```.  
                Yii;                .; .;;`.    
                YY;ii._           .;`.;;;; :   So cat
                iiYYYYYYiiiii;;;;i` ;;::;;;;    
            _.;YYYYYYiiiiiiYYYii  .;;.   ;;; 
         .YYYYYYYYYYiiYYYYYYYYYYYYii;`  ;;;;    
       .YYYYYYY$$YYiiYY$$$$iiiYYYYYY;.ii;`..   
      :YYY$!.  TYiiYY$$$$$YYYYYYYiiYYYYiYYii.    
      Y$MM$:   :YYYYYY$!"``"4YYYYYiiiYYYYiiYY.    Much mouse
   `. :MM$$b.,dYY$$Yii" :'   :YYYYllYiiYYYiYY    
_.._ :`4MM$!YYYYYYYYYii,.__.diii$$YYYYYYYYYYY
.,._ $b`P`     "4$$$$$iiiiiiii$$$$YY$$$$$$YiY;
   `,.`$:       :$$$$$$$$$YYYYY$$$$$$$$$YYiiYYL
    "`;$$.    .;PPb$`.,.``T$$YY$$$$YYYYYYiiiYYU:  
    ;$P$;;: ;;;;i$y$"!Y$$$b;$$$Y$YY$$YYYiiiYYiYY 
    $Fi$$ .. ``:iii.`-":YYYYY$$YY$$$$$YYYiiYiYYY    
    :Y$$rb ````  `_..;;i;YYY$YY$$$$$$$YYYYYYYiYY:    
     :$$$$$i;;iiiiidYYYYYYYYYY$$$$$$YYYYYYYiiYYYY. 
      `$$$$$$$YYYYYYYYYYYYY$$$$$$YYYYYYYYiiiYYYYYY    
      .i!$$$$$$YYYYYYYYY$$$$$$YYY$$YYiiiiiiYYYYYYY    
     :YYiii$$$$$$$YYYYYYY$$$$YY$$$$YYiiiiiYYYYYYi'    
**/

int main(int argc, char* argv[]) {
	int eval, quit = 1;
	// check if console mode
	if (argc == 3) {
		if (!strcmp(argv[1], "-console")) {
			Game* game = game_malloc();
			while (quit) {
				if (load_game(game, stdin) == CONSOLE_QUIT_CODE) {
				//if (load_game(game, stdin) == CONSOL_QUIT_CODE) {
					//fprintf(stderr, "Error: load_game failed.\n");
					break;
				}
				if ((eval = evaluateGame(game)) > MAX_EVALUATION) {
					fprintf(stderr, "Error: evaluateGame failed.\n");
					fprintf(stdout, "q\n");
					break;
				}
				// if the evaluation is for the mouse
				if (!strcmp(argv[2], "mouse")) {
					eval = -eval;
				}
				printf("%d\n", eval);
			}
			game_free(game);
		} else {
			return run_gui();
		}
	} else {
		return run_gui();
	}
	return 0;
}
