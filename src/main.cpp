#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "SudokuBoard.h"
#include "sudokuBoardReader.h"
#include "BoardGenerator.h"
#include "backtrackingSolver.h"


int main(int argc, char *argv[])
{
    bool FC = false, ACP = false, MAC = false, MRV = false, DH = false, LCV = false, PTY = false;    
    for (int i = 4; i < argc; i++)
    {
        if (strcmp(argv[i], "FC") == 0) FC = true;
        if (strcmp(argv[i], "ACP") == 0) ACP = true;
        if (strcmp(argv[i], "MAC") == 0) MAC = true;
        if (strcmp(argv[i], "MRV") == 0) MRV = true;
        if (strcmp(argv[i], "DH") == 0) DH = true;
        if (strcmp(argv[i], "LCV") == 0) LCV = true;
        if (strcmp(argv[i], "PTY") == 0) PTY = true;
    }

    std::cout << FC << std::endl << MRV <<std::endl;

    if (argc < 4)
    {
        std::cout << "Not enough arguments given\n";
        return 10;
    }

    try
    {
        sudokuBoardReader sudRead(argv[1]);
        SudokuBoard newBoard = sudRead.getBoard();
        std::cout << newBoard.displayBoard() << '\n';

        backtrackingSolver btSolver(&newBoard, std::atoi(argv[3]), FC, ACP, MAC, MRV, DH, LCV);
        btSolver.solve();

        std::cout << newBoard.displayBoard() << '\n';

        std::ofstream output;
        output.open(argv[2]);
        output << btSolver.generateOfp();
        output.close(); 
    }
    catch (BadFileException e)
    {
        std::cout << "Input file could not be opened\n";
        return 10;
    }

    return 0;
}
