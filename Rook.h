#pragma once

class CKing;

class CRook : public CPiece
{
public:
	explicit CRook(EColour theColour, CBoard* pBoard, short thePosition);
	virtual ~CRook(void);

	virtual bool move(short sourceRow, short targetRow, short sourceColumn, short targetColumn);
	bool hasMoved() { return moved; }

private:
	virtual bool isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn);
	virtual bool isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn);
	bool kingHasCastled(ECastleType& eCastleType);

	bool moved;

};

