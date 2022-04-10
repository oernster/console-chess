#include "StdAfx.h"
#include "Piece.h"
#include "Rook.h"
#include "King.h"
#include "Board.h"
#include "Square.h"


CKing::CKing(EColour theColour, CBoard* pBoard, short thePosition)
	: CPiece(theColour, pBoard, thePosition), moved(false), castled(false), eCastleType(eUnknown)
{
}

CKing::~CKing(void)
{
}

bool CKing::move(short sourceRow, short targetRow, short sourceColumn, short targetColumn)
{
	bool bResult = true;

	if (!CPiece::move(sourceRow, targetRow, sourceColumn, targetColumn))
		return false;

	if (isMoveCastling(sourceRow, targetRow, sourceColumn, targetColumn))
	{
		bool bExitLoop = false;
		CRook* pRook = NULL;

		if (targetColumn > sourceColumn)
		{
			eCastleType = eRightCastle;
		}
		else
		{
			eCastleType = eLeftCastle;
		}

		// find correct good guy's rook
		for (auto rookFinderIt = pBoard->getSquares().begin(); (rookFinderIt != pBoard->getSquares().end()) && !bExitLoop; ++rookFinderIt)
		{
			if ((*rookFinderIt)->isSquareOccupied())
			{
				if ((*rookFinderIt)->getPiece()->getColour() == eColour) 
				{				
					pRook = dynamic_cast<CRook*> ((*rookFinderIt)->getPiece());
					if (pRook != NULL)
					{
						if (((*rookFinderIt)->getRow() == sourceRow) && ((*rookFinderIt)->getColumn() == sourceColumn))
						{
							// it's the correct good guy's rook
							bExitLoop = true;
						}
						else // reset the rook pointer as this is not the right one
						{
							pRook = NULL;
						}
					}
				}
			}
		}

		// SORT THIS OUT

		// castle if we have the correct rook
		/*if (pRook != NULL)
		{
			if (eCastleType == eLeftCastle)
			{
				xcolumn = 1;
				ycolumn = 3;
			}
			else if (eCastleType == eRightCastle)
			{
				xcolumn = 8;
				ycolumn = 6;
			}

			if (eColour == eWhite)
			{
				xrow = 1;
				yrow = 1;
			}
			else if (eColour == eBlack)
			{
				xrow = 8;
				yrow = 8;
			}

			if (!pRook->move(getPositionFromRowAndColumn(xrow, xcolumn), getPositionFromRowAndColumn(yrow, ycolumn)))
			{
				bResult = false;
			}
		}*/
	}

	moved = true;

	return bResult;
}

bool CKing::isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn)
{
	if ((xrow == (yrow + 1)) && (xcolumn == ycolumn)
		|| (xrow == (yrow - 1)) && (xcolumn == ycolumn)
		|| (xrow == yrow) && (xcolumn == (ycolumn + 1))
		|| (xrow == yrow) && (xcolumn == (ycolumn - 1))
		|| (xrow == (yrow + 1)) && (xcolumn == (ycolumn - 1))
		|| (xrow == (yrow - 1)) && (xcolumn == (ycolumn - 1))
		|| (xrow == (yrow - 1)) && (xcolumn == (ycolumn + 1))
		|| (xrow == (yrow + 1)) && (xcolumn == (ycolumn + 1)))
	{
		return true;
	}
	// we may be castling so first ensure that we are not in check
	/*else if (inCheck())
	{
		return false;
	}*/ //TBD
	// now ensure that we are moving adjacent to a rook and that the rook and king have never moved in the current game
	else if (isMoveCastling(xrow, yrow, xcolumn, ycolumn) && !rookHasMoved() && !moved)
	{
		castled = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool CKing::isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn)
{
	bool bResult = false;

	if (isMoveCastling(xrow, xcolumn, yrow, ycolumn))
	{
		short row = 0;
		std::vector<short> columns;
		columns.clear();

		if (eCastleType == eLeftCastle)
		{
			columns.push_back(2);
			columns.push_back(3);
			columns.push_back(4);
		}
		// eRightCastle
		else
		{
			columns.push_back(6);
			columns.push_back(7);
		}

		if (eColour == eWhite)
		{
			row = 1;
		}
		// eBlack
		else 
		{
			row = 8;
		}

		// if any of the spaces that the king must travel through to castle are occupied the move is blocked
		for (auto it = columns.begin(); it != columns.end(); ++it)
		{
			if (pBoard->getSquares()[getPositionFromRowAndColumn(row, *it)]->isSquareOccupied())
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

bool CKing::inCheck()
{
	// go through all nemesis player's pieces and do isMoveLegal and isMoveBlocked using the good guy's King as the target square
	short xrow = 0;
	short xcolumn = 0;
	short yrow = 0;
	short ycolumn = 0;
	bool bExitLoop = false;

	// find the good guy's King
	for (auto kingFinderIt = pBoard->getSquares().begin(); (kingFinderIt != pBoard->getSquares().end()) && !bExitLoop; ++kingFinderIt)
	{
		if ((*kingFinderIt)->isSquareOccupied())
		{
			if ((*kingFinderIt)->getPiece()->getColour() == eColour) // it's the good guy's King
			{
				yrow = (*kingFinderIt)->getRow();
				ycolumn = (*kingFinderIt)->getColumn();

				bExitLoop = true;
			}
		}
	}

	for (auto it = pBoard->getSquares().begin(); it != pBoard->getSquares().end(); ++it)
	{
		if ((*it)->isSquareOccupied())
		{
			if ((*it)->getPiece()->getColour() != eColour) // it's a nemesis piece
			{
				xrow = (*it)->getRow();
				xcolumn = (*it)->getColumn();
				
				// y's are this King
				if ((*it)->getPiece()->isMoveLegal(xrow, yrow, xcolumn, ycolumn) && !(*it)->getPiece()->isMoveBlocked(xrow, yrow, xcolumn, ycolumn))
				{
					return true;
				}
			}
		}
	}
	
	return false;
}

bool CKing::checkMate()
{
	short xrow = 0;
	short xcolumn = 0;

	pBoard->getRowsAndColumnsFromPosition(position, xrow, xcolumn);
	
	if (inCheck() && allLegalMovesAreInCheck(xrow, xcolumn))
	{
		return true;
	}

	return false;
}

bool CKing::allLegalMovesAreInCheck(short xrow, short xcolumn)
{
	if (isMoveLegal(xrow, xrow+1, xcolumn, xcolumn) && !isMoveBlocked(xrow, xrow+1, xcolumn, xcolumn) && !inCheckTest(xrow+1, xcolumn))
	{
		return false;
	}
	else if (isMoveLegal(xrow, xrow-1, xcolumn, xcolumn) && !isMoveBlocked(xrow, xrow-1, xcolumn, xcolumn) && !inCheckTest(xrow-1, xcolumn))
	{
		return false;
	}
	else if (isMoveLegal(xrow, xrow, xcolumn, xcolumn+1) && !isMoveBlocked(xrow, xrow, xcolumn, xcolumn+1) && !inCheckTest(xrow, xcolumn+1))
	{
		return false;
	}
	else if (isMoveLegal(xrow, xrow, xcolumn, xcolumn-1) && !isMoveBlocked(xrow, xrow, xcolumn, xcolumn-1) && !inCheckTest(xrow, xcolumn-1))
	{
		return false;
	}
	else if (isMoveLegal(xrow, xrow+1, xcolumn, xcolumn+1) && !isMoveBlocked(xrow, xrow+1, xcolumn, xcolumn+1) && !inCheckTest(xrow+1, xcolumn+1))
	{
		return false;
	}
	else if (isMoveLegal(xrow, xrow-1, xcolumn, xcolumn-1) && !isMoveBlocked(xrow, xrow-1, xcolumn, xcolumn-1) && !inCheckTest(xrow-1, xcolumn-1))
	{
		return false;
	}
	else if (isMoveLegal(xrow, xrow+1, xcolumn, xcolumn-1) && !isMoveBlocked(xrow, xrow+1, xcolumn, xcolumn-1) && !inCheckTest(xrow+1, xcolumn-1))
	{
		return false;
	}
	else if (isMoveLegal(xrow, xrow-1, xcolumn, xcolumn+1) && !isMoveBlocked(xrow, xrow-1, xcolumn, xcolumn+1) && !inCheckTest(xrow-1, xcolumn+1))
	{
		return false;
	}	
	else
	{
		return true;
	}
}

bool CKing::inCheckTest(short yrow, short ycolumn)
{
	short xrow = 0;
	short xcolumn = 0;

	for (auto it = pBoard->getSquares().begin(); it != pBoard->getSquares().end(); ++it)
	{
		if ((*it)->isSquareOccupied())
		{
			if ((*it)->getPiece()->getColour() != eColour) // it's a nemesis piece
			{
				xcolumn = (*it)->getColumn();
				xrow = (*it)->getRow();
		
				// y's are this King
				if ((*it)->getPiece()->isMoveLegal(xrow, yrow, xcolumn, ycolumn) && !(*it)->getPiece()->isMoveBlocked(xrow, yrow, xcolumn, ycolumn))
				{
					return true;
				}
			}
		}
	}
	
	return false;
}

bool CKing::rookHasMoved()
{
	CRook* pRook = NULL;
	if (eColour == eWhite)
	{
		pRook = getRook(eBlack);
	}
	else if (eColour == eBlack)
	{
		pRook = getRook(eWhite);
	}
	else
	{
		return false;
	}
	
	return pRook->hasMoved();
}

bool CKing::isMoveCastling(short xrow, short yrow, short xcolumn, short ycolumn)
{
	if (ycolumn > xcolumn)
	{
		if ((xrow == yrow) && ((ycolumn - xcolumn) == 2))
		{
			return true;
		}
	}
	else if (xcolumn > ycolumn)
	{
		if ((xrow == yrow) && ((xcolumn - ycolumn) == 2))
		{
			return true;
		}
	}
	
	return false;
}

CRook* CKing::getRook(EColour theColour)
{
	short xrow = 0;
	short xcolumn = 0;
	bool bExitLoop = false;
	CRook* pRook = NULL;

	// find the good guy's King
	for (auto rookFinderIt = pBoard->getSquares().begin(); (rookFinderIt != pBoard->getSquares().end()) && !bExitLoop; ++rookFinderIt)
	{
		if ((*rookFinderIt)->isSquareOccupied())
		{
			if ((*rookFinderIt)->getPiece()->getColour() == eColour) 
			{				
				CRook* pRook = dynamic_cast<CRook*> ((*rookFinderIt)->getPiece());
				if (pRook != NULL)
				{
					// it's the good guy's rook
					return pRook;
				}
			}
		}
	}

	return pRook;
}