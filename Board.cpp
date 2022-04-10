#include "StdAfx.h"
#include <map>

#include "Piece.h"
#include "Square.h"
#include "Board.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Pawn.h"

CBoard* CBoard::_instance = NULL;
long CBoard::instanceCnt = 0;


CBoard::CBoard(void)
{
	initBoard();
}


CBoard::~CBoard(void)
{
	--instanceCnt;
	if ((instanceCnt == 0) && (_instance != NULL))
	{
		// delete all squares
		for (auto it = squares.begin(); it != squares.end(); ++it)
		{
			if ((*it) != NULL)
			{
				delete (*it);
				*it = NULL;
			}
		}

		delete _instance;
		_instance = NULL;
	}
}

CBoard* CBoard::Instance()
{
	if (_instance == NULL)
	{
		_instance = new CBoard;
	}

	++instanceCnt;

	return _instance;
}

void CBoard::initBoard()
{
	initSquares();
	initPositions();
}

void CBoard::initSquares()
{
	squares.clear();
	
	// Initialise the chess board to have empty squares
	// Determine the row/column numbers 
	// : indexed with 1 as the first column element, left to right
	// : indexed with 1 as the first row element, bottom to top
	short i = 0;
	short row = 1;
	for (i = 0; i < 64; ++i)
	{
		CSquare* pSquare = new CSquare;
	
		if (((i + 1) % 8) != 0)
		{
			pSquare->setColumn((i + 1) % 8);
		}
		else
		{
			pSquare->setColumn(8);
		}

		pSquare->setRow(row);

		squares.push_back(pSquare);
		
		if (pSquare->getColumn() == 8)
		{
			++row;
		}
	}

	// 0-7 = bottom row, white rear guard
	// 8-15 = second to bottom row, white front phalanx
	// 48-55 = second to top row, black front phalanx
	// 56-63 = top row, black rear guard

	// Initialise white pieces on board
	short pos = 8;
	for (auto it = &squares[8]; it != &squares[16]; ++it)
	{
		(*it)->setSquareOccupied(true);
		(*it)->setPiece(new CPawn(eWhite, this, pos));
		++pos;
	}

	(*squares.begin())->setSquareOccupied(true);
	(*squares.begin())->setPiece(new CRook(eWhite, this, 0));

	(*(squares.begin()+1))->setSquareOccupied(true);
	(*(squares.begin()+1))->setPiece(new CKnight(eWhite, this, 1));

	(*(squares.begin()+2))->setSquareOccupied(true);
	(*(squares.begin()+2))->setPiece(new CBishop(eWhite, this, 2));

	(*(squares.begin()+3))->setSquareOccupied(true);
	(*(squares.begin()+3))->setPiece(new CQueen(eWhite, this, 3));

	(*(squares.begin()+4))->setSquareOccupied(true);
	(*(squares.begin()+4))->setPiece(new CKing(eWhite, this, 4));

	(*(squares.begin()+5))->setSquareOccupied(true);
	(*(squares.begin()+5))->setPiece(new CBishop(eWhite, this, 5));

	(*(squares.begin()+6))->setSquareOccupied(true);
	(*(squares.begin()+6))->setPiece(new CKnight(eWhite, this, 6));

	(*(squares.begin()+7))->setSquareOccupied(true);
	(*(squares.begin()+7))->setPiece(new CRook(eWhite, this, 7));


	// Initialise black pieces on board
	pos = 55;
	for (auto it = &squares[55]; it != &squares[47]; --it)
	{
		(*it)->setSquareOccupied(true);
		(*it)->setPiece(new CPawn(eBlack, this, pos));
		--pos;
	}

	(*(squares.end()-1))->setSquareOccupied(true);
	(*(squares.end()-1))->setPiece(new CRook(eBlack, this, 63));

	(*(squares.end()-2))->setSquareOccupied(true);
	(*(squares.end()-2))->setPiece(new CKnight(eBlack, this, 62));

	(*(squares.end()-3))->setSquareOccupied(true);
	(*(squares.end()-3))->setPiece(new CBishop(eBlack, this, 61));

	(*(squares.end()-4))->setSquareOccupied(true);
	(*(squares.end()-4))->setPiece(new CQueen(eBlack, this, 60));

	(*(squares.end()-5))->setSquareOccupied(true);
	(*(squares.end()-5))->setPiece(new CKing(eBlack, this, 59));

	(*(squares.end()-6))->setSquareOccupied(true);
	(*(squares.end()-6))->setPiece(new CBishop(eBlack, this, 58));

	(*(squares.end()-7))->setSquareOccupied(true);
	(*(squares.end()-7))->setPiece(new CKnight(eBlack, this, 57));

	(*(squares.end()-8))->setSquareOccupied(true);
	(*(squares.end()-8))->setPiece(new CRook(eBlack, this, 56));

}

void CBoard::initPositions()
{
	positions.clear();
	short i = 0;
	char col = 'A';
	std::string str;
	short character = (short) col;
	short row = 1;
	char* pStr = new char [5];
	memset(pStr, 0, 5);
	
	for (i = 0; i < 64; i++)
	{
		str.clear();
		col = (char) character;
		sprintf_s(pStr, 4, "%c%d", col, row);
		str.assign(pStr);

		// Associate element position in squares vector with position descriptor
		positions[i] = str;

		// Change to next position descriptor
		character = (short) col;
		++character;
		
		if (((i+1)%8) == 0)
		{
			col = 'A';
			character = (short) col;
			++row;
		}
	}

	delete pStr;
	pStr = NULL;
}

bool CBoard::getRowsAndColumnsFromPosition(short pos, short& theRow, short& theColumn)
{
	bool bResult = false;

	// get row data
	if (pos >= 0 && pos < 8)
	{
		theRow = 1;
		bResult = true;
	}
	else if (pos > 7 && pos < 16)
	{
		theRow = 2;
		bResult = true;
	}
	else if (pos > 15 && pos < 24)
	{
		theRow = 3;
		bResult = true;
	}
	else if (pos > 23 && pos < 32)
	{
		theRow = 4;
		bResult = true;
	}
	else if (pos > 31 && pos < 40)
	{
		theRow = 5;
		bResult = true;
	}
	else if (pos > 39 && pos < 48)
	{
		theRow = 6;
		bResult = true;
	}
	else if (pos > 47 && pos < 56)
	{
		theRow = 7;
		bResult = true;
	}
	else if (pos > 55 && pos < 64)
	{
		theRow = 8;
		bResult = true;
	}

	// get column data	
	theColumn = ((pos % 8) + 1);

	return bResult;
}

bool CBoard::getRowsAndColumnsFromPosition(std::string pos, short &row, short& column)
{
	bool bResult = false;
	short arrPos = -1;

	for (auto it = positions.begin(); it != positions.end(); ++it)
	{
		if (strcmp(it->second.c_str(), pos.c_str()) == 0)
		{
			arrPos = it->first;

			bResult = getRowsAndColumnsFromPosition(arrPos, row, column);
			
			break;
		}
	}

	return bResult;
}

void CBoard::display()
{

}

CPiece* CBoard::getPiece(std::string position)
{
	CPiece* pPiece = NULL;

	for (auto it = positions.begin(); it != positions.end(); ++it)
	{
		if (strcmp(it->second.c_str(), position.c_str()) == 0)
		{
			if (getSquares()[it->first]->isSquareOccupied())
			{
				pPiece = getSquares()[it->first]->getPiece();
				break;
			}
		}
	}

	return pPiece;
}

bool CBoard::getPosFromRowAndColumn(short& pos, short row, short column)
{
	bool bResult = false;

	if ((row >= 1 && row <= 8) && (column >=1 && column <= 8))
	{
		pos = ((row - 1) * 8) + column - 1;

		bResult = true;
	}

	return bResult;
}
