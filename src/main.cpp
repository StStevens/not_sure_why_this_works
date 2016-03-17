#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "SudokuBoard.h"
#include "sudokuBoardReader.h"
#include "BoardGenerator.h"
#include "backtrackingSolver.h"



/*
 * If GEN token is given, reads the input file
 * to get the parameters N, p, q, and M and returns
 * a vector of integers with these
 */
std::vector<int> get_params(std::string input)
{
    std::vector<int> params;
    std::ifstream ifp;
    std::string token;

    ifp.open(input);
    if (!(ifp.is_open()))
    {
        std::cout << "Unable to open file.\n";
        exit(-1);
    }
    
    while (ifp >> token)
        params.push_back(std::stoi(token));
    ifp.close();

    return params;
}

/*
 * Given a vector of parameters as input, generates an input file for
 * the sudokuBoardReader to construct a new board by calling board
 * generator with the given paramters N, p, q, and M
 */
void generate_board_file(std::vector<int> &params)
{
    BoardGenerator bg;
    SudokuBoard sb = bg.generateBoard(params[0], params[1], params[2], params[3]);

    std::ofstream ofp;
    ofp.open("randBoard.txt");
    if (!(ofp.is_open()))
    {
        std::cout << "Unable to open file.\n";
        exit(-1);
    }

    ofp << bg.boardString(sb);
    ofp.close();
}


/*
 * Main Program
 * First reads through the passed in to activate specific heuristics, or the generator
 * Either generates a board, or reads one from the current directory and calls the 
 * backtrackingSolver on it, then prints the solved board
 */
int main(int argc, char *argv[])
{
    bool FC = false, ACP = false, MAC = false, MRV = false, DH = false, LCV = false, GEN = false;    
    for (int i = 4; i < argc; i++)
    {
        if (strcmp(argv[i], "FC") == 0) FC = true;
        if (strcmp(argv[i], "ACP") == 0) ACP = true;
        if (strcmp(argv[i], "MAC") == 0) MAC = true;
        if (strcmp(argv[i], "MRV") == 0) MRV = true;
        if (strcmp(argv[i], "DH") == 0) DH = true;
        if (strcmp(argv[i], "LCV") == 0) LCV = true;
        if (strcmp(argv[i], "GEN") == 0) GEN = true;
    }

    if (argc < 4)
    {
        std::cout << "Not enough arguments given\n";
        return 10;
    }

    try
    {
        std::string inpFile;
        if (GEN)
        {
            std::vector<int> params = get_params(argv[1]);
            generate_board_file(params);
            inpFile = "randBoard.txt";
        }
        else inpFile = argv[1];

        sudokuBoardReader sudRead(inpFile);
        SudokuBoard newBoard = sudRead.getBoard();
        std::cout << newBoard.displayBoard() << '\n';

        backtrackingSolver btSolver(&newBoard, std::atoi(argv[3]), FC, MRV, DH, LCV);
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
