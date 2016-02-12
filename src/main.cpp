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
    bool FC, ACP, MAC, MRV, DH, LCV = false;    
    for (int i = 4; i < argc; i++)
    {
        if (strcmp(argv[i], "FC") == 0) FC = true;
        if (argv[i] == "ACP") ACP = true;
        if (argv[i] == "MAC") MAC = true;
        if (argv[i] == "MRV") MRV = true;
        if (argv[i] == "DH") DH = true;
        if (argv[i] == "LCV") LCV = true;
    }

    std::cout << FC << "\n\n";

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
