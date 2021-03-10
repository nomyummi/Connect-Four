#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "constants.h"
#include "Texture.h"
#include "Title.h"
#include "globals.h"

Title::Title()
{
	if (!tPlayButton[BUTTON_SPRITE_MOUSE_OVER_MOTION].loadFile(renderer, "images/PlayGray.png"))
	{
		printf("Failed to load controls texture image!\n");
	}
	if (!tPlayButton[BUTTON_SPRITE_MOUSE_DOWN].loadFile(renderer, "images/Play.png"))
	{
		printf("Failed to load controls texture image!\n");
	}
	if (!ConnectFourTitle.loadFile(renderer, "images/ConnectFourTitle.png"))
	{
		printf("Failed to load controls texture image!\n");
	}
	currentButton = &tPlayButton[BUTTON_SPRITE_MOUSE_DOWN];
}

Title::~Title()
{
	//Free the surfaces
	tPlayButton[BUTTON_SPRITE_MOUSE_OVER_MOTION].free();
	tPlayButton[BUTTON_SPRITE_MOUSE_DOWN].free();
	ConnectFourTitle.free();
}

void Title::handle_events()
{
	//Game Menu State
	int x, y;
	bool inside;

	while (SDL_PollEvent(&e) != 0)
	{
		//mouse events
		if (e.type == SDL_QUIT)
		{
			set_next_state(STATE_EXIT);
		}
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_q)
				set_next_state(STATE_EXIT);
		}
		if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_GetMouseState(&x, &y);
			inside = true;
			if (x < PLAYBUTTON_X)
			{
				inside = false;
			}
			//Mouse is right of the button
			else if (x > PLAYBUTTON_X + PLAYBUTTON_WIDTH)
			{
				inside = false;
			}
			//Mouse above the button
			else if (y < PLAYBUTTON_Y)
			{
				inside = false;
			}
			//Mouse below the button
			else if (y > PLAYBUTTON_Y + PLAYBUTTON_HEIGHT)
			{
				inside = false;
			}
			if (inside == true)
			{
				if (e.type == SDL_MOUSEMOTION)
				{
					currentButton = &tPlayButton[BUTTON_SPRITE_MOUSE_OVER_MOTION];
				}
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					currentButton = &tPlayButton[BUTTON_SPRITE_MOUSE_DOWN];
					set_next_state(STATE_GAME);
				}
			}
			else currentButton = &tPlayButton[BUTTON_SPRITE_MOUSE_DOWN];
		}
	}

}

void Title::render()
{
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	currentButton->render(renderer, PLAYBUTTON_X, PLAYBUTTON_Y);
	ConnectFourTitle.render(renderer, TITLE_IMAGE_X, TITLE_IMAGE_Y);
	SDL_RenderPresent(renderer);
}
