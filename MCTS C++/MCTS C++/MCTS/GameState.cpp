#include "GameState.h"


#include <iostream>
GameState::GameState()
{

}

/**
* setAndApplyAction will update the game state with the provided action and update the game board
*/
void GameState::SetAndApplyAction(GameAction newAction)
{
	gameAction = newAction;

	// update the board
	gameBoard.board[newAction.row][newAction.col] = newAction.playerMove;

}

/**
* setAndApplyAction will return all possible moves from the stored board state
*/
std::vector<std::pair<int, int>> GameState::GetPossibleMoves()
{
	std::vector<std::pair<int, int>> possibleMoves;

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			if (gameBoard.board[row][col] == BOARD_SQUARE_STATE::NONE)
			{
				// if currently empty,add to possible moves
				possibleMoves.push_back(std::make_pair(row, col));
			}
		}
	}

	return possibleMoves;
}

/**
* checkWin will check if the stored board state is a winning one for either marker and return the winnig marker
* If there is no winning marker, NONE will be returned
*/

BOARD_SQUARE_STATE GameState::CheckWin()
{

	// check each row for a match
	for (int i = 0; i < 3; i++)
	{
		if (gameBoard.board[i][0] != BOARD_SQUARE_STATE::NONE &&
			gameBoard.board[i][0] == gameBoard.board[i][1] &&
			gameBoard.board[i][0] == gameBoard.board[i][2])
		{
			return gameBoard.board[i][0];
		}

	}

	// Check each column
	for (int i = 0; i < 3; i++)
	{
		if (gameBoard.board[0][i] != BOARD_SQUARE_STATE::NONE &&
			gameBoard.board[0][i] == gameBoard.board[1][i] &&
			gameBoard.board[0][i] == gameBoard.board[2][i])
		{
			return gameBoard.board[0][i];
		}
	}

	// Check first diagonal
	if (gameBoard.board[0][0] != BOARD_SQUARE_STATE::NONE &&
		gameBoard.board[0][0] == gameBoard.board[1][1] &&
		gameBoard.board[0][0] == gameBoard.board[2][2])
	{
		return gameBoard.board[0][0];
	}

	// Check second diagonal
	if (gameBoard.board[0][2] != BOARD_SQUARE_STATE::NONE &&
		gameBoard.board[0][2] == gameBoard.board[1][1] &&
		gameBoard.board[0][2] == gameBoard.board[2][0])
	{
		return gameBoard.board[0][2];
	}

	// If no-one wins return the default state
	return BOARD_SQUARE_STATE::NONE;
}