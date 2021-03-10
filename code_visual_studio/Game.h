#ifndef GAME_H
#define GAME_H

#include "GameState.h"
#include "Board.h"
class Game : public GameState
{
public:
	Game();
	virtual ~Game();

	bool load_images();
	void handle_events();
	void render();
	void keyboardControls(SDL_Event event, Board* gameBoard, bool* gridWhite, Uint8* red, Uint8* blue, Uint8* green);
private:
	Board board = Board(REDCHIP, BLUECHIP);
	Texture* currentGrid;
	Texture grids[NUM_COLORS_GRID];
	bool gridWhite;
	Uint8 red;
	Uint8 green;
	Uint8 blue;
	Texture spriteSheet;
	Texture tRows;
	Texture tControls;
	Texture gameWinner[NUMPLAYERS + 1];
	Texture tPlayerTurn[NUMPLAYERS];
	SDL_Rect chipQuadrants[4];
	int playerWon;
	bool gameOver;
};
#endif