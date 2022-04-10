#include "StdAfx.h"
#include <vector>

#include "Piece.h"
#include "Location.h"
#include "Bishop.h"
#include "King.h"
#include "Board.h"
#include "Square.h"

CBishop::CBishop(EColour theColour, CBoard* pBoard, short thePosition)
	: CPiece(theColour, pBoard, thePosition)
{
}

CBishop::~CBishop(void)
{
}

bool CBishop::isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn)
{
	bool bResult = false;

	short rowDiff = -1;
	short columnDiff = -1;
	
	// diagonal movement verification
	if ( (yrow > xrow) && (ycolumn > xcolumn) )
	{
		rowDiff = yrow - xrow;
		columnDiff = ycolumn - xcolumn;
		if (rowDiff == columnDiff)
		{
			bResult = true;
		}
	}
	else if ( (yrow < xrow) && (ycolumn > xcolumn) )
	{
		rowDiff = xrow - yrow;
		columnDiff = ycolumn - xcolumn;
		if (rowDiff == columnDiff)
		{
			bResult = true;
		}
	}
	else if ( (yrow > xrow) && (ycolumn < xcolumn) )
	{
		rowDiff = yrow - xrow;
		columnDiff = xcolumn - ycolumn;
		if (rowDiff == columnDiff)
		{
			bResult = true;
		}
	}
	else if ( (yrow < xrow) && (ycolumn < xcolumn) )
	{
		rowDiff = xrow - yrow;
		columnDiff = xcolumn - ycolumn;
		if (rowDiff == columnDiff)
		{
			bResult = true;
		}
	}

	return bResult;
}

bool CBishop::isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn)
{
	bool bResult = false;
	short rowDiff = -1;
	short i = 0;

	// we are assuming the diference in rows equals the difference in columns since we passed the isLegalMove() test

	// now test for any piece in the diagonal path of the move
	if ( (yrow > xrow) )
	{		
		rowDiff = yrow - xrow;

		for (i = xrow + 1; i < rowDiff; ++i)
		{
			if (pBoard->getSquares()[getPositionFromRowAndColumn(i, i)]->isSquareOccupied())
			{
				bResult = true;
				break;
			}
		}
	}
	else if ( yrow < xrow )
	{
		rowDiff = xrow - yrow;

		for (i = yrow + 1; i < rowDiff; ++i)
		{
			if (pBoard->getSquares()[getPositionFromRowAndColumn(i, i)]->isSquareOccupied())
			{
				bResult = true;
				break;
			}
		}
	}

	// same colour piece on target square is a block
	if (isTargetSquareBlocked(yrow, ycolumn))
	{
		bResult = true;	
	}
		
	return bResult;
}

