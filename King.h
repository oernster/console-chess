#pragma once

class CRook;

class CKing : public CPiece
{
public:
	explicit CKing(EColour theColour, CBoard* pBoard, short thePosition);
	virtual ~CKing(void);

	virtual bool move(short sourceRow, short targetRow, short sourceColumn, short targetColumn);
	bool hasCastled(ECastleType& theCastleType) { theCastleType = eCastleType; return castled; }
	bool inCheckTest(short yrow, short ycolumn);
	bool inCheck();
	bool checkMate();
	
private:
	virtual bool isMoveLegal(short xrow, short yrow, short xcolumn, short ycolumn);
	virtual bool isMoveBlocked(short xrow, short yrow, short xcolumn, short ycolumn);

	bool allLegalMovesAreInCheck(short xrow, short xcolumn);
	bool isMoveTowardRook(short yrow, short ycolumn);
	bool rookHasMoved();
	bool hasMoved() { return moved; }
	bool isMoveCastling(short xrow, short yrow, short xcolumn, short ycolumn);
	CRook* getRook(EColour theColour);

	bool moved;
	bool castled;
	ECastleType eCastleType;

};

