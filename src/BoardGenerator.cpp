#include "BoardGenerator.h"


SudokuBoard BoardGenerator::generateBoard(int N, int p, int q, int numAssignments, int timeout)
{
	Board tempBoard(N, std::vector<char>(N, '0'));
	SudokuBoard sb(p, q, N, tempBoard);

	time(&startTime);

	if (numAssignments > sb.getN() * sb.getN())
	{
		std::cout << "Number of assignments exceeds spaces on board.\n";

		return sb;
	}

	srand(time(NULL));

	int assigned = 0;
	while (assigned < numAssignments) 
	{
		int i = rand() % N + 0, j = rand() % N + 0, k = rand() % (N-1) + 1;
		char val = sb.getDomain()[k];
		if ( sb.getValue(i, j) != '0' ) continue;
		if ( sb.makeAssignment(i, j, val) ) assigned++;
		else
		{
			time(&currTime);
			if (currTime - startTime > timeout)
			{
				std::cout << "Timeout at " << assigned << " elements.\n";
				return sb;
			}
		}
	}
    return sb;
}

std::string BoardGenerator::boardString(SudokuBoard sb)
{
    std::stringstream output;
    output << sb.getN() << " " << sb.getP() << " " << sb.getQ() << std::endl;
    for (int i = 0; i < sb.getN(); i++)
    {
        for (int j = 0; j < sb.getN(); j++)
        {
            output << sb.getValue(i, j) << " ";
        }
        output << std::endl;
    }
    return output.str();
}
