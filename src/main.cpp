#include <iostream>
#include <fstream>
#include <cstdlib>
#include "SudokuBoard.h"
#include "sudokuBoardReader.h"
#include "BoardGenerator.h"
#include "backtrackingSolver.h"


int main(int argc, char *argv[])
{
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

        backtrackingSolver btSolver(&newBoard, std::atoi(argv[3]));
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
