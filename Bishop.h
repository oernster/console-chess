#pragma once 

class CBishop : public CPiece
{
public:
	explicit CBishop(EColour theColour, CBoard* pBoard, short thePosition);
	virtual ~CBishop(void);

private:
	virtual bool isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn);
	virtual bool isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn);
	
};

