#include "StdAfx.h"
#include "Piece.h"
#include "Square.h"

CSquare::CSquare(bool containsPiece, CPiece* piece, short theRow, short theColumn)
: bContainsPiece(containsPiece),
  pPiece(piece),
  row(theRow),
  column(theColumn)
{
	if (bContainsPiece)
	{
		if (pPiece != NULL)
		{
			pPiece->setPosition(pPiece->getPositionFromRowAndColumn(row, column));
		}
	}
}

CSquare::~CSquare(void)
{
	if (pPiece != NULL)
	{
		delete pPiece;
		pPiece = NULL;
	}
	
	bContainsPiece = false;
}

CSquare::CSquare(const CSquare& s)
{
	short thisRow = this->row;
	short thisColumn = this->column;
	*this = s;
	row = thisRow;
	column = thisColumn;
}

CSquare& CSquare::operator=(const CSquare& s)
{
	short tempRow = this->getRow();
	short tempColumn = this->getColumn();
	pPiece = s.pPiece;
	this->setRow(tempRow);
	this->setColumn(tempColumn);

	return *this;
}