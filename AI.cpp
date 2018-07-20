#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include "constants.h"
#include "Texture.h"
#include "Chip.h"
#include "Board.h"
#include "AI.h"
#include <algorithm>

AI::AI()
{
	root = new Node();
	root->value = -100000;
	root->depth = 0;
	root->playerTurn = P2; //P2 is the maximizing player. 
	root->column = 0;
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
	/*for (int i = 0; i < MAXCOLS; i++)
	{
	deleteNodes(root->nextMoves[i]);
	}*/
	deleteNodes(root);
	//delete root; //Destructor is called when switching from two player to one player. Not sure why it bugs out though if this is uncommented
}

void AI::resetTree()
{
	deleteNodes(root);
}
void AI::deleteNodes(Node* node)
{
	if (node == nullptr) //should not reach here
		return;

	bool isTerminatingNode = true;
	for (int i = 0; i < MAXCOLS; i++)
	{
		if (node->nextMoves[i] != nullptr)
		{
			isTerminatingNode = false;
			break;
		}

	}
	if (isTerminatingNode == true)
	{
		delete node;
		node = nullptr;
		return;
	}

	for (int i = 0; i < MAXCOLS; i++)
	{
		deleteNodes(node->nextMoves[i]);
	}
	//if (!root)
	//	delete node;

}

void AI::generateTree()
{
	//Update root 
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			root->boardState[i][j] = currentBoardState[i][j];
		}
	}
	addNodestoTree(root);
}

void AI::addNodestoTree(Node* parent)
{
	if (parent == nullptr)
		return;
	if (parent->depth == MAXDEPTH)
		return;
	for (int i = 0; i < MAXCOLS; i++)
	{
		parent->nextMoves[i] = newNode(parent, i);
		if (parent->nextMoves[i] != nullptr)
			addNodestoTree(parent->nextMoves[i]);
	}
}

Node* AI::newNode(Node* parent, int col)
{
	Node* n;

	bool possibleMove = false;
	for (int r = MAXROWS - 1; r >= 0; r--)
	{
		if (parent->boardState[r][col] == 0)
		{
			possibleMove = true;
			n = new Node();
			n->depth = parent->depth + 1;
			//Copy the boardstate of the parent
			for (int i = 0; i < MAXROWS; i++)
			{
				for (int j = 0; j < MAXCOLS; j++)
				{
					n->boardState[i][j] = parent->boardState[i][j];
				}
			}
			if (parent->playerTurn == P2) //P2 is making a move
			{
				n->column = col;
				n->boardState[r][col] = P2; //P2 makes move
				n->playerTurn = P1;  //It is now P1's turn to make a move
				n->value = 100000; //values of the minimizing player is -100000 ("-infinity")
			}
			else
			{
				n->column = col;
				n->boardState[r][col] = P1;
				n->playerTurn = P2;
				n->value = -100000;
			}
			break;
		}
	}

	if (possibleMove)
	{
		for (int i = 0; i < MAXCOLS; i++)
			n->nextMoves[i] = nullptr;
		if (n->playerTurn == P2)
			n->value = std::max(n->value, evaluationFunction(n));
		else
			n->value = std::min(n->value, evaluationFunction(n));
		return n;
	}
	else
	{
		return nullptr;
	}
	//Make a move in the specified column of the new boardstate



}

void AI::updateBoardState(int playerTurn, int row, int col)
{
	currentBoardState[row][col] = playerTurn;
}

//Base implementation, check for win
int AI::evaluationFunction(Node* node)
{
	if (isWinner(P2, node)) //P2 is AI
		return 1000;
	else if (isWinner(P1, node))
		return -1000;
	else
		return 0;
}

int AI::minimax(Node* node)
{
	int value;
	int bestValue;
	//If terminating node, return value
	bool isTerminatingNode = true;
	for (int i = 0; i < MAXCOLS; i++)
	{
		if (node->nextMoves[i] != nullptr)
			isTerminatingNode = false;
	}
	if (isTerminatingNode == true)
		return node->value;


	if (node->depth == MAXDEPTH)
		return node->value;

	if (node->playerTurn == P2) //P1's turn to make a move or P2 has just made a move ("isMaximizingPlayer" = true);
	{
		bestValue = -100000;
		for (int i = 0; i < MAXCOLS; i++)
		{
			value = minimax(node->nextMoves[i]);
			bestValue = std::min(value, bestValue);
		}
		return bestValue;
	}
	else
	{
		bestValue = 100000;
		for (int i = 0; i < MAXCOLS; i++)
		{
			value = minimax(node->nextMoves[i]);
			bestValue = std::min(value, bestValue);
		}
		return bestValue;
	}
}

int AI::findBestMove()
{
	int value;
	int bestValue = 0;
	int columnToPick = 0;

	for (int i = 0; i < MAXCOLS; i++)
	{
		value = minimax(root->nextMoves[i]);
		if (value > bestValue)
		{
			bestValue = value;
			columnToPick = i;
		}
	}
	root->value = bestValue;
	std::cout << "\n P2 moved " << columnToPick + 1 << " \n";
	return columnToPick + 1;
}

void AI::printBoardState()
{

	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			std::cout << currentBoardState[i][j];
		}
		std::cout << "\n";
	}
}

void AI::printTreeHelper(Node* node)
{

	if (node == nullptr)
		return;

	for (int i = 0; i < MAXCOLS; i++)
	{
		printTreeHelper(node->nextMoves[i]);
	}
	std::cout << "\n";
	std::cout << "Depth: " << node->depth << " Value: " << node->value << " Player Turn: " << node->playerTurn << " | ";
	if (node->value != 0)
	{
		for (int i = 0; i < MAXROWS; i++)
		{
			std::cout << "\n";
			for (int j = 0; j < MAXCOLS; j++)
			{
				std::cout << node->boardState[i][j];
			}


		}
	}
}

void AI::printTree()
{
	printTreeHelper(root);
}
bool AI::isWinner(int player, Node* node)
{
	//Check cols
	for (int c = 0; c < MAXCOLS; c++)
	{
		for (int r = MAXROWS - 4; r >= 0; r--)
		{
			if (node->boardState[r][c] == player && node->boardState[r + 1][c] == player && node->boardState[r + 2][c] == player && node->boardState[r + 3][c] == player)
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

			if (node->boardState[r][c] == player && node->boardState[r][c + 1] == player && node->boardState[r][c + 2] == player && node->boardState[r][c + 3] == player)
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

			if (node->boardState[r][c] == player && node->boardState[r - 1][c + 1] == player && node->boardState[r - 2][c + 2] == player && node->boardState[r - 3][c + 3] == player)
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

			if (node->boardState[r][c] == player && node->boardState[r + 1][c + 1] == player && node->boardState[r + 2][c + 2] == player && node->boardState[r + 3][c + 3] == player)
			{
				return true;
			}
		}
	}

	return false;
}


