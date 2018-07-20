#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "constants.h"
#include "Texture.h"
#include "Chip.h"
#include "Board.h"
#include "Title.h"
#include "Game.h"
#include "GameState.h"
#include "OnePlayerMode.h"
#include <iostream>
#include "globals.h"

bool init();
bool loadMedia();
void close();

void set_next_state(int newState);
void change_state();

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
SDL_Event e;
int stateID = STATE_NULL;
int nextState = STATE_NULL;
GameState *currentState = NULL;

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	//Load media
	if (!loadMedia())
	{
		printf("Failed to load media!\n");
	}
	stateID = STATE_TITLE;

	currentState = new Title();

	//While the user hasn't quit
	while (stateID != STATE_EXIT)
	{
		//Do state event handling
		currentState->handle_events();

		//Do state logic
		currentState->logic();

		//Change state if needed
		change_state();

		//Do state rendering
		currentState->render();
	}
	//Free resources and close SDL
	close();

	return 0;
}



bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		window = SDL_CreateWindow("Connect Four", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color to white
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	return success;
}

void set_next_state(int newState)
{
	//If the user doesn't want to exit
	if (nextState != STATE_EXIT)
	{
		//Set the next state
		nextState = newState;
	}
}
void change_state()
{
	//If the state needs to be changed
	if (nextState != STATE_NULL)
	{
		//Delete the current state
		if (nextState != STATE_EXIT)
		{
			delete currentState;
		}
		//Change the state
		switch (nextState)
		{
		case STATE_TITLE:
			currentState = new Title();
			break;
		case STATE_GAME:
			currentState = new Game();
			break;
		case STATE_ONE_PLAYER_MODE:
			currentState = new OnePlayerMode();
			break;
		}

		//Change the current state ID
		stateID = nextState;

		//NULL the next state ID
		nextState = STATE_NULL;
	}
}
void close()
{
	////Free loaded images


	//Destroy window	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}