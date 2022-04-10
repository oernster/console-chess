#pragma once
class CLocation
{
public:
	explicit CLocation(short theRow, short theColumn);
	~CLocation(void);

	short getRow() { return row; }
	short getColumn() { return column; }

private:

	short row;
	short column;
};

