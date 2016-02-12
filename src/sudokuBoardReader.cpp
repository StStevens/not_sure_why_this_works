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


SudokuBoard sudokuBoardReader::getBoard(){
	SudokuBoard returnBoard(this->p, this->q, this->n, this->board);
	return returnBoard;
}


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
//        throw (InvalidRowEntry("Number of row entires != N"));
    }
        this->board.push_back(entries);
}
