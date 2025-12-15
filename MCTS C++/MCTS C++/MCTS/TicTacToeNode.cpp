#include "TicTacToeNode.h"
#include <iostream>
#include "GameBoard.h"
#include <AIBlackboard_Global.h>
#include "AIRandom_Global.h"
#include <cmath>

// general functions
BOARD_SQUARE_STATE getOppositeMove(BOARD_SQUARE_STATE state)
{
	// Flip to the other player
	if (state == BOARD_SQUARE_STATE::CIRCLE)
	{
		return BOARD_SQUARE_STATE::CROSS;
	}
	else if (state == BOARD_SQUARE_STATE::CROSS)
	{
		return BOARD_SQUARE_STATE::CIRCLE;
	}
	else
		return BOARD_SQUARE_STATE::NONE;
}

// class functions
TicTacToeNode::TicTacToeNode()
{
	isEndState = false;
	parent = nullptr;
	wins = 0;
	visits = 0;
	activePlayer = BOARD_SQUARE_STATE::NONE;
}

TicTacToeNode::~TicTacToeNode()
{
	// parent will be deleted by its parent so we don't need to clean it up here
	parent = nullptr;

	for (auto branch : branches)
	{
		delete branch;
	}
	branches.clear();
	availableMoves.clear();
}

/*
* Select - will perform the Select step of the MCTS algorithm
* will select down a path according to the UCT selection policy until leaf is found
*/
TicTacToeNode* TicTacToeNode::Select()
{
	if (availableMoves.size() > 0)
	{
		// still unexplored branches, explore
		return this;
	}

	if (isEndState)
	{
		// leaf node, return and will be handled by Reasoner
		return this;
	}

	// **UCT-BASED SELECTION** - Choose best child using Upper Confidence Bound
	double bestUCT = -999999.0;
	TicTacToeNode* bestChild = nullptr;
	
	const double explorationParameter = sqrt(2.0);  // Standard UCT exploration parameter
	
	for (TicTacToeNode* child : branches)
	{
		if (child->getVisits() == 0)
		{
			// Unvisited nodes get maximum priority
			return child->Select();
		}
		
		// Calculate UCT value: exploitation + exploration
		double exploitation = (double)child->getWins() / (double)child->getVisits();
		double exploration = explorationParameter * sqrt(log((double)visits) / (double)child->getVisits());
		double uctValue = exploitation + exploration;
		
		if (uctValue > bestUCT)
		{
			bestUCT = uctValue;
			bestChild = child;
		}
	}
	
	if (bestChild != nullptr)
	{
		return bestChild->Select();
	}
	
	return this;
}

/*
* Expand - will perform the Expansion step of the MCTS algorithm
* will add a new node by randomly selecting from all possible moves
*/
TicTacToeNode* TicTacToeNode::Expand()
{
	if (isEndState)
	{
		return NULL; // if this is game end state, we cannot expand
	}
	else
	{
		// pick a random move from the unexplored ones
		int randomMove = rand() % availableMoves.size();
		int columnToPlay = availableMoves[randomMove];

		// delete the selected move from vector
		availableMoves.erase(availableMoves.begin() + randomMove);

		// create a new child node 
		TicTacToeNode* childNode = new TicTacToeNode();
		childNode->setParent(this);

		// flip the marker to other player
		BOARD_SQUARE_STATE nextTurn = getOppositeMove(activePlayer);
		childNode->setActivePlayer(nextTurn);

		// create the new world state
		GameState newWorldState;
		newWorldState.gameBoard = this->worldState.gameBoard;
		GameAction newAction(columnToPlay, nextTurn);

		// apply the move to the board and set the child's state
		newWorldState.SetAndApplyAction(newAction);
		childNode->setGameState(newWorldState);

		// add the newly created node as a child of current node 
		branches.push_back(childNode);

		// return node, for simulation
		return childNode;
	}
}

/*
* Simulate - will perform the Simulate step of the MCTS algorithm
* will play random moves until an end state is found, and set the simulationWinner
*/
void TicTacToeNode::Simulate(BOARD_SQUARE_STATE startingTurn)
{
	// Make copy of Game State
	GameState simulationState = worldState;
	BOARD_SQUARE_STATE currentPlayer = startingTurn;
	
	// WHILE game state NOT end of game:
	while (true)
	{
		// Check for win
		BOARD_SQUARE_STATE winner = simulationState.CheckWin();
		if (winner != BOARD_SQUARE_STATE::NONE)
		{
			simulationWinner = winner;
			return;
		}
		
		// Get all possible Moves
		std::vector<int> possibleColumns = simulationState.GetPossibleMoves();
		
		// Check for draw (no moves left)
		if (possibleColumns.size() == 0)
		{
			simulationWinner = BOARD_SQUARE_STATE::NONE;  // Draw
			return;
		}
		
		// Randomly select one Move for active player
		int randomIndex = AIRandom_Global::getInstance()->GetRandomValue((int)possibleColumns.size());
		int selectedColumn = possibleColumns[randomIndex];
		
		// Apply Move to Game State
		GameAction simulatedAction(selectedColumn, currentPlayer);
		simulationState.SetAndApplyAction(simulatedAction);
		
		// Set active player for next turn
		currentPlayer = getOppositeMove(currentPlayer);
	}
}

/*
* CalcResult will determining if the AI won or lost, and trigger the back propagation
*/
void TicTacToeNode::CalcResult()
{
	BOARD_SQUARE_STATE _aiMarker = (BOARD_SQUARE_STATE)AIBlackboard_Global::getInstance()->GetValueInt("AIMarker");
	if (simulationWinner == _aiMarker)
	{
		Backpropagate(1);
	}
	else if (simulationWinner == BOARD_SQUARE_STATE::NONE)
	{
		Backpropagate(0);
	}
	else
	{
		Backpropagate(-1);
	}
}

/*
* Backpropagate - will perform the Backpropagate step of the MCTS algorithm
* the result will be returned back up the tree branches to the root node
*/
void TicTacToeNode::Backpropagate(int result)
{
	// Increment visits
	visits++;
	
	// Increment wins depending on result (1 = AI Win, 0 = Draw, -1 = Opponent Win)
	// We store wins from the perspective of the AI, so flip result for opponent nodes
	if (result == 1)
	{
		wins++;
	}
	else if (result == -1)
	{
		// Opponent win means this node led to a loss
		// Don't increment wins
	}
	else
	{
		// Draw - give partial credit
		wins += 0.5;
	}
	
	// If the node has a parent, call parent->Backpropagate with the results
	if (parent != nullptr)
	{
		parent->Backpropagate(result);
	}
}

// status tracking and updates
/**
* setGameState will set the game state for the node, as well as calling generatePossibleMoves to update status
*/
void TicTacToeNode::setGameState(GameState newWorldState)
{
	worldState = newWorldState;
	generatePossibleMoves();
}

/**
* generatePossibleMoves will calculate the possible moves from this node and set the isEndState variable appropriately
* should only be called once when node's game state is defined
*/
void TicTacToeNode::generatePossibleMoves()
{
	availableMoves = worldState.GetPossibleMoves();
	if (availableMoves.size() == 0)
		isEndState = true;
	if (worldState.CheckWin() != BOARD_SQUARE_STATE::NONE)
		isEndState = true;
}

/*
* FindHighestRankingChild will determine which of this node's children has the highest metric
*/
TicTacToeNode* TicTacToeNode::FindHighestRankingChild()
{
	if (branches.size() == 0)
		return NULL;

	float maxWins = 0;
	int maxIndex = 0;

	for (int i = 0; i < branches.size(); i++)
	{
		float nodeWinRate = ((float)branches[i]->getWins()) / branches[i]->getVisits();
		if (nodeWinRate > maxWins)
		{
			maxIndex = i;
			maxWins = nodeWinRate;
		}
	}

	return branches[maxIndex];
}

/*
* resetNode will return the node to its default values
*/
void TicTacToeNode::resetNode()
{
	branches.clear();
	wins = 0;
	visits = 0;
	availableMoves.clear();
}
