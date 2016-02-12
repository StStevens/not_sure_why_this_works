#include "backtrackingSolver.h"


class TimeOutError{};

backtrackingSolver::backtrackingSolver(SudokuBoard* toSolve, int maxTime, bool fc = false)
{
    board = toSolve;
    generateConstraintGraph();
    timeout = maxTime;
    hasSolution = false;
    deadEnds = 0;
    nodeCount = 0;
    forwardCheckingEnabled = fc;
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

    std::list<CheckChange> fcPruned;

    Key newVar = selectUnassignedVariable();
    nodeCount++;

    for (Domain::iterator toUse = constraintGraph[newVar].begin(); toUse != constraintGraph[newVar].end(); toUse++)
    {
        if ( board->makeAssignment(newVar.first, newVar.second, *toUse) )
        {
            if(forwardCheckingEnabled)
            {
                forwardCheck(newVar.first, newVar.second, *toUse, fcPruned);
            }

            if (backTrackingSearch(level+1)) return true;
                board->clearAssignment(newVar.first, newVar.second);

            if(forwardCheckingEnabled)
            {
                replaceInDomain(fcPruned);
                fcPruned.clear();
            }
        }
    }

    toAssign.push_front(newVar);
    deadEnds++;
    return false;
}

CheckChange backtrackingSolver::removeFromDomain(Key entry, char toRemove){
    Domain::iterator domainEntry = constraintGraph[entry].find(toRemove);
        if(!(domainEntry == constraintGraph[entry].end())){
                constraintGraph[entry].erase(domainEntry);
                return CheckChange(entry, toRemove);
       }
}

KeySet backtrackingSolver::getRelatedEntries(int row, int column)
{
    KeySet relatedPairs = board->getBoxMembers(row, column);
    KeySet rowMembers = board->getRowMembers(row);
    KeySet colMembers = board->getColMembers(column);
    for(KeySet::iterator keyPair = rowMembers.begin(); keyPair != rowMembers.end(); keyPair++)
    {
        relatedPairs.insert(*keyPair);
    }
    for(KeySet::iterator keyPair = colMembers.begin(); keyPair != colMembers.end(); keyPair++)
    {
        relatedPairs.insert(*keyPair);
    }
    KeySet::iterator removeSelf = relatedPairs.find(Key(row, column));
    if(removeSelf != relatedPairs.end()) //If we forward check away the key it won't assign.
    {
        relatedPairs.erase(removeSelf);
    }

    return relatedPairs;
}

void backtrackingSolver::replaceInDomain(std::list<CheckChange> toRestore)
{
    for(std::list<CheckChange>::iterator entry = toRestore.begin(); entry !=  toRestore.end(); entry++)
    {
        Key toFix = entry->first;
        char toAdd = entry->second;
        constraintGraph[toFix].insert(toAdd);
    //  constraintGraph[entry->first].insert(entry->second);
    }
}

void backtrackingSolver::forwardCheck(int row, int column, char assigned, std::list<CheckChange> &changeList)
{
    KeySet potentialChanges = getRelatedEntries(row, column);
    for(KeySet::iterator toCheck = potentialChanges.begin(); toCheck != potentialChanges.end(); toCheck++)
    {
        Domain::iterator domainEntry = constraintGraph[*toCheck].find(assigned);
        if(domainEntry != constraintGraph[*toCheck].end()){
            changeList.push_back(removeFromDomain(*toCheck, assigned));
        }
    }
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
