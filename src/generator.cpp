#include <fstream>
#include "BoardGenerator.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Not enough arguments given.\n";
        return 10;
    }

    std::vector<std::string> params;
    std::ifstream ifp;
    std::ofstream ofp;
    std::string line, token;

    ifp.open(argv[1]);
    if (!(ifp.is_open()))
    {
        std::cout << "Unable to open file.\n";
        return 10;
    }

    std::getline(ifp, line);
    std::stringstream parse(line);

    while (parse >> token)
        params.push_back(token);
    ifp.close();

    BoardGenerator bg;
    SudokuBoard sb = bg.generateBoard(std::stoi(params[0]), std::stoi(params[1]), std::stoi(params[2]), std::stoi(params[3]));
    
    ofp.open(argv[2]);
    if (!(ofp.is_open()))
    {
        std::cout << "Unable to open file.\n";
        return 10;
    }

    ofp << bg.boardString(sb);
    ofp.close();
    
    return 0;
}
