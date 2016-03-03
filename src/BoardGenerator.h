#ifndef SUDOKU_GENERATOR_H
#define SUDOKU_GENERATOR_H

#include <ctime>
#include <random>
#include <sstream>
#include <iostream>
#include "SudokuBoard.h"


class BoardGenerator
{
	time_t startTime;
	time_t endTime;

	public:
	BoardGenerator() {}         // Constructor
	SudokuBoard generateBoard(int N, int p, int q, int numAssingments, int timeout = 30);
    std::string boardString(SudokuBoard sb);
};


#endif
