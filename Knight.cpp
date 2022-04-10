#include "StdAfx.h"
#include "Piece.h"
#include "Knight.h"
#include "King.h"
#include "Board.h"


CKnight::CKnight(EColour theColour, CBoard* pBoard, short thePosition)
	: CPiece(theColour, pBoard, thePosition)
{
}


CKnight::~CKnight(void)
{
}

bool CKnight::isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn)
{
	bool bResult = false;

	// 1 square left/right, 3 squares up/down
	if ( ((ycolumn - xcolumn) == 1) && ((yrow - xrow) == 3)
		|| ((xcolumn - ycolumn) == 1) && ((yrow - xrow) == 3)
		|| ((ycolumn - xcolumn) == 1) && ((xrow - yrow) == 3)
		|| ((xcolumn - ycolumn) == 1) && ((xrow - yrow) == 3) )
	{
		bResult = true;
	}
	// 3 squares left/right, 1 square up/down
	else if ( ((ycolumn - xcolumn) == 3) && ((yrow - xrow) == 1) 
		|| ((xcolumn - ycolumn) == 3) && ((yrow - xrow) == 1)
		|| ((ycolumn - xcolumn) == 3) && ((xrow - yrow) == 1)
		|| ((xcolumn - ycolumn) == 3) && ((xrow - yrow) == 1) )
	{
		bResult = true;
	}

	return false;
}

bool CKnight::isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn)
{
	bool bResult = false;

	// same colour piece on target square is a block
	if (isTargetSquareBlocked(yrow, ycolumn))
	{
		bResult = true;	
	}

	return bResult;
}
