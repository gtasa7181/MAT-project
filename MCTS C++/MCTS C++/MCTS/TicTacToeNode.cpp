#include "TicTacToeNode.h"
#include <iostream>
#include "GameBoard.h"
#include <AIBlackboard_Global.h>
#include "AIRandom_Global.h"


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
*  will select down a path according to the selection policy until leaf is found
*/
TicTacToeNode* TicTacToeNode::Select()
{

	if (availableMoves.size() > 0)
	{
		// still unexplored branches, explore
		return this;
	}

	if (isEndState )
	{
		// leaf node, return and will be handled by Reasoner
		return this;
	}



	// TASK TODO - replace below below code with UCT based selection


	int randomBranch = rand() % branches.size();
	return branches[randomBranch]->Select();



}

/*
* Expand  - will perform the Expansion step of the MCTS algorithm
* will add a new node by randomly selecting from all possible moves
*/
TicTacToeNode* TicTacToeNode::Expand()
{

	if (isEndState)
	{
		//std::cout << "Cannont expand - leaf " << std::endl;
		return NULL; // if this is game end state, we cannot expand
	}
	else
	{
		// pick a random move fro, the unexplored ones
		int randomMove = rand() % availableMoves.size();
		std::pair<int, int> move = availableMoves[randomMove];

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
		GameAction newAction(move.first, move.second, nextTurn);

		// apply the move to the board and set the child's state
		newWorldState.SetAndApplyAction(newAction);
		childNode->setGameState(newWorldState);

		// add the newly created node as a child of current node		
		branches.push_back(childNode);

		// reutrn node, for simulation
		return childNode;
	}

}
#/*
* Simulate  - will perform the Simulate step of the MCTS algorithm
* will play random moves until a end state is found, and set the simulationWinner
*/
void TicTacToeNode::Simulate(BOARD_SQUARE_STATE startingTurn)
{



	//TASK TODO - replace below code with the simulation functionality
	// - ensure you set 'simulationWinner' in this function
	// 
	// # PSEUDOCODE
	// 
	//	Make copy of Game State
	//	Set active player
	//	WHILE game state NOT end of game :
	//		Get all possible Moves
	//		Randomly select one Move for active player
	//		Apply Move to Game State
	//		Set active player for next turn
	//	
	// Set simulationWinner to the simualation result



	// Class/Functions of use:

	//	Class			Function			Description
	//	TicTacToeNode	GetGameState()		Will return the GameState for the current Node
	//	TicTacToeNode	GetOppositeMove()	Will return the opposite marker than the one provided.E.g. if X is provided, O will be returned.
	//	GameState		CheckWin()			If the game state is a winning state, will return BOARD_SQUARE_STATE of the winner, otherwise ‘NONE’
	//	GameState		GetPossibleMoves()	Will return a vector of possible moves from the game state.Each move is a std::pair<int, int> representing <Row, Col>
	//	GameAction		GameAction()		A new GameAction can be created using the GameAction constructor, with parameters Row, Col, Marker
	//	AIRandom_Global	GetRandom01()		Will return a float between 0 and 1
	//	AIRandom_Global	GetRandomValue()	Will return a value between 0 and the value provided




	if (AIRandom_Global::getInstance()->GetRandom01() >= 0.5f)
	{
		simulationWinner = BOARD_SQUARE_STATE::CIRCLE;
	}
	else
	{
		simulationWinner = BOARD_SQUARE_STATE::CROSS;
	}




}



/*
* CalcResult will determing if the AI won or lost, and trigger the back propagation
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
* Backpropagate  - will perform the Backpropagate step of the MCTS algorithm
* the result will be returned back up the tree branches to the root node
*/

void TicTacToeNode::Backpropagate(int result)
{

	//TASK TODO

	// incerement visits
	// increment wins depending on result (1 = AI Win, 0 = Draw, -1 = Opponent (Player) Win)

	// if the node has a parent, call parent-> Backpropagate with the results
	// the node will be NULL if there is no parment




}




// status tracking and updates



/**
*  setGameState will set the game state for the node, as well as calling generatePossibleMoves to update status
*/

void TicTacToeNode::setGameState(GameState newWorldState)
{
	worldState = newWorldState;
	generatePossibleMoves();
}


/**
*  generatePossibleMoves will calculate the possible mvoes from this node and set the isEndState variable appropriately
*  should only be called once when node's game state is defined
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

#/*
* resetNode will return the node to its default values
*/

void TicTacToeNode::resetNode()
{
	branches.clear();
	wins = 0;
	visits = 0;
	availableMoves.clear();
}