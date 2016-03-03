#include <iostream>
#include <string>
#include <sstream>
#include "SudokuBoard.h"

class InvalidBoardError{};


/**
 * Constructor for SudokuBoard
 *
 * @param[in] int p The number of rows per box
 * @param[in] int q The number of columns per box
 * @param[in] int N The number of row and columns in the board
 *
 * @returns SudokuBoard
 **/
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

//Setters
void SudokuBoard::setP(int p) { this->p = p; }
void SudokuBoard::setQ(int q) { this->q = q; }
void SudokuBoard::setN(int N) { this->N = N; }
void SudokuBoard::setBoard(Board newBoard){	board = newBoard; }

//Getters
int SudokuBoard::getP() { return p; }
int SudokuBoard::getQ() { return q; }
int SudokuBoard::getN() { return N; }
std::string SudokuBoard::getDomain() { return validEntries; }
Board SudokuBoard::getBoard() { return board; }

/**
 * Returns the board as a string.
 *
 * @returns String
 **/
std::string SudokuBoard::displayBoard()
{
	std::stringstream toReturn;
	toReturn << "  N: " << N << "   P: " << p << "   Q: " << q << std::endl;

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

/**
 * Returns the board solution as a String.
 * Solution is all zeros for no solution.
 *
 * @prama[in] bool hasSolution If the board has a solution.
 * 
 * @returns string The solution to the board as a string.
 **/
std::string SudokuBoard::boardAsString(bool hasSolution)
{
    std::stringstream boardTuple;
    boardTuple << "(";
    if (hasSolution)
    {
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
    }
    else if (!hasSolution)
    {
        for (int k = 0; k < N; k++)
        {
            for (int l = 0; l < N; l++)
            {
                if ((k * l) == (N-1) * (N-1))
                    boardTuple << "0)";
                else boardTuple << "0,";
            }
        }
    }
    return boardTuple.str();
}
/**
 * Returns true if the character assignment
 * is consistent for the row.
 *
 * @param[in] int rowNum        The row number to check.
 * @param[in] char assignment   The character to try and assign.
 *
 * @returns bool True is the assignment is consistent.
 **/
bool SudokuBoard::validRow(int rowNum, char assignment)
{
	for (int i = 0; i < N; i++){
		if (board[rowNum][i] == assignment){
			return false;
		}
	}
	return true;
}

/**
 * Returns true if the character assignment
 * is consistent for the column.
 *
 * @param[in] int colNum        The column number to check.
 * @param[in] char assignment   The character to try and assign.
 *
 * @returns bool True is the assignment is consistent.
 **/
bool SudokuBoard::validCol(int colNum, char assignment)
{
	for (int i = 0; i < N; i++){
		if (board[i][colNum] == assignment){
			return false;
		}
	}
	return true;
}

/**
 * Returns true if the character assignment
 * is consistent for the box.
 *
 * @param[in] int rowNum        The row number to check.
 * @param[in] int colNum        The column number to check.
 * @param[in] char assignment   The character to try and assign.
 *
 * @returns bool True is the assignment is consistent.
 **/
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

/**
 * Returns true if the character assignment
 * is consistent for the board.
 *
 * @param[in] int rowNum        The box number to check.
 * @param[in] int colNum        The column number to check.
 * @param[in] char assignment   The character to try and assign.
 *
 * @returns bool True is the assignment is consistent.
 **/
bool SudokuBoard::validAssignment(int rowNum, int colNum, char assignment)
{
	return validCol(colNum, assignment) && validRow(rowNum, assignment) && validBox(rowNum, colNum, assignment);
}

/**
 * Returns true if the character assignment
 * is made.
 *
 * @param[in] int rowNum        The box number to check.
 * @param[in] int colNum        The column number to check.
 * @param[in] char assignment   The character to try and assign.
 *
 * @returns bool True is the assignment was made and  consistent.
 **/
bool SudokuBoard::makeAssignment(int rowNum, int colNum, char assignment)
{
	if (validAssignment(rowNum, colNum, assignment)){
		board[rowNum][colNum] = assignment;
		return true;
	}
	return false;
	
}

/**
 * Clears an assigned value by setting it to zero.
 *
 * @param[in] int rowNum        The row number to clear
 * @param[in] int colNum        The column number to clear
 *
 * @returns Void
 **/
void SudokuBoard::clearAssignment(int rowNum, int colNum)
{
    board[rowNum][colNum] = '0';
}


/**
 * Gets all unassigned entires in a box.
 *
 * @param[in] int rowNum        The row number to check in
 * @param[in] int colNum        The column number to check in
 * @param[in] KeySet &boxKeys   Reference to set to fill
 *
 * @returns Void
 **/
void SudokuBoard::getBoxMembers(int rowNum, int colNum, KeySet &boxKeys, bool getAssigned)
{
    int lowX, highX, lowY, highY;

    lowX = rowNum - (rowNum % q);
    highX = rowNum + (q - (rowNum % q));
    lowY = colNum - (colNum % p);
    highY = colNum + (p - (colNum % p));

    for (int i = lowX; i < highX; i++)
    {
        for (int j = lowY; j < highY; j++)
        {
            if (getAssigned || board[i][j] == '0')
            {
                std::pair<int, int> key(i, j);
                boxKeys.insert(key);
            }
        }
    }
}

/**
 * Gets all unassigned entires in a row.
 *
 * @param[in] int rowNum        The row number to check in
 * @param[in] KeySet &rowKeys   Reference to set to fill
 *
 * @returns Void
 **/
void SudokuBoard::getRowMembers(int rowNum, KeySet &rowKeys, bool getAssigned)
{
	for(int i = 0; i < this->N; i++)
	{
        if (getAssigned || board[rowNum][i] == '0')
        {
            std::pair<int, int> key(rowNum, i);
            rowKeys.insert(key);
        } 
	}

}
/**
 * Gets all unassigned entires in a column.
 *
 * @param[in] int colNum        The column number to check in
 * @param[in] KeySet &colKeys   Reference to set to fill
 *
 * @returns Void
 **/
void SudokuBoard::getColMembers(int colNum, KeySet &colKeys, bool getAssigned)
{
	for(int i = 0; i < this->N; i++)
	{
        if (getAssigned || board[i][colNum] == '0')
        {
            std::pair<int, int> key(i, colNum);
            colKeys.insert(key);
        }
	}

}

