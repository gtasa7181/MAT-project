#include "MCTS_Reasoner.h"
#include <iostream>


bool MCTS_Reasoner::Init(std::string _id, AIBrainBlackboardBase& _context)
{

	actorBlackboard = &_context;
	reasonerID = _id;
	Global_Blackboard = AIBlackboard_Global::getInstance();
	return true;
}


void MCTS_Reasoner::Think()
{

	// create a new MCTS TicTacToeNode which will be used to create the tree
	rootNode = new TicTacToeNode();

	//update blackboard for status tracking
	GameState _mainState = Global_Blackboard->GetValueGameState("GameState");
	BOARD_SQUARE_STATE _playerMarker = (BOARD_SQUARE_STATE)Global_Blackboard->GetValueInt("PlayerMarker");
	BOARD_SQUARE_STATE _aiMarker = (BOARD_SQUARE_STATE)Global_Blackboard->GetValueInt("AIMarker");
	int _maxRuns = (BOARD_SQUARE_STATE)Global_Blackboard->GetValueInt("MaxRuns");


	//set the active player and the current main game state
	rootNode->setActivePlayer(_aiMarker);
	rootNode->setGameState(_mainState);


	// var to track the number of iterations
	int runCount = 0;


	// Apply the MCTS algorithm to determine the AI's move
	// - will run for maximum of MAX_RUNS iterations
	do {

		// Select - choose a node that will be expanded
		TicTacToeNode* selectedNode = rootNode->Select();

		// Expand - expand the tree from this node
		TicTacToeNode* expandedNode = selectedNode->Expand();

		// expandedNode will be NULL if it's a leaf node and simulation is not possible
		if (!expandedNode == NULL)
		{
			// Simulate - Simulate to an end point, starting with aiMarker turn.
			expandedNode->Simulate(_aiMarker);

			// CalcResult will trigger the back propagation of the result to the root node
			expandedNode->CalcResult();
		}

		runCount++;

	} while (runCount < _maxRuns);


	// perform the action - find child node with highest ranking
	TicTacToeNode* highestChild = rootNode->FindHighestRankingChild();
	GameAction bestAction = highestChild->getGameState().gameAction;

	// only a single option - make a move
	selectedOption = options[0];

	// update the blackboard with the selected action (Connect 4 only needs column)
	actorBlackboard->EditValueInt("Col", bestAction.column);


	//track data for output
	std::vector<int> _availableMoves = _mainState.GetPossibleMoves();


	// track total wins in children
	std::vector<int> _nodeWins;
	std::vector<int> _nodeVisits;
	std::vector<int> _nodeActionRow;
	std::vector<int> _nodeActionCol;

	for (TicTacToeNode* child : rootNode->getBranches())
	{
		_nodeWins.push_back(child->getWins());
		_nodeVisits.push_back(child->getVisits());
		// For Connect 4, we only track column (no row needed due to gravity)
		_nodeActionCol.push_back(child->getGameState().gameAction.column);
	}


	Global_Blackboard->EditValueIntVec("AINodeWins", _nodeWins);
	Global_Blackboard->EditValueIntVec("AINodeVisits", _nodeVisits);
	// Removed AINodeRow since Connect 4 doesn't need it
	Global_Blackboard->EditValueIntVec("AINodeCol", _nodeActionCol);


	delete rootNode;
}






// - AI Framework Code -

void MCTS_Reasoner::SetOptions(AIConstructorBase& _constructor)
{
	std::vector<AIOptionBase*> _options = _constructor.GetOptionVector();

	for (int i = 0; i < _options.size(); i++)
	{
		AIOption* o = new AIOption();
		o->Init(_options[i]->GetOptionID(), _options[i]->GetOptionAction(), _options[i]->GetPriority());

		for (int j = 0; j < _options[i]->GetOptionConsiderations().size(); j++)
		{
			AIConsideration* c = new AIConsideration();
			c->Init(_options[i]->GetOptionConsiderations()[j]->GetID(), _options[i]->GetOptionConsiderations()[j]->GetRule());
			o->AddOptionConsideration(c);
		}

		options.push_back(o);
	}
}
