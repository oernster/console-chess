#include "StdAfx.h"
#include "Piece.h"
#include "King.h"
#include "Rook.h"
#include "Board.h"
#include "Square.h"

CRook::CRook(EColour theColour, CBoard* pBoard, short thePosition)
	: CPiece(theColour, pBoard, thePosition), moved(false)
{
}

CRook::~CRook(void)
{
}

bool CRook::move(short sourceRow, short targetRow, short sourceColumn, short targetColumn)
{
	bool bResult = true;

	if (!CPiece::move(sourceRow, targetRow, sourceColumn, targetColumn))
	{
		bResult = false;
	}
	
	moved = true;

	return bResult;
}

bool CRook::isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn)
{
	bool bResult = false;
	ECastleType eCastleType = eUnknown;
	
	// horizontal/vertical movement verification
	if (xrow == yrow && xcolumn == ycolumn)
	{
		bResult = true;
	}
	// are we castling?
	else if (kingHasCastled(eCastleType) && !moved)
	{
		if (xrow == yrow)
		{
			if (eCastleType == eLeftCastle)
			{
				if (ycolumn == 4)
				{
					bResult = true;
				}
			}
			else if (eCastleType == eRightCastle)
			{
				if (ycolumn == 6)
				{
					bResult = true;
				}
			}
		}
	}
	
	return bResult;
}

bool CRook::isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn)
{
	bool bResult = false;
	short i = 0;
	
	// verify path vertically up the board is not blocked
	if (xrow < yrow)
	{
		for (i = xrow + 1; i < yrow; i++)
		{
			if (pBoard->getSquares()[getPositionFromRowAndColumn(i, xcolumn)]->isSquareOccupied())
			{
				bResult = true;
				break;
			}
		}
	}

	// verify path vertically down the board is not blocked
	if (yrow < xrow)
	{
		for (i = yrow + 1; i < xrow; i++)
		{
			if (pBoard->getSquares()[getPositionFromRowAndColumn(i, xcolumn)]->isSquareOccupied())
			{
				bResult = true;
				break;
			}
		}
	}

	// verify path horizontally right across the board is not blocked
	if (xcolumn < ycolumn)
	{
		for (i = xcolumn + 1; i < ycolumn; i++)
		{
			if (pBoard->getSquares()[getPositionFromRowAndColumn(xrow, i)]->isSquareOccupied())
			{
				bResult = true;
				break;
			}
		}
	}

	// verify path horizontally left across the board is not blocked
	if (ycolumn < xcolumn)
	{
		for (i = ycolumn + 1; i < xcolumn; i++)
		{
			if (pBoard->getSquares()[getPositionFromRowAndColumn(xrow, i)]->isSquareOccupied())
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

	return false;
}

bool CRook::kingHasCastled(ECastleType& eCastleType)
{
	// find king of this colour and get castle status
	short xrow = 0;
	short xcolumn = 0;
	CKing* pKing = dynamic_cast<CKing*> (getKing(eColour));
	if (pKing != NULL)
	{
		return pKing->hasCastled(eCastleType);
	}
	else
	{
		return false;
	}
}
