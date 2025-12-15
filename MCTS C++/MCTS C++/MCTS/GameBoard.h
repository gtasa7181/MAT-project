#pragma once

/*
GameBoard defines enums and structures for the Tic Tac Toe game
 - BOARD_SQUARE_STATE used for markers
 - GameBoard to represent a 3x3 game board
 - GameAction to represent an action taken by a player (i.e. place marker X in row 1, col 2)

*/

// define enum's and structs used in the game

// BOARD_SQUARE_STATE used for markers
enum BOARD_SQUARE_STATE { NONE, CIRCLE, CROSS };

// GameBoard to represent a 3x3 game board
struct GameBoard
{
	BOARD_SQUARE_STATE board[3][3]{ { BOARD_SQUARE_STATE::NONE } };
};


//GameAction to represent an action taken by a player (i.e. place marker X in row 1, col 2)
struct GameAction
{
	int row, col;
	BOARD_SQUARE_STATE playerMove;

public:
	GameAction(int newRow, int newCol, BOARD_SQUARE_STATE move)
	{
		row = newRow;
		col = newCol;
		playerMove = move;
	}

	GameAction()
	{
		playerMove = BOARD_SQUARE_STATE::NONE;
		row = 0;
		col = 0;
	}


};

