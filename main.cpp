#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "constants.h"
#include "Texture.h"
#include "Chip.h"
#include "Board.h"
#include <iostream>
#include "globals.h"

bool init();
bool loadMedia();
void close();
void keyboardControls(SDL_Event event, Board* gameBoard, int* playerTurn, bool* gridWhite, Uint8* red, Uint8* blue, Uint8* green);
void switchTurns(int* playerTurn);

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
Texture* currentGrid;
Texture* currentButton;
Texture grids[NUM_COLORS_GRID];
Texture spriteSheet;
Texture gameWinner[NUMPLAYERS + 1];
Texture tPlayerTurn[NUMPLAYERS];
Texture tRows;
Texture tControls;
Texture playScreen;
Texture tPlayButton[2];
SDL_Rect chipQuadrants[4];

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			int x, y;
			bool inside;
			Uint8 red = 0xFF;
			Uint8 green = 0xFF;
			Uint8 blue = 0xFF;
			bool gridWhite = false;
			currentGrid = &grids[BLUE];
			currentButton = &tPlayButton[BUTTON_SPRITE_MOUSE_DOWN];
			Board board = Board(REDCHIP, BLUECHIP);
			int playerTurn = P1;
			bool gameOver = false;   
			int playerWon = 0;
			bool playButtonClicked = false;

			//While application is running
			while (!quit)
			{
				//Game Menu State
				if (playButtonClicked == false)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						//mouse events
						if (e.type == SDL_QUIT)
						{
							quit = true;
						}
						if (e.type == SDL_KEYDOWN)
						{
							if (e.key.keysym.sym == SDLK_q)
								quit = true;
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
									playButtonClicked = true;
								}
							}

							else currentButton = &tPlayButton[BUTTON_SPRITE_MOUSE_DOWN];
						}
					}
				}
				
				//Game State
				while (playButtonClicked == true && SDL_PollEvent(&e) != 0)
					{
						//User requests quit
						if (e.type == SDL_QUIT)
						{
							quit = true;
						}
						else if (e.type == SDL_KEYDOWN)
						{
							if (!gameOver)
							{
								keyboardControls(e, &board, &playerTurn, &gridWhite, &red, &blue, &green);
								if (e.key.keysym.sym == SDLK_q)
									quit = true;
							}
							else if (gameOver)
							{
								if (e.key.keysym.sym == SDLK_q)
									quit = true;
								if (e.key.keysym.sym == SDLK_c)
								{
									gameOver = false;
									board.replay();
									playerTurn = 1;
								}
							}
						}
					}
					//Clear screen
					SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(renderer);
			
					if (playButtonClicked == true)
					{
						//Set viewport for the board
						SDL_Rect boardViewport;
						boardViewport.x = 0;
						boardViewport.y = 0;
						boardViewport.w = 640;
						boardViewport.h = 480;
						SDL_RenderSetViewport(renderer, &boardViewport);

						//Render the pieces 
						for (int r = 1; r <= MAXROWS; r++)
						{
							for (int c = 1; c <= MAXCOLS; c++)
							{
								if (board.m_grid[r - 1][c - 1] != NULL)
									spriteSheet.render(renderer, 14 + (c - 1)*GRIDSPACE_WIDTH, 5 + (r - 1)*GRIDSPACE_HEIGHT, 72, 72, &board.m_grid[r - 1][c - 1]->spriteSheetQuadrant());
							}
						}

						//Render the board;
						if (gridWhite)
							currentGrid->setColor(red, green, blue);

						currentGrid->render(renderer, 0, 0);

						//Row numbering viewport
						SDL_Rect rowNumberingViewport;
						rowNumberingViewport.x = 0;
						rowNumberingViewport.y = 480;
						rowNumberingViewport.w = 640;
						rowNumberingViewport.h = 50;
						SDL_RenderSetViewport(renderer, &rowNumberingViewport);

						tRows.render(renderer, 0, 0);

						//Controls viewport
						SDL_Rect controlsViewport;
						controlsViewport.x = 640;
						controlsViewport.y = 0;
						controlsViewport.w = 180;
						controlsViewport.h = 480;
						SDL_RenderSetViewport(renderer, &controlsViewport);

						tControls.render(renderer, 0, 0);

						//Bottom right corner viewport
						SDL_Rect bottomRightCornerViewport;
						bottomRightCornerViewport.x = 640;
						bottomRightCornerViewport.y = 480;
						bottomRightCornerViewport.w = 180;
						bottomRightCornerViewport.h = 180;
						SDL_RenderSetViewport(renderer, &bottomRightCornerViewport);


						//Render the player's chip color
						if (playerTurn == P1)
						{
							if (!gameOver)
								spriteSheet.render(renderer, 15, 15, 150, 150, &chipQuadrants[REDCHIP]);
							else
								spriteSheet.render(renderer, 15, 15, 150, 150, &chipQuadrants[BLUECHIP]);
						}
						if (playerTurn == P2)
						{
							if (!gameOver)
								spriteSheet.render(renderer, 15, 15, 150, 150, &chipQuadrants[BLUECHIP]);
							else
								spriteSheet.render(renderer, 15, 15, 150, 150, &chipQuadrants[REDCHIP]);
						}


						//Bottom panel viewport
						SDL_Rect bottomPanelViewport;
						bottomPanelViewport.x = 0;
						bottomPanelViewport.y = 530;
						bottomPanelViewport.w = 640;
						bottomPanelViewport.h = 120;
						SDL_RenderSetViewport(renderer, &bottomPanelViewport);

						//Render the winner to the screen if there's a winner
						if (gameOver)
						{
							if (playerWon == P1)
								gameWinner[P1].render(renderer, 0, 0);
							else if (playerWon == P2)
								gameWinner[P2].render(renderer, 0, 0);
							else gameWinner[0].render(renderer, 0, 0);
						}
						else
						{
							//Otherwise render whose turn it is
							if (playerTurn == P1)
							{
								tPlayerTurn[P1 - 1].render(renderer, 0, 0);
							}
							else tPlayerTurn[P2 - 1].render(renderer, 0, 0);
						}



						//Update screen
						SDL_RenderPresent(renderer);

						//Check if there's a winner
						if (board.winner(P1))
						{
							gameOver = true;
							playerWon = P1;
						}
						else if (board.winner(P2))
						{
							gameOver = true;
							playerWon = P2;
						}
						else if (board.tie())
						{
							gameOver = true;
							playerWon = 0;
						}

					}
					if (playButtonClicked == false)
					{
						currentButton->render(renderer, PLAYBUTTON_X, PLAYBUTTON_Y);
						SDL_RenderPresent(renderer);
					}
			}
		}
	}
	//Free resources and close SDL
	close();

	return 0;
}

void switchTurns(int* playerTurn)
{
	if (*playerTurn == P1)
		*playerTurn = P2;
	else *playerTurn = P1;
}

void keyboardControls(SDL_Event event, Board* gameBoard, int* playerTurn, bool* gridWhite, Uint8* red, Uint8* blue, Uint8* green){
	switch (event.key.keysym.sym)
	{
	case SDLK_8:
		currentGrid = &grids[BLUE]; //Original board
		*gridWhite = false;
		break;
	case SDLK_9:
		currentGrid = &grids[BLACK]; //Turn the board black
		*gridWhite = false;
		break;
	case SDLK_0:
		currentGrid = &grids[WHITE]; //White board
		*gridWhite = true;
		break;
	case SDLK_r:
		*red += 17;
		break;
	case SDLK_g:
		*green += 17;
		break;
	case SDLK_b:
		*blue += 17;
		break;
	case SDLK_c:
		gameBoard->replay();
		*playerTurn = 1;
		break;
	case SDLK_z:
		gameBoard->redoPreviousMove();
		switchTurns(playerTurn);
		break;
	case SDLK_1:
		if (gameBoard->validMove(1))
		{
			gameBoard->turn(1, *playerTurn);
			switchTurns(playerTurn);
		}
		break;
	case SDLK_2:
		if (gameBoard->validMove(2))
		{
			gameBoard->turn(2, *playerTurn);
			switchTurns(playerTurn);
		}
		break;
	case SDLK_3:
		if (gameBoard->validMove(3))
		{
			gameBoard->turn(3, *playerTurn);
			switchTurns(playerTurn);
		}
		break;
	case SDLK_4:
		if (gameBoard->validMove(4))
		{
			gameBoard->turn(4, *playerTurn);
			switchTurns(playerTurn);
		}
		break;
	case SDLK_5:
		if (gameBoard->validMove(5))
		{
			gameBoard->turn(5, *playerTurn);
			switchTurns(playerTurn);
		}
		break;
	case SDLK_6:
		if (gameBoard->validMove(6))
		{
			gameBoard->turn(6, *playerTurn);
			switchTurns(playerTurn);
		}
		break;
	case SDLK_7:
		if (gameBoard->validMove(7))
		{
			gameBoard->turn(7, *playerTurn);
			switchTurns(playerTurn);
		}
		break;
	}
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

	//Load grids

	if (!grids[BLUE].loadFile(renderer,"Connect4Board.png"))
	{
		printf("Failed to load blue board texture image!\n");
		success = false;
	}
	if (!grids[WHITE].loadFile(renderer, "Connect4Board_White.png"))
	{
		printf("Failed to load white board texture image!\n");
		success = false;
	}
	if (!grids[BLACK].loadFile(renderer, "Connect4Board_Black.png"))
	{
		printf("Failed to load black board texture image!\n");
		success = false;
	}
	if (!spriteSheet.loadFile(renderer, "Connect4Pieces.png"))
	{
		printf("Failed to load piece texture image!\n");
		success = false;
	}
	if (!gameWinner[0].loadFile(renderer, "Tie.png"))
	{
		printf("Failed to load tie texture image!\n");
		success = false;
	}
	if (!gameWinner[P1].loadFile(renderer, "Player1Wins.png"))
	{
		printf("Failed to load P1 winner texture image!\n");
		success = false;
	}
	if (!gameWinner[P2].loadFile(renderer, "Player2Wins.png"))
	{
		printf("Failed to load P2 winner texture image!\n");
		success = false;
	}
	if (!tPlayerTurn[P1-1].loadFile(renderer, "Player1Turn.png"))
	{
		printf("Failed to load P1s turn texture image!\n");
		success = false;
	}
	if (!tPlayerTurn[P2-1].loadFile(renderer, "Player2Turn.png"))
	{
		printf("Failed to load P2s turn texture image!\n");
		success = false;
	}
	if (!tRows.loadFile(renderer, "Rows.png"))
	{
		printf("Failed to load row numbers texture image!\n");
		success = false;
	}
	if (!tControls.loadFile(renderer, "Controls.png"))
	{
		printf("Failed to load controls texture image!\n");
		success = false;
	}

	if (!tPlayButton[BUTTON_SPRITE_MOUSE_OVER_MOTION].loadFile(renderer, "PlayGray.png"))
	{
		printf("Failed to load controls texture image!\n");
		success = false;
	}
	if (!tPlayButton[BUTTON_SPRITE_MOUSE_DOWN].loadFile(renderer, "Play.png"))
	{
		printf("Failed to load controls texture image!\n");
		success = false;
	}


	chipQuadrants[REDCHIP].x = 0;
	chipQuadrants[REDCHIP].y = 0;
	chipQuadrants[REDCHIP].w = 150;
	chipQuadrants[REDCHIP].h = 150;

	chipQuadrants[BLUECHIP].x = 150;
	chipQuadrants[BLUECHIP].y = 0;
	chipQuadrants[BLUECHIP].w = 150;
	chipQuadrants[BLUECHIP].h = 150;

	chipQuadrants[YELLOWCHIP].x = 0;
	chipQuadrants[YELLOWCHIP].y = 150;
	chipQuadrants[YELLOWCHIP].w = 150;
	chipQuadrants[YELLOWCHIP].h = 150;

	chipQuadrants[GREENCHIP].x = 150;
	chipQuadrants[GREENCHIP].y = 150;
	chipQuadrants[GREENCHIP].w = 150;
	chipQuadrants[GREENCHIP].h = 150;

	return success;
}

void close()
{
	////Free loaded images
	grids[BLUE].free();
	grids[WHITE].free();
	grids[BLACK].free();
	spriteSheet.free();
	gameWinner[0].free();
	gameWinner[P1].free();
	gameWinner[P2].free();
	tPlayerTurn[P1-1].free();
	tPlayerTurn[P2-1].free();
	tRows.free();

	//Destroy window	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}