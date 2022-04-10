#pragma once

#include <vector>
#include <map>

class CSquare;

class CBoard
{
public:
	CBoard(void);
	~CBoard(void);
	static CBoard* Instance();

	bool getRowsAndColumnsFromPosition(short pos, short& row, short& column);
	bool getRowsAndColumnsFromPosition(std::string pos, short &row, short& column);
	bool getPosFromRowAndColumn(short& pos, short row, short column);

	std::vector<CSquare*>& getSquares() { return squares; }
	void display();
	CPiece* getPiece(std::string);
	std::map<short, std::string>& getPositions() { return positions; }
	
private:
	void initBoard();
	void initSquares();
	void initPositions();

	static CBoard* _instance;
	static long instanceCnt;
	
	std::vector<CSquare*> squares;
	std::map<short, std::string> positions;
};

