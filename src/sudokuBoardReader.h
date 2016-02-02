#ifndef SUDOKUBOARDREADER_H
#define SUDOKUBOARDREADER_H

#include "SudokuBoard.h"
#include <vector>
#include <string>

class sudokuBoardReader {
	public:
		sudokuBoardReader(std::string);
		SudokuBoard getBoard();//std::vector<std::vector<char> > getBoard();
	private:
		int n;
		int p;
		int q;
		Board board;
		void parseHeader(std::string headerStr);
		void fillRow(int rowNum, std::string toParse);
};
#endif
