#pragma once

class CSquare
{

public:
	explicit CSquare(bool containsPiece = false, CPiece* piece = NULL, short theRow = 1, short theColumn = 1);
	CSquare (const CSquare& s);
	CSquare& operator=(const CSquare& s);

	~CSquare(void);

	bool isSquareOccupied() { return bContainsPiece; }
	void setSquareOccupied(bool occupied) { bContainsPiece = occupied; }
	CPiece* getPiece() { return pPiece; }
	void setPiece(CPiece* thePiece) { pPiece = thePiece; }
	short getRow() { return row; }
	short getColumn() { return column; }
	void setRow(short theRow) { row = theRow; }
	void setColumn(short theColumn) { column = theColumn; }

private:

	short row;
	short column;
	bool bContainsPiece;
	CPiece* pPiece;
};
