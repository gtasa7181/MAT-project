#include "AIConstructor_MCTS.h"
#include "AIActor_TicTacToe.h"

bool AIConstructor_MCTS::Init() {

	method = AIConstructorBase::Method::MCTS;
	return true;
}



/*
DefineActions creates all the possible actions that the AI can perform
*/
void AIConstructor_MCTS::DefineActions()
{



	//Define the function for making a move
	auto makeMoveFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_TicTacToe* actor = static_cast<AIActor_TicTacToe*>(bb.GetActorContext());

		int _row = actor->GetBBValueInt("Row");
		int _col = actor->GetBBValueInt("Col");

		actor->MakeMove(_col);

		return ActionStatus::ACTION_SUCCESS;
		};


	// Link the function to an identifier
	AddActionByName("ActionMakeMove", makeMoveFunction);


}

/*
   DefineConsiderations creates all Considerations that will be linked to Options
// Options will only be selected if the Condition is true
*/
void AIConstructor_MCTS::DefineConsiderations()
{


}


/*
* DefineOptions links the Options with Considerations and Actions
*/
void AIConstructor_MCTS::DefineOptions()
{

	// Single Option for TicTacToe
	AddOptionByName("MakeMove", "ActionMakeMove");

}





// ** Helper Funcitons **
// these link to the AI Framework and should not be edited




void AIConstructor_MCTS::AddActionByName(std::string _actionName, std::function<ActionStatus(AIBrainBlackboardBase&)> _function)
{

	//Define an action, and init with the function which will be called when the action is selected
	AIAction* _action = new AIAction();
	_action->Init(_actionName, _function);
	actions.insert({ _actionName, _action });
}

void AIConstructor_MCTS::AddConsiderationByName(std::string _actionName, std::function<bool(AIBrainBlackboardBase&)> _function)
{

	//Define an action, and init with the function which will be called when the action is selected
	AIConsideration* _consideration = new AIConsideration();
	_consideration->Init(_actionName, _function);
	considerations.insert({ _actionName, _consideration });
}

void AIConstructor_MCTS::AddOptionByName(std::string _optionName, std::string _actionName, int _priority)
{
	// create an option for action
	AIOption* _option = new AIOption();
	_option->Init(_optionName, actions[_actionName], _priority);
	options.insert({ _optionName, _option });
}
