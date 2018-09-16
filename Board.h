#ifndef BOARD_H
#define BOARD_H

#include "Chip.h"
#include "constants.h"
#include "AI.h"
class Texture;

class Board
{
public:
	Board(int player_one_color, int player_two_color);
	~Board();
	void turn(int col);
	void AImove();
	void redoPreviousMove();
	bool winner(int player);
	bool tie();
	bool checkCols(int player);
	bool checkRows(int player);
	bool checkBackDiagonal(int player);
	bool checkForwardDiagonal(int player);
	void replay();
	void switchTurns();
	int isTurn() { return playerTurn; }
	Chip* m_grid[MAXROWS][MAXCOLS]; //m_grid[0][0] is the top left corner of the board.
private:
	Chip m_players[NUMPLAYERS];
	AI* m_robot;
	int m_previous_move_row; //row is the row number (starting with 1). The array m_grid has rows that start with 0.
	int m_previous_move_col; //col is the column number (starting with 1). The array m_grid has cols that start with 0.
	bool m_undid_once_already;
	int playerTurn;

};


#endif
