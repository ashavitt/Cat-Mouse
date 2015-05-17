#ifndef _MAIN_GUI_H_
#define _MAIN_GUI_H_

#include "SDL.h"
#include "../main/ListUtils.h"
#include "../core/ai.h"
#include "shared.h"
#include "widget.h"
#include "uitree.h"
#include "../error/error.h"
#include "handlers.h"
#include "../io/file_io.h"

//Screen attributes
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define BPP 32 //bits per pixel

#define WAIT_TIME_MACHINE_MOVE 1000

int run_gui();

#endif
