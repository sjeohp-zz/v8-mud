#ifndef Mud_Chess_h
#define Mud_Chess_h

#include <iostream>
#include <string>

using namespace std;

typedef enum PieceColor {
	BLACK,
	WHITE
} PieceColor;

typedef enum PieceType {
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
} PieceType;

typedef struct Piece {
	
} Piece;

typedef struct Square {
	int empty;
	PieceColor pieceColor;
	PieceType pieceType;
} Square;

typedef Square** Board;

int GetBoardDimen();
Board* GetSharedBoard();
Board* createBoard(int dimen);
void resetBoard(PieceColor playingAsColor);
std::string strForSquare(Square* square);
int updateBoardWithMove(string mv);

#endif