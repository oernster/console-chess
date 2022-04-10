#pragma once

class CQueen : public CPiece
{
public:
	explicit CQueen(EColour theColour, CBoard* pBoard, short thePosition);
	virtual ~CQueen(void);

private:
	virtual bool isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn);
	virtual bool isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn);

};

