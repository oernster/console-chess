#include "StdAfx.h"
#include "Piece.h"
#include "Pawn.h"
#include "King.h"
#include "Board.h"
#include "Square.h"

CPawn::CPawn(EColour theColour, CBoard* pBoard, short thePosition)
	: CPiece(theColour, pBoard, thePosition), moved(false)
{
}

CPawn::~CPawn(void)
{
}

bool CPawn::move(short sourceRow, short targetRow, short sourceColumn, short targetColumn)
{
	bool bResult = true;

	if (!CPiece::move(sourceRow, targetRow, sourceColumn, targetColumn))
	{
		return false;
	}

	// if the pawn is advancing one square and a nemesis piece adjacent to it did not move last turn and the user had an opportunity to do so, en passant applies
	if (isEnPassant(targetRow, targetColumn))
	{
		// remove the relevant nemesis piece from the board

		// WHAT IF THERE ARE TWO PIECES - ONE EITHER SIDE, AND NEITHER MOVED LAST TURN WITH A VALID OPPORTUNITY - TAKE BOTH OUT OF ACTION??? CLARIFY RULES
		;
	}

	// TURN PAWN INTO QUEEN?

	// we allow 2 steps forward on first move only
	moved = true; 

	return bResult;
}

bool CPawn::isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn)
{
	bool bResult = false;
	short rowDiff = 0;
	short columnDiff = 0;

	if (ycolumn > xcolumn)
	{
		columnDiff = ycolumn - xcolumn;	
	}
	else if (xcolumn > ycolumn)
	{
		columnDiff = xcolumn - ycolumn;	
	}

	if (yrow > xrow)
	{
		if (eColour == eWhite)
		{
			rowDiff = yrow - xrow;
		}
	}
	else if (xrow > yrow)
	{
		if (eColour == eBlack)
		{
			rowDiff = xrow - yrow;
		}
	}

	if (moved == false)
	{
		// advance one row and optionally move diagonally to take a piece
		if (rowDiff == 1)
		{
			if (columnDiff == 0)
			{
				bResult = true;
			}
			else if (columnDiff == 1)
			{
				if (pBoard->getSquares()[getPositionFromRowAndColumn(yrow, ycolumn)]->isSquareOccupied())
				{
					// it is only legal to move diagonally with a pawn when taking a nemesis piece
					if (pBoard->getSquares()[getPositionFromRowAndColumn(yrow, ycolumn)]->getPiece()->getColour() != eColour)
					{
						bResult = true;
					}
				}
			}
		}

		// advancing two rows in one move is only allowed if the pawn has not yet moved
		if ( (rowDiff == 2) && (columnDiff == 0) )
		{
			bResult = true;
		}
	}
	else
	{
		if (rowDiff == 1)
		{
			if ( (columnDiff == 0) || (columnDiff == 1) )
			{
				bResult = true;
			}
		}
	}

	return bResult;
}

bool CPawn::isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn)
{
	bool bResult = false;
	short intermediateRow = -1;
	short rowDiff = 0;

	// verify path vertically up the board is not blocked
	if (xrow < yrow)
	{
		rowDiff = yrow - xrow;
		intermediateRow = xrow + 1;
	}
	else if (yrow < xrow)
	{
		rowDiff = xrow - yrow;
		intermediateRow = yrow - 1;
	}

	if (rowDiff == 2)
	{
		// verify movement path is clear; it is forbidden to take a piece with a pawn when advancing two spaces
		if (pBoard->getSquares()[getPositionFromRowAndColumn(intermediateRow, ycolumn)]->isSquareOccupied())
		{
			bResult = true;
		}
	}

	// same colour piece on target square is a block
	if (isTargetSquareBlocked(yrow, ycolumn))
	{
		bResult = true;	
	}

	return bResult;
}

bool CPawn::isEnPassant(int x, int y)
{
	return false;
}
