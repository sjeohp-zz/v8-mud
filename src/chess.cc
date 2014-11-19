#include <stdio.h>
#include <stdlib.h>
#include "chess.h"


int dimen = 8;
Board* CHESSBOARD;

int GetBoardDimen() {
	return dimen;
}

Board* GetSharedBoard() {
	if (CHESSBOARD == NULL) {
		CHESSBOARD = createBoard(dimen);
	}
	return CHESSBOARD;
}

Board* createBoard(int dimen) {
	Board* board = (Square***)malloc(dimen*sizeof(Square**));
	for (int i = 0; i < dimen; ++i) {
		board[i] = (Square**)malloc(dimen*sizeof(Square*));
		for (int j = 0; j < dimen; ++j) {
			Square* square = (Square*)malloc(dimen*sizeof(Square));
			square->empty = 1;
			board[i][j] = square;

		}
	}
	return board;
}

void resetBoard(PieceColor playingAsColor) {

	Board* board = GetSharedBoard();
	/* opponent nobles */

	board[0][0]->empty = 0;
	board[0][0]->pieceColor = (PieceColor)!playingAsColor;
	board[0][0]->pieceType = ROOK;

	board[0][1]->empty = 0;
	board[0][1]->pieceColor = (PieceColor)!playingAsColor;
	board[0][1]->pieceType = KNIGHT;

	board[0][2]->empty = 0;
	board[0][2]->pieceColor = (PieceColor)!playingAsColor;
	board[0][2]->pieceType = BISHOP;

	board[0][3]->empty = 0;
	board[0][3]->pieceColor = (PieceColor)!playingAsColor;
	board[0][3]->pieceType = playingAsColor == BLACK ? KING : QUEEN;

	board[0][4]->empty = 0;
	board[0][4]->pieceColor = (PieceColor)!playingAsColor;
	board[0][4]->pieceType = playingAsColor == BLACK ? QUEEN : KING;

	board[0][5]->empty = 0;
	board[0][5]->pieceColor = (PieceColor)!playingAsColor;
	board[0][5]->pieceType = BISHOP;

	board[0][6]->empty = 0;
	board[0][6]->pieceColor = (PieceColor)!playingAsColor;
	board[0][6]->pieceType = KNIGHT;

	board[0][7]->empty = 0;
	board[0][7]->pieceColor = (PieceColor)!playingAsColor;
	board[0][7]->pieceType = ROOK;

	/* opponent pawns */

	board[1][0]->empty = 0;
	board[1][0]->pieceColor = (PieceColor)!playingAsColor;
	board[1][0]->pieceType = PAWN;

	board[1][1]->empty = 0;
	board[1][1]->pieceColor = (PieceColor)!playingAsColor;
	board[1][1]->pieceType = PAWN;

	board[1][2]->empty = 0;
	board[1][2]->pieceColor = (PieceColor)!playingAsColor;
	board[1][2]->pieceType = PAWN;

	board[1][3]->empty = 0;
	board[1][3]->pieceColor = (PieceColor)!playingAsColor;
	board[1][3]->pieceType = PAWN;

	board[1][4]->empty = 0;
	board[1][4]->pieceColor = (PieceColor)!playingAsColor;
	board[1][4]->pieceType = PAWN;

	board[1][5]->empty = 0;
	board[1][5]->pieceColor = (PieceColor)!playingAsColor;
	board[1][5]->pieceType = PAWN;

	board[1][6]->empty = 0;
	board[1][6]->pieceColor = (PieceColor)!playingAsColor;
	board[1][6]->pieceType = PAWN;

	board[1][7]->empty = 0;
	board[1][7]->pieceColor = (PieceColor)!playingAsColor;
	board[1][7]->pieceType = PAWN;

	/* player pawns */

	board[6][0]->empty = 0;
	board[6][0]->pieceColor = playingAsColor;
	board[6][0]->pieceType = PAWN;

	board[6][1]->empty = 0;
	board[6][1]->pieceColor = playingAsColor;
	board[6][1]->pieceType = PAWN;

	board[6][2]->empty = 0;
	board[6][2]->pieceColor = playingAsColor;
	board[6][2]->pieceType = PAWN;

	board[6][3]->empty = 0;
	board[6][3]->pieceColor = playingAsColor;
	board[6][3]->pieceType = PAWN;

	board[6][4]->empty = 0;
	board[6][4]->pieceColor = playingAsColor;
	board[6][4]->pieceType = PAWN;

	board[6][5]->empty = 0;
	board[6][5]->pieceColor = playingAsColor;
	board[6][5]->pieceType = PAWN;

	board[6][6]->empty = 0;
	board[6][6]->pieceColor = playingAsColor;
	board[6][6]->pieceType = PAWN;

	board[6][7]->empty = 0;
	board[6][7]->pieceColor = playingAsColor;
	board[6][7]->pieceType = PAWN;

	/* player nobles */

	board[7][0]->empty = 0;
	board[7][0]->pieceColor = playingAsColor;
	board[7][0]->pieceType = ROOK;

	board[7][1]->empty = 0;
	board[7][1]->pieceColor = playingAsColor;
	board[7][1]->pieceType = KNIGHT;

	board[7][2]->empty = 0;
	board[7][2]->pieceColor = playingAsColor;
	board[7][2]->pieceType = BISHOP;

	board[7][3]->empty = 0;
	board[7][3]->pieceColor = playingAsColor;
	board[7][3]->pieceType = playingAsColor == BLACK ? KING : QUEEN;

	board[7][4]->empty = 0;
	board[7][4]->pieceColor = playingAsColor;
	board[7][4]->pieceType = playingAsColor == BLACK ? QUEEN : KING;

	board[7][5]->empty = 0;
	board[7][5]->pieceColor = playingAsColor;
	board[7][5]->pieceType = BISHOP;

	board[7][6]->empty = 0;
	board[7][6]->pieceColor = playingAsColor;
	board[7][6]->pieceType = KNIGHT;

	board[7][7]->empty = 0;
	board[7][7]->pieceColor = playingAsColor;
	board[7][7]->pieceType = ROOK;
}

std::string strForSquare(Square* square) {
	if (square->empty) {
		return ".";
	} else {
		if (square->pieceColor == BLACK) {
			switch (square->pieceType) {
				case PAWN:
					return "♟";
				case KNIGHT:
					return "♞";
				case BISHOP:
					return "♝";
				case ROOK:
					return "♜";
				case QUEEN:
					return "♛";
				case KING:
					return "♚";
				default:
					return "";
			}
		} else {
			switch (square->pieceType) {
				case PAWN:
					return "♙";
				case KNIGHT:
					return "♘";
				case BISHOP:
					return "♗";
				case ROOK:
					return "♖";
				case QUEEN:
					return "♕";
				case KING:
					return "♔";
				default:
					return "";
			}
		}
	}
}

// void displayGameState(Board* board, int dimen) {

// 	printf("\n  a b c d e f g h \n");
// 	// printf("\n");
// 	for (int row = 0; row < dimen; ++row) {
// 		printf("%d ", dimen-row);
// 		for (int col = 0; col < dimen; ++col) {
// 			Square* square = board[row][col];
// 			printf("%s ", strForSquare(square));
// 		}
// 		printf(" %d\n", dimen-row);
// 		// printf("\n");
// 	}

// 	printf("  a b c d e f g h \n\n");
// }

void updateBoard(Board* board) {

	char str[6];
	printf("Enter your move (e.g. 'b1 c3'):\n");
	fgets(str, 6, stdin);
	if (str[0]-'a' < 0 || str[0]-'a' > 7 ||
		str[1]-'1' < 0 || str[1]-'1' > 7 ||
		str[3]-'a' < 0 || str[3]-'a' > 7 ||
		str[4]-'1' < 0 || str[4]-'1' > 7
		) {
		printf("Invalid move (out of bounds / ill formed): %s\n", str);
		return;
	}
	int frow = 7-(str[1]-'1');
	int fcol = str[0]-'a';
	int trow = 7-(str[4]-'1');
	int tcol = str[3]-'a';
	Square* from = board[frow][fcol];
	Square* to = board[trow][tcol];
	if (from == NULL) {
		printf("Invalid move (no piece): %s", str);
		return;
	}
	to->empty = 0;
	to->pieceColor = from->pieceColor;
	to->pieceType = from->pieceType;
	from->empty = 1;
}

int checkGameOver() {


	return 0;
}

// int main() {

// 	int dimen = 8;
// 	Board* board = createBoard(dimen);

// 	resetBoard(board, WHITE);

// 	Player* p1 = (Player*)malloc(sizeof(Player));
// 	Player* p2 = (Player*)malloc(sizeof(Player));

// 	int stop = 0;
// 	while (!stop) {

// 		displayGameState(board, dimen);

// 		updateStateForPlayer(p1, board);
// 		// updateStateForPlayer(p2);

// 		if (checkGameOver()) {
// 			break;
// 		}
// 	}

// 	return 0;
// }