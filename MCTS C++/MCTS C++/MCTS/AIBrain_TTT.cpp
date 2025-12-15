#include "AIBrain_TTT.h"

#include "MCTS_Reasoner.h"
void AIBrain_TTT::Update()
{
	// call the reasoner to get the selected action
	brainReasoner->Update();

}

bool AIBrain_TTT::Init(AIConstructorBase& _constructor, AIActorBase& _actorContext)
{
	brainBlackboard = new AIBrainBlackboard();
	brainBlackboard->Init(_actorContext);



	brainReasoner = new MCTS_Reasoner();

	brainReasoner->SetOptions(_constructor);

	brainReasoner->Init("MCTS Reasoner", *brainBlackboard);



	return true;

}