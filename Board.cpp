#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "constants.h"
#include "Texture.h"
#include "Chip.h"
#include "Board.h"
#include <random>

//Add to check if valid move 

Board::Board(int player_one_color, int player_two_color)
{
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			m_grid[i][j] = NULL;
		}
	}
	m_players[0] = Chip(player_one_color, P1);
	m_players[1] = Chip(player_two_color, P2);
	m_previous_move_row = 0;
	m_previous_move_col = 0;
	m_undid_once_already = false;
	playerTurn = P1;
	m_robot = new AI();
}

Board::~Board()
{
	delete m_robot;
}
void Board::replay()
{
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			m_grid[i][j] = NULL;
			m_robot->updateBoardState(0, i,j);
		}
	}
	playerTurn = P1;
}
void Board::turn(int col)
{
	for (int r = MAXROWS - 1; r >= 0; r--)
	{
		if (m_grid[r][col - 1] == NULL)
		{
			m_grid[r][col - 1] = &m_players[playerTurn - 1];
			m_robot->updateBoardState(playerTurn, r, col - 1);
			m_previous_move_row = r + 1;
			m_previous_move_col = col;
			m_undid_once_already = false;
			switchTurns();
			break;
		}
	}
}


void Board::randomMove()
{
	int random_column = rand() % 7 + 1;
	while (!validMove(random_column))
	{
		random_column = rand() % 7 + 1;
	}
	//turn(random_column);
	m_robot->generateTree();
	turn(m_robot->findBestMove());
	m_robot->printTree();
	m_robot->resetTree();
	//m_robot->printTree(); //Cannot print tree for some reason. 


	
}


void Board::switchTurns()
{
	if (playerTurn == P1)
		playerTurn = P2;
	else playerTurn = P1;
}
bool Board::validMove(int col)
{
	if (m_grid[0][col - 1] == NULL)
		return true;
	else
		return false;
}

void Board::redoPreviousMove()
{
	if (!m_undid_once_already)
	{
		m_grid[m_previous_move_row - 1][m_previous_move_col - 1] = NULL;
		m_robot->updateBoardState(0, m_previous_move_row - 1, m_previous_move_col - 1);
		m_undid_once_already = true;
		switchTurns();
	}
}
bool Board::winner(int player)
{
	return checkCols(player) || checkRows(player) || checkForwardDiagonal(player) || checkBackDiagonal(player);
}

bool Board::checkCols(int player)
{
	for (int c = 0; c < MAXCOLS; c++)
	{
		for (int r = MAXROWS - 4; r >= 0; r--)
		{
			if (m_grid[r][c] != NULL && m_grid[r + 1][c] != NULL && m_grid[r + 2][c] != NULL && m_grid[r + 3][c] != NULL)
			{
				if (m_grid[r][c]->player() == player && m_grid[r + 1][c]->player() == player && m_grid[r + 2][c]->player() == player && m_grid[r + 3][c]->player() == player)
				{
					return true;
				}
			}

		}
	}
	return false;
}


bool Board::checkRows(int player)
{
	for (int r = 0; r < MAXROWS; r++)
	{
		for (int c = 0; c < MAXCOLS - 3; c++)
		{
			if (m_grid[r][c] != NULL && m_grid[r][c + 1] != NULL && m_grid[r][c + 2] != NULL && m_grid[r][c + 3] != NULL)
			{
				if (m_grid[r][c]->player() == player && m_grid[r][c + 1]->player() == player && m_grid[r][c + 2]->player() == player && m_grid[r][c + 3]->player() == player)
				{
					return true;
				}
			}

		}
	}
	return false;
}


bool Board::checkForwardDiagonal(int player)
{
	for (int r = MAXROWS - 1; r >= 3; r--)
	{
		for (int c = 0; c < MAXCOLS - 3; c++)
		{
			if (m_grid[r][c] != NULL && m_grid[r - 1][c + 1] != NULL && m_grid[r - 2][c + 2] != NULL && m_grid[r - 3][c + 3] != NULL)
			{
				if (m_grid[r][c]->player() == player && m_grid[r - 1][c + 1]->player() == player && m_grid[r - 2][c + 2]->player() == player && m_grid[r - 3][c + 3]->player() == player)
				{
					return true;
				}
			}

		}
	}
	return false;
}
bool Board::checkBackDiagonal(int player)
{
	for (int r = 0; r < MAXROWS - 3; r++)
	{
		for (int c = 0; c < MAXCOLS - 3; c++)
		{
			if (m_grid[r][c] != NULL && m_grid[r + 1][c + 1] != NULL && m_grid[r + 2][c + 2] != NULL && m_grid[r + 3][c + 3] != NULL)
			{
				if (m_grid[r][c]->player() == player && m_grid[r + 1][c + 1]->player() == player && m_grid[r + 2][c + 2]->player() == player && m_grid[r + 3][c + 3]->player() == player)
				{
					return true;
				}
			}

		}
	}
	return false;
}

bool Board::tie()
{
	//Every part of the board must be filled before a tie can be called
	for (int r = 0; r < MAXROWS; r++)
	{
		for (int c = 0; c < MAXCOLS; c++)
		{
			if (m_grid[r][c] == NULL)
				return false;
		}
	}
	return true;
}
