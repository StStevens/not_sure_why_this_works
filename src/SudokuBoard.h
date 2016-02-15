#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include <string>
#include <vector>
#include <set>
#include <utility>

class BadFileException{};

const std::string AllowedValues = "123456789ABCDEFGHIJKLMNOPQRSTUZ";
typedef std::vector< std::vector<char> > Board;
typedef std::set<std::pair<int, int>> KeySet;

class SudokuBoard
{
	int p; //Rows
	int q; //Columns
	int N;
	std::string validEntries;

	Board board;

	void setP(int p);
	void setQ(int q);
	void setN(int N);
	void setBoard(Board newBoard);
	bool validRow(int rowNum, char assignment);
	bool validCol(int colNum, char assignment);
	bool validBox(int rowNum, int colNum, char assignment);

	public:
	SudokuBoard(int p, int q, int N, Board newBoard);
	int getP();
	int getQ();
	int getN();
	std::string getDomain();
	Board getBoard();
	char getValue(int p, int q) { return board[p][q]; }

	std::string displayBoard();
	std::string boardAsString(bool hasSolution);
	bool validAssignment(int rowNum, int colNum, char assignment);
	bool makeAssignment(int rownNum, int colNum, char assignment);
	void clearAssignment(int rowNum, int colNum);
	void getBoxMembers(int rowNum, int colNum, KeySet &boxKeys);
	void getRowMembers(int rowNum, KeySet &rowKeys);
	void getColMembers(int colNum, KeySet &colKeys);
	
};


#endif
