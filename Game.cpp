#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "constants.h"
#include "Texture.h"
#include "Game.h"
#include "globals.h"


bool Game::load_images()
{
	bool success = true;
	if (!grids[BLUE].loadFile(renderer, "Connect4Board.png"))
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
	if (!tPlayerTurn[P1 - 1].loadFile(renderer, "Player1Turn.png"))
	{
		printf("Failed to load P1s turn texture image!\n");
		success = false;
	}
	if (!tPlayerTurn[P2 - 1].loadFile(renderer, "Player2Turn.png"))
	{
		printf("Failed to load P2s turn texture image!\n");
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
Game::Game()
{
	if (!load_images())
		set_next_state(STATE_EXIT);

	currentGrid = &grids[BLUE];
	gridWhite = false;
	red = 0xFF;
	blue = 0xFF;
	green = 0xFF;
	playerWon = 0;
	gameOver = false;
}

Game::~Game()
{
	grids[BLUE].free();
	grids[WHITE].free();
	grids[BLACK].free();
	spriteSheet.free();
	tRows.free();
	tControls.free();
	gameWinner[0].free();
	gameWinner[P1].free();
	gameWinner[P2].free();
	tPlayerTurn[P1 - 1].free();
	tPlayerTurn[P2 - 1].free();
}
void Game::handle_events()
{
	//User requests quit
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			set_next_state(STATE_EXIT);
		}
		else if (e.type == SDL_KEYDOWN)
		{
			if (!gameOver)
			{
				keyboardControls(e, &board, &gridWhite, &red, &blue, &green);
				if (e.key.keysym.sym == SDLK_q)
					set_next_state(STATE_EXIT);
				if (e.key.keysym.sym == SDLK_o)
					set_next_state(STATE_ONE_PLAYER_MODE);
			}
			else if (gameOver)
			{
				if (e.key.keysym.sym == SDLK_q)
					set_next_state(STATE_EXIT);
				if (e.key.keysym.sym == SDLK_c)
				{
					gameOver = false;
					board.replay();
				}
			}
		}
	}
	
}

void Game::keyboardControls(SDL_Event event, Board* gameBoard, bool* gridWhite, Uint8* red, Uint8* blue, Uint8* green) {
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
		break;
	case SDLK_z:
		gameBoard->redoPreviousMove();
		break;
	case SDLK_1:
		gameBoard->turn(1);
		break;
	case SDLK_2:
		gameBoard->turn(2);
		break;
	case SDLK_3:
		gameBoard->turn(3);
		break;
	case SDLK_4:
		gameBoard->turn(4);
		break;
	case SDLK_5:
		gameBoard->turn(5);
		break;
	case SDLK_6:
		gameBoard->turn(6);
		break;
	case SDLK_7:
		gameBoard->turn(7);
		break;
	}
}

void Game::render()
{
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

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
	if (board.isTurn() == P1)
	{
		if (!gameOver)
			spriteSheet.render(renderer, 15, 15, 150, 150, &chipQuadrants[REDCHIP]);
		else
			spriteSheet.render(renderer, 15, 15, 150, 150, &chipQuadrants[BLUECHIP]);
	}
	if (board.isTurn() == P2)
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
		if (board.isTurn() == P1)
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