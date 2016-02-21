#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include <string>
#include <vector>
#include <set>
#include <utility>

class BadFileException{};

const std::string AllowedValues = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
typedef std::vector< std::vector<char> > Board;
typedef std::set<std::pair<int, int>> KeySet;

class SudokuBoard
{
	int p; //Rows
	int q; //Columns
	int N; //Range N

    //The valid entires for our board 1 - N
	std::string validEntries;
    
    //Our board
	Board board;


    //Setters
	void setP(int p);
	void setQ(int q);
	void setN(int N);
	void setBoard(Board newBoard);

    //Validation Sub Checks
	bool validRow(int rowNum, char assignment);
	bool validCol(int colNum, char assignment);
	bool validBox(int rowNum, int colNum, char assignment);

	public:
    //Constructor
	SudokuBoard(int p, int q, int N, Board newBoard);

    //Getters
	int getP();
	int getQ();
	int getN();
	std::string getDomain();
	Board getBoard();
	char getValue(int p, int q) { return board[p][q]; }

    //Display the Board
	std::string displayBoard();
	std::string boardAsString(bool hasSolution);
    //Assignemt Validation
	bool validAssignment(int rowNum, int colNum, char assignment);
    //Make an assignment
	bool makeAssignment(int rownNum, int colNum, char assignment);
	void clearAssignment(int rowNum, int colNum);

    //Getters for related entries
	void getBoxMembers(int rowNum, int colNum, KeySet &boxKeys);
	void getRowMembers(int rowNum, KeySet &rowKeys);
	void getColMembers(int colNum, KeySet &colKeys);
	
};


#endif
