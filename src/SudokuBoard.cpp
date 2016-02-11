#include <iostream>
#include <string>
#include <sstream>
#include "SudokuBoard.h"

class InvalidBoardError{};

SudokuBoard::SudokuBoard(int p, int q, int N, Board newBoard)
{
	if (N != p * q || N < 1)
	{
  		throw (InvalidBoardError());
	}
	else
	{
    		setP(p);
    		setQ(q);
    		setN(N);
	    	setBoard(newBoard);
		validEntries = AllowedValues.substr(0, N);
	}
}

void SudokuBoard::setP(int p) { this->p = p; }
void SudokuBoard::setQ(int q) { this->q = q; }
void SudokuBoard::setN(int N) { this->N = N; }
void SudokuBoard::setBoard(Board newBoard)
{
	board = newBoard;
}
    
int SudokuBoard::getP() { return p; }
int SudokuBoard::getQ() { return q; }
int SudokuBoard::getN() { return N; }
std::string SudokuBoard::getDomain() { return validEntries; }
Board SudokuBoard::getBoard() { return board; }

std::string SudokuBoard::displayBoard()
{
	std::stringstream toReturn;
	toReturn << "  N: " << N << "\tP: " << p << "\tQ: " << q << std::endl;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			toReturn << board[i][j] << " ";
			if ((j+1) % q == 0 && j != 0 && j != N-1)
				toReturn << "| ";
		}
		toReturn << std::endl;
		if ((i+1) % p == 0 && i != 0 && i != N-1)
		{
			for(int k = 0; k <= N+1 + (N / q / 2); k++)
				toReturn << "--";
			toReturn << std::endl;
		}
	}
	return toReturn.str();
}

std::string SudokuBoard::boardAsString()
{
    std::stringstream boardTuple;
    boardTuple << "(";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if ((i * j) == ((N-1) * (N-1)))
                boardTuple << board[i][j] << ")";
            else
                boardTuple << board[i][j] << ",";
        }
    } 
    
    return boardTuple.str();
}

bool SudokuBoard::validRow(int rowNum, char assignment)
{
	for (int i = 0; i < N; i++){
		if (board[rowNum][i] == assignment){
			return false;
		}
	}
	return true;
}

bool SudokuBoard::validCol(int colNum, char assignment)
{
	for (int i = 0; i < N; i++){
		if (board[i][colNum] == assignment){
			return false;
		}
	}
	return true;
}

bool SudokuBoard::validBox(int rowNum, int colNum, char assignment)
{
	int rDiv = rowNum/p;
	int cDiv = colNum/q;
	
	for (int i = rDiv * p; i < (rDiv + 1) * p; i++)
	{
		for (int j = cDiv * q; j < (cDiv + 1) * q; j++)
		{
			if (i < board.size() && j < board[i].size() && board[i][j] == assignment){
				return false;
			}
		}
	}
	return true;
}

bool SudokuBoard::validAssignment(int rowNum, int colNum, char assignment)
{
	return validCol(colNum, assignment) && validRow(rowNum, assignment) && validBox(rowNum, colNum, assignment);
}

bool SudokuBoard::makeAssignment(int rowNum, int colNum, char assignment)
{
	if (validAssignment(rowNum, colNum, assignment)){
		board[rowNum][colNum] = assignment;
		return true;
	}
	return false;
	
}

void SudokuBoard::clearAssignment(int rowNum, int colNum)
{
    board[rowNum][colNum] = '0';
}

KeySet SudokuBoard::getBoxMembers(int rowNum, int colNum)
{
    int lowX, highX, lowY, highY;
    KeySet boxKeys;

    lowX = rowNum - (rowNum % q);
    highX = rowNum + (q - (rowNum % q));
    lowY = colNum - (colNum % p);
    highY = colNum + (p - (colNum % p));

    for (int i = lowX; i < highX; i++)
    {
        for (int j = lowY; j < highY; j++)
        {
            std::pair<int, int> key(i, j);
            boxKeys.insert(key);
        }
    }

    return boxKeys;
}

KeySet SudokuBoard::getRowMembers(int rowNum)
{
	KeySet rowKeys;

	for(int i = 0; i < this->N; i++)
	{
		std::pair<int, int> key(rowNum, i);
		rowKeys.insert(key);
	}

	return rowKeys;
}

KeySet SudokuBoard::getColMembers(int colNum)
{
	KeySet colKeys;
	for(int i = 0; i < this->N; i++)
	{
		std::pair<int, int> key(i, colNum);
		colKeys.insert(key);
	}

	return colKeys;
}

