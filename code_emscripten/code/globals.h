#ifndef GLOBALS_H
#define GLOBALS_H
#include "SDL.h"
#include "globals.h"
#include "Texture.h"

extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern void set_next_state(int newState);
extern SDL_Event e;
extern int stateID;
extern int nextState;
extern bool changeToOnePlayerMode; 
// BUG - Enscriptem - nextState is bugged in Enscriptem. Probably due to it being global. 
// nextState will set itself to a random number sometime between handleEvents() and changeState() in main.
// This happens when you go out of the SDL window bounds, or an event happens (keyboard press, mouse move).
// Created changeToOnePlayer as a workaround.
#endif
