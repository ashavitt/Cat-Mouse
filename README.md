# Cat-Mouse - Software Project - TAU 2015
README.txt | C final project
Tip: read with line wrapping.
###### Partners: Amir Shavitt and Nitzan Pomerantz

## Introduction
In this project we were required to build a GUI application using SDL for a board game of Cat & Mouse with optional AI players (based on ex3) for both characters. We have also implemented AI evaluation function to copete in the class tournament.

## Design Choices
We have implemented two different generic UI subsystems to be used in our project:
### Game States
Our game state structure consist of all the required information regarding a specific screen of the game; using this information the screen visuals can be built and also be used by some HUD (mouse, keyboard).
The game state structure also behaves as a struct; each state holds a pointer to the previous state in the stack.
- When "pushing" down a state, we just set the new state as the current and point the previous.
- When "popping" up a state, we set the previous state as the current and free the popped state.
This way we can hold several game states together, thus holding the information for the current state and all previous ones altogether.

### Widgets
Widgets are GUI entities, designated to hold the GUI tree for some window.
Widgets come in one of four types: Window, Panel, Graphic or Button. Each type has its own purpose and usage and those are explained below.
Even though implementation-wise all widget type use the same structure and variables, logically they need only specific parts of it for their functioning.
A basic generic widget consists of:
- id (number) (used for focus)
- type (from the types described above and below)
- SDL_Rect dims - holding the width and height in its w,h fields
- A pointer to a parent widget
- A list of children widget (possibly empty)

and for graphic, button and panel:
- SDL_Rect pos - holding the position relative to the parent, and possible a redundant (because of dims) w,h dimensions

#### Widget Drawing and Positioning
Drawing to the screen is done only for the tree root (even though it is possible to start drawing from a non-root widget).
The drawing function sums up the relative coordinates for each child, and positions the widget at the sum of all its parent's and its own coordinates - which is its absolute position, or equivalently, relative to the root.
Children widgets are drawn, virtually and actually, "on top of" their parent. Only graphic widgets can be visually seen, so if some graphic is a child of some other, larger graphic, and positioned accordingly, the child graphic will appear on top of the parent graphic.
For simple positioning, we have implemented a get_center function, which returns the SDL_Rect pos that will position the widget at the center (horizontally and vertically) of its parent. 
#### Widget Types
##### Window
A window is a widget-tree's root. There is only one window in a widget-tree. A window doesn't employ any additional fields but the basic ones in a widget structure.
##### Panel
A panel is a widget that's used to hold several widgets together. Usually it is used to allow easier positioning of several widgets that are positioned together. The panel uses the pos field to hold its position and dimensions (when required; could replicate dimensions into dims). 
##### Graphic
A graphic widget uses the dims field to hold the blitting SDL_Rect. This rectangle will be used to blit the required image part out of the sprite surface:
- SDL_Surface *imgsrc

Then the graphic will be positioned relatively to its parent and according to the pos field.
##### Button
A button widget is used to describe clickable areas on the screen. The clickable area is defined by the pos field, using also its w,h fields. A button widget uses a function pointer field to hold the handler to be called when a click occurs:
- int (*onclick)(struct widget*, game_state*)

We assume that no button areas intersect, and if they do one will be selected arbitrarily to preform an action.
This onclick function has a constant header and recieves some widget and the game structure.

Using the id field, we can set focus to widgets. In the project we have only set focus to button widgets. When the state marks some widget id as focused, we can search for it in the widget tree using its id (find_widget_by_id) and change the button's background to appear focused, and also to call its click handler using the keyboard RETURN key.
## Error Handling
We check for general runtime errors (allocation errors, SDL errors) every time such possible errors may occur. If such an error occurs, we print an error message to stderr and return some value to indicate an error occured. Such a value is NULL for functions returning pointers or some number other than 0, according to the error codes defined in error/error.h . The function calling the function where the error occured, will recieve an error code and print to stderr the higher level message about the error. Thus messages will be printed in all function-call depth, explaining exactly where the error occured. Eventually, some information may be retrieved from the program's return value using the $? command.
## Dependency Tree

## Directory Hierarchy

## Utility evaluation function:
	high level description - 
	reasoning -
	running time - 