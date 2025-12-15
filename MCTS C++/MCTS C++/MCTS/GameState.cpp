#include "GameState.h"
#include <iostream>

GameState::GameState()
{
	// Initialize empty 6x7 board
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 7; col++)
		{
			gameBoard.board[row][col] = BOARD_SQUARE_STATE::NONE;
		}
	}
}

/**
* setAndApplyAction will update the game state with the provided action and update the game board
*/
void GameState::SetAndApplyAction(GameAction newAction)
{
	gameAction = newAction;
	
	// Find the lowest available row in the column (gravity)
	int row = GetNextAvailableRow(newAction.column);
	
	if (row != -1)
	{
		// Place the piece
		gameBoard.board[row][newAction.column] = newAction.playerMove;
	}
}

/**
* GetNextAvailableRow returns the lowest empty row in a column (-1 if column is full)
*/
int GameState::GetNextAvailableRow(int col)
{
	// Start from bottom row (5) and go up
	for (int row = 5; row >= 0; row--)
	{
		if (gameBoard.board[row][col] == BOARD_SQUARE_STATE::NONE)
		{
			return row;
		}
	}
	return -1;  // Column is full
}

/**
* GetPossibleMoves will return all possible moves (available columns) from the stored board state
*/
std::vector<int> GameState::GetPossibleMoves()
{
	std::vector<int> possibleMoves;
	
	// Check each column to see if it has space
	for (int col = 0; col < 7; col++)
	{
		// Check if top row of this column is empty
		if (gameBoard.board[0][col] == BOARD_SQUARE_STATE::NONE)
		{
			possibleMoves.push_back(col);
		}
	}
	
	return possibleMoves;
}


/**
* CheckWin will check if the stored board state is a winning one for either marker and return the winner marker
* If there is no winning marker, NONE will be returned
*/
BOARD_SQUARE_STATE GameState::CheckWin()
{
	// Check horizontal wins
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col <= 3; col++)
		{
			if (gameBoard.board[row][col] != BOARD_SQUARE_STATE::NONE &&
				gameBoard.board[row][col] == gameBoard.board[row][col+1] &&
				gameBoard.board[row][col] == gameBoard.board[row][col+2] &&
				gameBoard.board[row][col] == gameBoard.board[row][col+3])
			{
				return gameBoard.board[row][col];
			}
		}
	}
	
	// Check vertical wins
	for (int col = 0; col < 7; col++)
	{
		for (int row = 0; row <= 2; row++)
		{
			if (gameBoard.board[row][col] != BOARD_SQUARE_STATE::NONE &&
				gameBoard.board[row][col] == gameBoard.board[row+1][col] &&
				gameBoard.board[row][col] == gameBoard.board[row+2][col] &&
				gameBoard.board[row][col] == gameBoard.board[row+3][col])
			{
				return gameBoard.board[row][col];
			}
		}
	}
	
	// Check diagonal wins (bottom-left to top-right)
	for (int row = 3; row < 6; row++)
	{
		for (int col = 0; col <= 3; col++)
		{
			if (gameBoard.board[row][col] != BOARD_SQUARE_STATE::NONE &&
				gameBoard.board[row][col] == gameBoard.board[row-1][col+1] &&
				gameBoard.board[row][col] == gameBoard.board[row-2][col+2] &&
				gameBoard.board[row][col] == gameBoard.board[row-3][col+3])
			{
				return gameBoard.board[row][col];
			}
		}
	}
	
	// Check diagonal wins (top-left to bottom-right)
	for (int row = 0; row <= 2; row++)
	{
		for (int col = 0; col <= 3; col++)
		{
			if (gameBoard.board[row][col] != BOARD_SQUARE_STATE::NONE &&
				gameBoard.board[row][col] == gameBoard.board[row+1][col+1] &&
				gameBoard.board[row][col] == gameBoard.board[row+2][col+2] &&
				gameBoard.board[row][col] == gameBoard.board[row+3][col+3])
			{
				return gameBoard.board[row][col];
			}
		}
	}
	
	return BOARD_SQUARE_STATE::NONE;
}
