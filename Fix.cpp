#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "constants.h"
#include "Texture.h"
#include "Chip.h"
#include "Board.h"
#include "AI.h"

AI::AI()
{
	root = new Node();
	root->depth = 0;
	root->playerTurn = P1;
	for (int i = 0; i < MAXCOLS; i++)
		root->nextMoves[i] = nullptr;

	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			currentBoardState[i][j] = 0;
			root->boardState[i][j] = 0;
		}
	}
	
}

AI::~AI()
{
	delete root;
}

void AI::resetTree()
{

}

void AI::generateTree()
{
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			root->boardState[i][j] = currentBoardState[i][j];
		}
	}
	Node* p;
	if (p->depth == MAXDEPTH-1)
		return;
	
}

Node* AI::newNode(Node* parent, int col)
{
	Node* n = new Node();
	n->depth = parent->depth + 1;
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			n->boardState[i][j] = parent->boardState[i][j];
		}
	}
	for (int r = MAXROWS - 1; r >= 0; r--)
	{
		if (n->boardState[r][col] == 0)
		{
			if (parent->playerTurn == P1)
			else n->boardState[r][col = P1;
			break;
		}
	}
	for (int i = 0; i < MAXCOLS; i++)
		n->nextMoves[i] = nullptr;
}
void AI::updateBoardState(int playerTurn, int row, int col)
{
	currentBoardState[row][col] = playerTurn;
}

//Base implementation, check for win
int AI::evaluationFunction()
{
	if (isWinner(P2)) //P2 is AI
		return 1000;
	if (isWinner(P1))
		return -1000;

	return 0;
}

int AI::minimax(int currentBoard[MAXROWS][MAXCOLS], int depth, bool isMaximizingPlayer)
{

}

bool AI::isWinner(int player)
{
	//Check cols
	for (int c = 0; c < MAXCOLS; c++)
	{
		for (int r = MAXROWS - 4; r >= 0; r--)
		{
			if (currentBoardState[r][c] == player && currentBoardState[r + 1][c] == player && currentBoardState[r + 2][c] == player && currentBoardState[r + 3][c] == player)
			{
				return true;
			}

		}
	}
	//Check rows
	for (int r = 0; r < MAXROWS; r++)
	{
		for (int c = 0; c < MAXCOLS - 3; c++)
		{

			if (currentBoardState[r][c] == player && currentBoardState[r][c + 1] == player && currentBoardState[r][c + 2] == player && currentBoardState[r][c + 3] == player)
			{
				return true;
			}
		}
	}
	//Check forward diagonals
	for (int r = MAXROWS - 1; r >= 3; r--)
	{
		for (int c = 0; c < MAXCOLS - 3; c++)
		{

			if (currentBoardState[r][c] == player && currentBoardState[r - 1][c + 1] == player && currentBoardState[r - 2][c + 2] == player && currentBoardState[r - 3][c + 3] == player)
			{
				return true;
			}

		}
	}
	//Check back diagonals
	for (int r = 0; r < MAXROWS - 3; r++)
	{
		for (int c = 0; c < MAXCOLS - 3; c++)
		{

			if (currentBoardState[r][c] == player && currentBoardState[r + 1][c + 1] == player && currentBoardState[r + 2][c + 2] == player && currentBoardState[r + 3][c + 3] == player)
			{
				return true;
			}
		}
	}

	return false;
}


