#pragma once


/*
GameBoard defines enums and structures for the Connect Four game
    - BOARD_SQUARE_STATE used for markers
    - GameBoard to represent a 6x7 game board  
    - GameAction to represent an action taken by a player (i.e. place marker in column)
*/


// define enum's and structs used in the game

// BOARD_SQUARE_STATE used for markers
enum BOARD_SQUARE_STATE { NONE, CIRCLE, CROSS };

// GameBoard to represent a 6x7 game board
struct GameBoard
{
	BOARD_SQUARE_STATE board[6][7];  // 6 rows x 7 columns for Connect 4
};


//GameAction to represent an action taken by a player (i.e. place marker in column)
struct GameAction
{
	int column;  // Only need column for Connect 4 (gravity handles row)
	BOARD_SQUARE_STATE playerMove;

public:
	GameAction(int newCol, BOARD_SQUARE_STATE move)
	{
		column = newCol;
		playerMove = move;
	}

	GameAction()
	{
		playerMove = BOARD_SQUARE_STATE::NONE;
		column = -1;
	}
};
