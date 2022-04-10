#pragma once

#include <string>

typedef enum
{
	eWhite,
	eBlack

} EColour;

typedef enum
{
	eUnknown,
	eLeftCastle, // King moves to rook on left of board and rook jumps right over King
	eRightCastle // King moves to rook on right of board and rook jumps left over King

} ECastleType;


class CBoard;

class CPiece
{
public:
	explicit CPiece(EColour theColour, CBoard* pBoard, short thePosition);
	virtual ~CPiece(void);

	bool move(std::string sourcePos, std::string targetPos);

	virtual bool move(short sourceRow, short targetRow, short sourceColumn, short targetColumn);
	virtual bool isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn) = 0;
	virtual bool isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn) = 0;
	
	bool isMovePossible(short x, short y);
	void setPosition(short thePosition) { position = thePosition; }
	short getPositionFromRowAndColumn(short row, short column) { return ((8 * (row-1)) + column) - 1; }
	CPiece* getKing(EColour theColour);

	EColour getColour() { return eColour; }
	
protected:
	bool isTargetSquareBlocked(short yrow, short ycolumn);
	
	EColour eColour;
	CBoard* pBoard;
	short position;
	bool movedLastTurn;

private:
	bool revertMove(short sourcePos, short targetPos, CPiece* pPiece, bool occupied);
	bool getRowAndColumn(std::string position, short& row, short& column);
	void updateMoveRecords();
	void setMovedLastTurn(bool movedLastTurnSetting) { movedLastTurn = movedLastTurnSetting; }
};

