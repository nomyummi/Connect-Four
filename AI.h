#ifndef AI_H
#define AI_H

#include "Chip.h"
#include "constants.h"
struct Move
{
	int bestValue;
	int bestColumn;
};
struct Node
{
	int boardState[MAXROWS][MAXCOLS];
	int depth;
	int playerTurn;
	int value;
	int column;
	Node* nextMoves[MAXCOLS];
};
class AI
{
public:
	AI();
	~AI();
	int minimax(Node* node); //return col number
	int findBestMove();
	bool validMove(Node* node, int val);
	int evaluationFunction(Node* node);
	void updateBoardState(int playerTurn, int row, int col);
	bool isWinner(int player, Node* node);
	int countThrees(int player, Node* node);
	void generateTree(); //Of MAXDEPTH
	void addNodestoTree(Node* parent);
	Node* newNode(Node* parent, int col);
	void deleteNodes(Node* node);
	void resetTree();
	void printBoardState(Node* node); //for debugging
	void printTree();
	void printTreeHelper(Node* node); //for debugging
private:
	Node * root;
	int currentBoardState[MAXROWS][MAXCOLS];
};
#endif