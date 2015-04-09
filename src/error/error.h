#ifndef __ERROR__
#define __ERROR__


/*** GUI ***/
/** Widget **/

/* draw_widget */


/* add_rect */
#define ERROR_NO_RECT1 2
#define ERROR_NO_RECT2 3

/* draw_widget */
#define ERROR_NO_WIDGET 10
#define ERROR_NO_WINDOW 11
#define ERROR_BLIT_FAIL 12

/** handlers **/
#define ERROR_NO_SDLEVENT 13
#define ERROR_FOCUSED_ID 14

/** UITree **/

/* build_ui */
#define ERROR_APPEND_FAILED 20
#define ERROR_ADD_RECT_FAILED 21
/** main **/

/* run_gui */
#define ERROR_BUILD_UI_FAILED 30
#define ERROR_WIDGET_FACTORY_FAILED 31
#define ERROR_DRAW_WIDGET_FAILED 32
#define ERROR_FILL_RECT_FAILED 33

/** actions **/
#define ERROR_NO_STATE 40

/*** General ***/
#define ERROR_MALLOC_FAILED 100

#endif
