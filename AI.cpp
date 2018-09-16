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
#include <queue>
#include <random>

AI::AI()
{
	//Create the root node of the tree
	root = new Node();
	root->value = -1000;
	root->depth = 0;
	root->playerTurn = P2; //Turn P2
	root->column = 0;
	for (int i = 0; i < MAXCOLS; i++)
		root->nextMoves[i] = nullptr;

	//Board state is empty initially
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
	deleteNodes(root);
	//delete root; //Destructor is called when switching from two player to one player. It bugs out if this is deleted, unsure why
}

void AI::resetTree()
{
	deleteNodes(root);
}
void AI::deleteNodes(Node* node)
{
	if (node == nullptr) 
		return;

	//Terminating node = leaf of a tree (no children)
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
	//Depth first deletion (post order)
	for (int i = 0; i < MAXCOLS; i++)
	{
		deleteNodes(node->nextMoves[i]);
	}

}

void AI::generateTree()
{
	//Update root with the current state of the board
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
	//Limit the depth of the tree (higher depth = more moves considered by AI + more memory required)
	if (parent->depth >= MAXDEPTH)
		return;
	//The next level of the tree consists of the board states of all possible next moves that the next player can make
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
		//Check if there is an empty space in the specified column
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
			if (parent->playerTurn == P2) //P2 has played, now it's P1's turn
			{
				n->column = col;
				n->boardState[r][col] = P2; //P2 moved here in the previous turn
				n->playerTurn = P1;  //P1's turn
				n->value = 1000; //P1 is the minimizer. Worst case for the minimizer
			}
			else
			{
				n->column = col;
				n->boardState[r][col] = P1;
				n->playerTurn = P2;
				n->value = -1000;
			}
			break;
		}
	}
	if (possibleMove)
	{
		for (int i = 0; i < MAXCOLS; i++)
			n->nextMoves[i] = nullptr;
		return n;
	}
	else
		return nullptr;
}

void AI::updateBoardState(int playerTurn, int row, int col)
{
	currentBoardState[row][col] = playerTurn;
}


int AI::evaluationFunction(Node* node)
{
	//Always move to the winning spot
	if (isWinner(P2, node)) //P2 is AI
		return 1000;
	if (isWinner(P1, node))
		return -1000;
	//If AI cannot win, then maximize number of open threes
	return 0 + countThrees(P2, node) + countThrees(P1, node);
}

int AI::minimax(Node* node)
{
	if (node == nullptr)
		return 0;
	int value = evaluationFunction(node);
	node->value = value;

	//If player wins, return value
	int bestValue;
	if (value == 1000)
		return value;
	if (value == -1000)
		return value;

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

	if (node->playerTurn == P2) //P2's turn (P2 is maximizing player/AI)
	{
		bestValue = -1000; //P2's worst score

		//Evaluate next level in tree and pick the maximum score
		for (int i = 0; i < MAXCOLS; i++)
		{
			value = minimax(node->nextMoves[i]);
			bestValue = std::max(value, bestValue);
		}
		node->value = bestValue;
		return bestValue;
	}
	else //P1's turn (P1 is minimizing player)
	{
		bestValue = 1000; //P1's worst score

		//Evalulate next level in tree and pick the minimum score
		for (int i = 0; i < MAXCOLS; i++)
		{
			value = minimax(node->nextMoves[i]);
			bestValue = std::min(value, bestValue);
		}
		node->value = bestValue;
		return bestValue;
	}
}

bool AI::validMove(Node* node,int val)
{
	if (node != nullptr && currentBoardState[0][node->column] == 0 && node->value == val)
		return true;
	else
		return false;
}

int AI::findBestMove()
{
	int value;
	int bestValue = -1000;
	int columnToPick = 0;

	//P2 picks the maximum value from its children nodes
	for (int i = 0; i < MAXCOLS; i++)
	{
		if (root->nextMoves[i] != nullptr)
		{
			value = minimax(root->nextMoves[i]);
			if (value > bestValue)
			{
				bestValue = value;
				columnToPick = i;
			}
		}
	}

	//If there are multiple moves with the same value as the best value, pick a random move with that value
	columnToPick = rand() % 7;
	while (!validMove(root->nextMoves[columnToPick],bestValue))
	{
		columnToPick = rand() % 7;
	}
	root->value = bestValue;
	std::cout << "\n P2 moved " << columnToPick << " " << bestValue << " \n";
	//TODO: fix redo button for AI, add in menu option for AI, add in cursor functionality, update instructions
	return columnToPick + 1;
}

void AI::printBoardState(Node* node)
{
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			std::cout << node->boardState[i][j];
		}
		std::cout << "\n";
	}
}

void AI::printTreeHelper(Node* node)
{
	//Breadth first search
	if (node == nullptr)
		return;

	std::queue<Node*> q;
	q.push(node);
	while (!q.empty())
	{
		Node* temp = q.front();
		std::cout  << "Column" << temp->column + 1 << "\n";
		std::cout << "Node value: " << temp->value  << "\n";
		printBoardState(temp);
		q.pop();
		for (int i = 0; i < MAXCOLS; i++)
		{
			if (temp->nextMoves[i] != nullptr)
				q.push(temp->nextMoves[i]);
		}
	}
	
	
	/*
	Depth First Search
	for (int i = 0; i < MAXCOLS; i++)
	{
		printTreeHelper(node->nextMoves[i]);
	}
	*/
	/*
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
	*/
}

void AI::printTree()
{
	//For debugging purposes
	std::cout << "New Turn \n";
	printTreeHelper(root);
}
int AI::countThrees(int player, Node* node)
{
	//Count number of open threes
	//reset back to original board
	for (int c = 0; c < MAXCOLS; c++)
	{
		for (int r = 0; r < MAXROWS; r++)
		{
			if (node->boardState[r][c] == -1)
				node->boardState[r][c] = 0;
		}
	}
	int count = 0;
	//Check cols
	for (int c = 0; c < MAXCOLS; c++)
	{
		for (int r = MAXROWS - 4; r >= 0; r--)
		{
			if (node->boardState[r][c] == player && node->boardState[r + 1][c] == player && node->boardState[r + 2][c] == player && node->boardState[r + 3][c] == 0)
			{
				node->boardState[r + 3][c] = -1;
				count += 1;
				break;
			}
		}
	}
	//Check rows
	for (int r = 0; r < MAXROWS; r++)
	{
		for (int c = 0; c < MAXCOLS - 3; c++)
		{
			if (node->boardState[r][c] == 0 && node->boardState[r][c + 1] == player && node->boardState[r][c + 2] == player && node->boardState[r][c + 3] == player)
			{
				count += 1;
				node->boardState[r][c] = -1;
			}
			if (node->boardState[r][c] == player && node->boardState[r][c + 1] == 0 && node->boardState[r][c + 2] == player && node->boardState[r][c + 3] == player)
			{
				count += 1;
				node->boardState[r][c+1] = -1;
			}
			if (node->boardState[r][c] == player && node->boardState[r][c + 1] == player && node->boardState[r][c + 2] == 0 && node->boardState[r][c + 3] == player)
			{
				count += 1;
				node->boardState[r][c+2] = -1;
			}
			if (node->boardState[r][c] == player && node->boardState[r][c + 1] == player && node->boardState[r][c + 2] == player && node->boardState[r][c + 3] == 0)
			{
				count += 1;
				node->boardState[r][c+3] = -1;
			}
		}
	}
	//Check forward diagonals
	for (int r = MAXROWS - 1; r >= 3; r--)
	{
		for (int c = 0; c < MAXCOLS - 3; c++)
		{

			if (node->boardState[r][c] == 0 && node->boardState[r - 1][c + 1] == player && node->boardState[r - 2][c + 2] == player && node->boardState[r - 3][c + 3] == player)
			{
				count += 1;
				node->boardState[r][c] = -1;
			}
			if (node->boardState[r][c] == player && node->boardState[r - 1][c + 1] == 0 && node->boardState[r - 2][c + 2] == player && node->boardState[r - 3][c + 3] == player)
			{
				count += 1;
				node->boardState[r-1][c+1] = -1;
			}
			if (node->boardState[r][c] == player && node->boardState[r - 1][c + 1] == player && node->boardState[r - 2][c + 2] == 0 && node->boardState[r - 3][c + 3] == player)
			{
				count += 1;
				node->boardState[r-2][c+2] = -1;
			}
			if (node->boardState[r][c] == player && node->boardState[r - 1][c + 1] == player && node->boardState[r - 2][c + 2] == player && node->boardState[r - 3][c + 3] == 0)
			{
				count += 1;
				node->boardState[r-3][c+3] = -1;
			}
		}
	}
	//Check back diagonals
	for (int r = 0; r < MAXROWS - 3; r++)
	{
		for (int c = 0; c < MAXCOLS - 3; c++)
		{

			if (node->boardState[r][c] == 0 && node->boardState[r + 1][c + 1] == player && node->boardState[r + 2][c + 2] == player && node->boardState[r + 3][c + 3] == player)
			{
				count += 1;
				node->boardState[r][c] = -1;
			}
			if (node->boardState[r][c] == player && node->boardState[r + 1][c + 1] == 0 && node->boardState[r + 2][c + 2] == player && node->boardState[r + 3][c + 3] == player)
			{
				count += 1;
				node->boardState[r+1][c+1] = -1;
			}
			if (node->boardState[r][c] == player && node->boardState[r + 1][c + 1] == player && node->boardState[r + 2][c + 2] == 0 && node->boardState[r + 3][c + 3] == player)
			{
				count += 1;
				node->boardState[r+2][c+2] = -1;
			}
			if (node->boardState[r][c] == player && node->boardState[r + 1][c + 1] == player && node->boardState[r + 2][c + 2] == player && node->boardState[r + 3][c + 3] == 0)
			{
				count += 1;
				node->boardState[r+3][c+3] = -1;
			}
		}
	}
	//Evaluate board based on number of open threes from player
	if (player == P2)
		return count * 20;
	else return count * (-20);
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


