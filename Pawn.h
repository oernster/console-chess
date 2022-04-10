#pragma once

class CPawn : public CPiece
{
public:
	explicit CPawn(EColour theColour, CBoard* pBoard, short thePosition);
	virtual ~CPawn(void);

	virtual bool move(short sourceRow, short targetRow, short sourceColumn, short targetColumn);
	
private:
	virtual bool isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn);
	virtual bool isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn);

	bool isEnPassant(int x, int y);

	bool moved;
};

