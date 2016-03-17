#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <string>
#include "SudokuBoard.h"
#include "sudokuBoardReader.h"


//Constructor

sudokuBoardReader::sudokuBoardReader(std::string fileName){
        try{
                std::ifstream sudokuFile;
                std::string line;
                sudokuFile.open(fileName);
                if (!sudokuFile) throw (BadFileException());
                std::getline(sudokuFile, line);
                parseHeader(line);
                int i = 0;
                while (i < this-> n && std::getline(sudokuFile, line)) {
			fillRow(i, line);
                        i++;
                }
                sudokuFile.close();
        } catch(std::ifstream::failure e) {
                std::cout << "Failed to read board file\n"; 
        }

}

/**
 * Returns a SudokuBoard Object that
 * represents the current board
 */
SudokuBoard sudokuBoardReader::getBoard(){
	SudokuBoard returnBoard(this->p, this->q, this->n, this->board);
	return returnBoard;
}

/**
 * Parses the header of a board file.
 *
 * This function prases the given str as
 * the header line for a board file.
 * It then sets the parameters of the board object
 * according to the parameters.
 *
 * @param[in]   str::headerStr  The string to parse as the header.
 *
 * @returns None
 */
void sudokuBoardReader::parseHeader(std::string headerStr){
    std::vector<std::string> options;
	std::stringstream lineStream(headerStr);
	std::string token;
	while(lineStream >> token){
		options.push_back(token);
	}
        if (options.size() != 3)
		std::cout << "Bad Parameters\n";
        n = std::stoi(options[0]);
        p = std::stoi(options[1]);
        q = std::stoi(options[2]);
}

/**
 * Fills the give from number with the string from
 * the board file.
 *
 * This function fills the board object at the given
 * rowNum with the contents of toParse
 *
 * @param[in]   int::rowNum     The row number to fill
 * @param[in]   str::toParse    The string to parse for entries
 *
 * @returns None
 */
void sudokuBoardReader::fillRow(int rowNum, std::string toParse) {
    std::vector<char> entries;
    std::stringstream rowStream(toParse);
    char entry;
    while(rowStream >> entry){
        entries.push_back(entry);
    }

    if (entries.size() != n)
    {
	    std::cout << "Bad N. Expected: " << n << " and got " << entries.size() << " \n";
    }
        this->board.push_back(entries);
}
