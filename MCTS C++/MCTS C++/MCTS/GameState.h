#pragma once

#include "GameBoard.h"
#include <vector>

/*
* GameState class represent current state of a Connect Four game
*/
class GameState
{
public:
	GameBoard gameBoard;
	GameAction gameAction;

	GameState();

	// state changing functions
	void SetAndApplyAction(GameAction newAction);
	
	// status checking
	std::vector<int> GetPossibleMoves();  // Returns list of valid columns
	BOARD_SQUARE_STATE CheckWin();
	
private:
	int GetNextAvailableRow(int col);  // Helper for gravity
};
