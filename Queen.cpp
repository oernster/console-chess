#include "StdAfx.h"
#include "Piece.h"
#include "Queen.h"
#include "King.h"
#include "Board.h"
#include "Square.h"


CQueen::CQueen(EColour theColour, CBoard* pBoard, short thePosition)
	: CPiece(theColour, pBoard, thePosition)
{
}

CQueen::~CQueen(void)
{
}

bool CQueen::isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn)
{
	bool bResult = false;
	ECastleType eCastleType = eUnknown;
	short rowDiff = -1;
	short columnDiff = -1;
	
	// horizontal/vertical movement verification
	if (xrow == yrow && xcolumn == ycolumn)
	{
		bResult = true;
	}

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


// need to know which path we're taking...
bool CQueen::isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn)
{
	bool bResult = false;
	short i = 0;
	short rowDiff = -1;
	bool bUp = false;
	bool bDown = false;
	bool bLeft = false;
	bool bRight = false;
	bool bVerticalUpResult = false;
	bool bVerticalDownResult = false;
	bool bHorizontalRightResult = false;
	bool bHorizontalLeftResult = false;
	bool bDiagonalUpResult = false;
	bool bDiagonalDownResult = false;
	
	if ( (xrow - yrow) < 0 )
	{
		bUp = true;
	}

	if ( (yrow - xrow) < 0 )
	{
		bDown = true;
	}

	if ( (xcolumn - ycolumn) < 0 )
	{
		bRight = true;
	}

	if ( (ycolumn - xcolumn) < 0 )
	{
		bLeft = true;
	}

	
	if (bUp && !bRight && !bLeft)
	{
		// verify path vertically up the board is not blocked
		if (xrow < yrow)
		{
			for (i = xrow + 1; i < yrow; ++i)
			{
				if (pBoard->getSquares()[getPositionFromRowAndColumn(i, xcolumn)]->isSquareOccupied())
				{
					bVerticalUpResult = true;
					break;
				}
			}
		}
	}

	if (bDown && !bRight && !bLeft)
	{
		// verify path vertically down the board is not blocked
		if (yrow < xrow)
		{
			for (i = yrow + 1; i < xrow; ++i)
			{
				if (pBoard->getSquares()[getPositionFromRowAndColumn(i, xcolumn)]->isSquareOccupied())
				{
					bVerticalDownResult = true;
					break;
				}
			}
		}
	}

	if (bRight && !bUp && !bDown)
	{
		// verify path horizontally right across the board is not blocked
		if (xcolumn < ycolumn)
		{
			for (i = xcolumn + 1; i < ycolumn; ++i)
			{
				if (pBoard->getSquares()[getPositionFromRowAndColumn(xrow, i)]->isSquareOccupied())
				{
					bHorizontalRightResult = true;
					break;
				}
			}
		}
	}

	if (bLeft && !bUp && !bDown)
	{
		// verify path horizontally left across the board is not blocked
		if (ycolumn < xcolumn)
		{
			for (i = ycolumn + 1; i < xcolumn; ++i)
			{
				if (pBoard->getSquares()[getPositionFromRowAndColumn(xrow, i)]->isSquareOccupied())
				{
					bHorizontalLeftResult = true;
					break;
				}
			}
		}
	}

	// we are assuming the diference in rows equals the difference in columns since we passed the isLegalMove() test

	if ((bRight && bUp) || (bLeft && bUp))
	{
		// now test for any piece in the diagonal path of the move
		if ( (yrow > xrow) )
		{		
			rowDiff = yrow - xrow;

			for (i = 1; i <= rowDiff; ++i)
			{
				if (pBoard->getSquares()[getPositionFromRowAndColumn(xrow + i, xcolumn + i)]->isSquareOccupied())
				{
					bDiagonalUpResult = true;
					break;
				}
			}
		}
	}

	if ((bRight && bDown) || (bLeft && bDown))
	{
		if ( yrow < xrow )
		{
			rowDiff = xrow - yrow;

			for (i = 1; i <= rowDiff; ++i)
			{
				if (pBoard->getSquares()[getPositionFromRowAndColumn(xrow + i, xcolumn + i)]->isSquareOccupied())
				{
					bDiagonalDownResult = true;
					break;
				}
			}
		}
	}

	if (bVerticalUpResult || bVerticalDownResult || bHorizontalRightResult || bHorizontalLeftResult || bDiagonalUpResult || bDiagonalDownResult)
	{
		bResult = true;
	}
	else if (isTargetSquareBlocked(yrow, ycolumn)) // same colour piece on target square is a block
	{
		bResult = true;	
	}

	return bResult;
}
