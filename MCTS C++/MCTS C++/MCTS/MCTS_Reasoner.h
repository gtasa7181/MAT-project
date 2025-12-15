#pragma once
#include "../AI_Framework/framework.h"
#include "TicTacToeNode.h"

class MCTS_Reasoner :
    public AIReasonerBase
{
public:
	bool Init(std::string _id, AIBrainBlackboardBase& _context) override;
	void Think() override;
	void SetOptions(AIConstructorBase& _constructor) override;

private:
	TicTacToeNode* rootNode;

	AIBlackboard_Global *Global_Blackboard;
};

