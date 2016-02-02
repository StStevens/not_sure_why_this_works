#include "backtrackingSolver.h"


class TimeOutError{};

backtrackingSolver::backtrackingSolver(SudokuBoard* toSolve, int maxTime)
{
	board = toSolve;
	generateConstraintGraph();
    timeout = maxTime;
    hasSolution = false;
    deadEnds = 0;
    nodeCount = 0; 
}

void backtrackingSolver::generateConstraintGraph()
{
	for (int i = 0; i < board->getN(); i++)
	{
		for (int j = 0; j < board->getN(); j++)
		{
			if (board->getValue(i, j) == '0')
			{
				toAssign.push_back(Key(i,j)); //Should move this later
				std::string domain = board->getDomain();
				for(std::string::iterator toUse = domain.begin(); toUse != domain.end(); ++toUse)
				{
				    constraintGraph[Key(i, j)].insert(*toUse);
				}
			}
		}
	}
}
   
std::string backtrackingSolver::generateOfp()
{
    std::stringstream output;
    output << "TOTAL_START=" << startTime << std::endl;
    output << "PREPROCESSING_START=" << prepStartTime << std::endl;
    output << "PREPROCESSING_DONE=" << endPrepTime << std::endl;
    output << "SEARCH_START=" << searchStartTime << std::endl;
    output << "SEARCH_DONE=" << endSearchTime << std::endl;
    output << "SOLUTION_TIME=" << ((endPrepTime - prepStartTime) + (endSearchTime - searchStartTime)) << std::endl;
    output << "STATUS=" << endResult << std::endl;
    output << "SOLUTION=" << board->boardAsString() << std::endl;
    output << "COUNT_NODES=" << nodeCount << std::endl;
    output << "COUNT_DEADENDS=" << deadEnds << std::endl;
    
    return output.str();
}

Key backtrackingSolver::selectUnassignedVariable()
{
    Key toReturn = toAssign.front();
    toAssign.pop_front();
	return toReturn;
}

bool backtrackingSolver::backTrackingSearch(int level)
{
    time(&endTime);
    if (endTime - startTime >= timeout) throw (TimeOutError());

    if (toAssign.empty())
        return true;

    Key newVar = selectUnassignedVariable();
    nodeCount++;

    for (Domain::iterator toUse = constraintGraph[newVar].begin(); toUse != constraintGraph[newVar].end(); toUse++)
    {
        if ( board->makeAssignment(newVar.first, newVar.second, *toUse) )
        {
            if (backTrackingSearch(level+1)) return true;
            board->clearAssignment(newVar.first, newVar.second);
        }
    }

    toAssign.push_front(newVar);
    deadEnds++;
    return false;
}

void backtrackingSolver::solve()
{
    time(&startTime);
    time(&prepStartTime);
    time(&endPrepTime);
    time(&searchStartTime);
        
    try
    {
        hasSolution = backTrackingSearch(1);
        endResult = "success";
    }
    catch (TimeOutError e)
    {
        endResult = "timeout";
    }
    catch (...)
    {
        endResult = "error";
    }

    time(&endSearchTime);
    time(&endTime);
}
