#include "backtrackingSolver.h"


class TimeOutError{};
class NoRemainingVals{};

backtrackingSolver::backtrackingSolver(SudokuBoard* toSolve, int maxTime, bool FC, bool ACP, bool MAC, bool MRV, bool DH, bool LCV)
{
    board = toSolve;
    generateConstraintGraph();
    timeout = maxTime;
    hasSolution = false;
    deadEnds = 0;
    nodeCount = 0;
    
    // Init Config Variables
    forwardCheckingEnabled = FC;
    minRemVar = MRV;
    degHeur = DH;
    this->LCV = LCV;
}

void backtrackingSolver::generateConstraintGraph()
{
    for (int i = 0; i < board->getN(); i++)
    {
        for (int j = 0; j < board->getN(); j++)
        {
            if (board->getValue(i, j) == '0')
            {
                toAssign.push_back(Key(i,j)); // Should move this later
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
    output << "SOLUTION=" << board->boardAsString(hasSolution) << std::endl;
    output << "COUNT_NODES=" << nodeCount << std::endl;
    output << "COUNT_DEADENDS=" << deadEnds << std::endl;
    
    return output.str();
}

Key backtrackingSolver::findHighestDegree(std::list<Key> &toCheck)
{
    KeySet keysConstrained;
    Key toReturn;
    int maxConstraints = 0;
    for (std::list<Key>::iterator iter = toCheck.begin(); iter != toCheck.end(); iter++)
    {
        getRelatedEntries(iter->first, iter->second, keysConstrained);
        if (keysConstrained.size() > maxConstraints)
        {
            maxConstraints = keysConstrained.size();
            toReturn = *iter;
        }
    }
    
    return toReturn;
}

std::list<Key>::iterator backtrackingSolver::find_in_list(Key find, std::list<Key> &toAssign)
{
    for (std::list<Key>::iterator iter = toAssign.begin(); iter != toAssign.end(); iter++)
    {
        if (*iter == find) return iter;
    }
    return toAssign.begin();
}

Key backtrackingSolver::selectUnassignedVariable()
{
    Key toReturn;
    std::list<Key>::iterator toErase;
    if (this->minRemVar)
    {
        std::list<Key> toCheck;
        int min = 36;
        for (std::list<Key>::iterator it = toAssign.begin(); it != toAssign.end(); it++)
        {
            if (this->constraintGraph[*it].size() < min)
            {
                min = this->constraintGraph[*it].size();
                toCheck.clear();
                toReturn = *it;
                toErase = it;
            }
            else if (this->constraintGraph[*it].size() == min)
            {
                toCheck.push_back(*it);
            }
        }
        if (toCheck.size() > 1 && this->degHeur)
        {
            toReturn = findHighestDegree(toCheck);
            toErase = find_in_list(toReturn, toAssign);
        }
        toAssign.erase(toErase);
    }
    else if (this->degHeur)
    {
        toReturn = findHighestDegree(toAssign);
        toErase = find_in_list(toReturn, toAssign);
        toAssign.erase(toErase);
    }
    else
    {
        toReturn = toAssign.front();
        toAssign.pop_front();
    }
    return toReturn;
}

char backtrackingSolver::selectUnassignedValue(int row, int column, KeySet &relatedEntries)
{
    char toUse;
    int score = 105;
    if(LCV)
    {
        for(Domain::iterator check = constraintGraph[Key(row, column)].begin(); check != constraintGraph[Key(row, column)].end(); check++)
        {
            int tempScore = getLCVScore(row, column, *check, relatedEntries);
            if(tempScore < score)
            {
                score = tempScore;
                toUse = *check;
            }
        }
    }
    else{
        return *(constraintGraph[Key(row, column)].begin());
    }
    return toUse;
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
    
    KeySet potentialChanges;
    getRelatedEntries(newVar.first, newVar.second, potentialChanges);
    
    Domain triedValues;
    
    while(!constraintGraph[newVar].empty())
    {
        char toUse = selectUnassignedValue(newVar.first, newVar.second, potentialChanges);
        triedValues.insert(toUse);
        constraintGraph[newVar].erase(toUse);

        try
        {
            if ( board->makeAssignment(newVar.first, newVar.second, toUse) )
            {
                if(forwardCheckingEnabled)
                    forwardCheck(newVar.first, newVar.second, toUse, fcPruned, potentialChanges);

                if (backTrackingSearch(level+1)) return true;
                    board->clearAssignment(newVar.first, newVar.second);

                if(forwardCheckingEnabled)
                {
                    replaceInDomain(fcPruned);
                    fcPruned.clear();
                }
            }
        }
        catch (NoRemainingVals e)
        {
            replaceInDomain(fcPruned);
            fcPruned.clear();
            board->clearAssignment(newVar.first, newVar.second);
        }
    }
    
    constraintGraph[newVar] = triedValues;
    toAssign.push_front(newVar);
    deadEnds++;
    return false;
}

CheckChange backtrackingSolver::removeFromDomain(Key entry, char toRemove){
    Domain::iterator domainEntry = constraintGraph[entry].find(toRemove);
        if(!(domainEntry == constraintGraph[entry].end())){
                if (constraintGraph[entry].empty()) throw (NoRemainingVals());
                constraintGraph[entry].erase(domainEntry);
                return CheckChange(entry, toRemove);
       }
}

void backtrackingSolver::getRelatedEntries(int row, int column, KeySet &relatedPairs)
{
    board->getBoxMembers(row, column, relatedPairs);
    board->getRowMembers(row, relatedPairs);
    board->getColMembers(column, relatedPairs);

    KeySet::iterator removeSelf = relatedPairs.find(Key(row, column));
    if(removeSelf != relatedPairs.end()) //If we forward check away the key it won't assign.
    {
        relatedPairs.erase(removeSelf);
    }
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

int backtrackingSolver::getLCVScore(int row, int column, char assigned, KeySet &relatedEntries)
{
    int score = 0;
    for(KeySet::iterator toCheck = relatedEntries.begin(); toCheck != relatedEntries.end(); toCheck++)
    {
        if(constraintGraph[(*toCheck)].find(assigned) != constraintGraph[(*toCheck)].end())
            score++;
    }
    return score;
}

void backtrackingSolver::forwardCheck(int row, int column, char assigned, std::list<CheckChange> &changeList, KeySet &potentialChanges)
{
 //   KeySet potentialChanges;
 // getRelatedEntries(row, column, potentialChanges);
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
