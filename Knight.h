#pragma once

class CKnight : public CPiece
{
public:
	explicit CKnight(EColour theColour, CBoard* pBoard, short thePosition);
	virtual ~CKnight(void);

private:
	virtual bool isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn);
	virtual bool isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn);

};

