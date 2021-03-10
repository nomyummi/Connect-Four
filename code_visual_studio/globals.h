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
#endif
