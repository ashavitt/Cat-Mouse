# Cat-Mouse - Software Project - TAU 2015
C final project
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
asdasdasd asd 
#### Window
#### Panel
#### Graphic
#### Button
##### Onclick

## List of files

## Utility evaluation function:
	high level description - 
	reasoning -
	running time - 
