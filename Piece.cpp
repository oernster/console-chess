#include "StdAfx.h"
#include "Piece.h"
#include "Board.h"
#include "Square.h"
#include "King.h"

CPiece::CPiece(EColour theColour, CBoard* theBoard, short thePosition)
 : eColour(theColour), position(thePosition), movedLastTurn(false), pBoard(theBoard)
{
}

CPiece::~CPiece(void)
{
	delete pBoard;
	pBoard = NULL;
}

bool CPiece::move(std::string sourcePos, std::string targetPos)
{
	bool bResult = false;
	short sourceRow = -1;
	short sourceColumn = -1;
	short targetRow = -1;
	short targetColumn = -1;

	if (getRowAndColumn(sourcePos, sourceRow, sourceColumn))
	{
		if (getRowAndColumn(targetPos, targetRow, targetColumn))
		{
			bResult = move(sourceRow, targetRow, sourceColumn, targetColumn);
		}
	}

	return bResult;
}

bool CPiece::getRowAndColumn(std::string position, short& row, short& column)
{
	return pBoard->getRowsAndColumnsFromPosition(position, row, column);
}


bool CPiece::move(short sourceRow, short targetRow, short sourceColumn, short targetColumn)
{
	bool bResult = true;
	short sourcePos = -1;
	short targetPos = -1;
    bool occupied = false;
	CPiece* pPiece = NULL;

	if (isMovePossible(targetRow, targetColumn))
	{
		if (!pBoard->getPosFromRowAndColumn(targetPos, targetRow, targetColumn))
			return false;

		// perform move operation
		if (pBoard->getSquares()[targetPos]->isSquareOccupied())
		{
			pPiece = pBoard->getSquares()[targetPos]->getPiece();
			occupied = true;
		}

		if (!pBoard->getPosFromRowAndColumn(sourcePos, sourceRow, sourceColumn))
			return false;
		
		if (!pBoard->getPosFromRowAndColumn(targetPos, targetRow, targetColumn))
			return false;
		
		std::vector<CSquare*> squares = pBoard->getSquares();
		
		squares[targetPos]->setPiece(pBoard->getSquares()[sourcePos]->getPiece());
		pBoard->getSquares()[targetPos]->getPiece()->setPosition(targetPos);
		
		squares[sourcePos]->setSquareOccupied(false);
		squares[sourcePos]->setPiece(NULL);
		squares[targetPos]->setSquareOccupied(true);

		// will it expose the good guy to check?
		CKing* pKing = dynamic_cast<CKing*> (getKing(eColour));
		if (pKing != NULL)
		{
			if (pKing->inCheck())
			{
				bResult = false;
			}
		}
		else
		{
			bResult = false;
		}

		if (!bResult)
		{
			// reset to original position prior to move
			bResult = revertMove(sourcePos, targetPos, pPiece, occupied);
		}
		else
		{
			if (pPiece != NULL)
			{
				delete pPiece;
				pPiece = NULL;
			}
		}
	
		updateMoveRecords();
	}
	else
	{
		bResult = false;
	}

	return bResult;
}

bool CPiece::revertMove(short sourcePos, short targetPos, CPiece* pPiece, bool occupied)
{
	std::vector<CSquare*> squares = pBoard->getSquares();
		
	squares[sourcePos]->setPiece(this);
	position = sourcePos;
		
	squares[targetPos]->setSquareOccupied(occupied);
	squares[targetPos]->setPiece(pPiece);
	squares[sourcePos]->setSquareOccupied(true);

	return true;
}

bool CPiece::isMovePossible(short x, short y)
{
	// CONSIDER THROWING EXCEPTIONS INSTEAD OF RETURNING FALSE OR RETURNING A STRING AS A PASSED PARAMETER

	// is the move within the board?
	if ((y < 0 || y > 8) || (x < 0 || x > 8))
	{
		return false;
	}

	short sourceRow = 0;
	short sourceColumn = 0;
	
	if (!pBoard->getRowsAndColumnsFromPosition(position, sourceRow, sourceColumn))
	{
		return false;
	}
	
	// is the move legal for this piece?
	if (!isMoveLegal(sourceRow, x, sourceColumn, y))
		return false;
		
	// is the move blocked?
	if (isMoveBlocked(sourceRow, x, sourceColumn, y))
		return false;

	return true;
}

CPiece* CPiece::getKing(EColour theColour)
{
	// find king of this colour and get castle status
	short xrow = 0;
	short xcolumn = 0;
	CKing* pKing = NULL;

	// find the good guy's King
	for (auto kingFinderIt = pBoard->getSquares().begin(); (kingFinderIt != pBoard->getSquares().end()); ++kingFinderIt)
	{
		if ((*kingFinderIt)->isSquareOccupied())
		{
			if ((*kingFinderIt)->getPiece()->getColour() == theColour) // it's the right colour King
			{
				pKing = dynamic_cast<CKing*> ((*kingFinderIt)->getPiece());
				
				if (pKing != NULL)
				{					
					return pKing;
				}
			}
		}
	}

	return NULL;
}

bool CPiece::isTargetSquareBlocked(short yrow, short ycolumn)
{
	bool bResult = false;

	if (pBoard->getSquares()[getPositionFromRowAndColumn(yrow, ycolumn)]->isSquareOccupied())
	{
		if (pBoard->getSquares()[getPositionFromRowAndColumn(yrow, ycolumn)]->getPiece()->getColour() == eColour)
		{
			bResult = true;
		}
	}

	return bResult;
}

void CPiece::updateMoveRecords()
{
	movedLastTurn = true;

	// set all other pieces of this colour to have a movedLastTurn value of false
	for (auto it = pBoard->getSquares().begin(); it != pBoard->getSquares().end(); ++it)
	{
		if ((*it)->isSquareOccupied())
		{
			if ((*it)->getPiece()->getColour() == eColour)
			{
				if ((*it)->getPiece() != this)
				{
					(*it)->getPiece()->setMovedLastTurn(false);
				}
			}
		}
	}

	return;
}