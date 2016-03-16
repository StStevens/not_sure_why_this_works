#include <algorithm>
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
                Key newKey = Key(i, j);
                toAssign.push_back(newKey); // Should move this later
                buildRelatedEntries(i, j, relatedEntries[newKey]);
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

Key backtrackingSolver::selectUnassignedVariable()
{
    Key toReturn;
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
            }
            else if (this->degHeur && this->constraintGraph[*it].size() == min)
                toCheck.push_back(*it);
        }
        if (this->degHeur && toCheck.size() > 1)
        {
            degreeMap.fill_valMap(toCheck);
            toReturn = degreeMap.mapheap_top();
            toAssign.remove(toReturn);
        }
        toAssign.remove(toReturn);
    }
    else if (this->degHeur)
    {
        degreeMap.fill_valMap(toAssign);
        toReturn = degreeMap.mapheap_top();
        toAssign.remove(toReturn);
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
            if((tempScore < score) || (tempScore == score && *check < toUse))
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
    int input;

    Key newVar = selectUnassignedVariable();
    nodeCount++;
    
    KeySet potentialChanges;
    KeySet constrained;
    getRelatedEntries(newVar.first, newVar.second, potentialChanges);
    
    Domain triedValues;
    while(!constraintGraph[newVar].empty())
    {
        char toUse = selectUnassignedValue(newVar.first, newVar.second, potentialChanges);
        triedValues.insert(toUse);

        try
        {
            if ( board->makeAssignment(newVar.first, newVar.second, toUse) )
            {
                if(forwardCheckingEnabled)
                    forwardCheck(newVar.first, newVar.second, toUse, fcPruned, potentialChanges);

                if (degHeur)
                {
                    buildRelatedEntries(newVar.first, newVar.second, constrained);
                    degreeMap.update_heap(constrained, false);
                }

                if (backTrackingSearch(level+1)) return true;
                board->clearAssignment(newVar.first, newVar.second);

                if(forwardCheckingEnabled)
                {
                    replaceInDomain(fcPruned);
                    fcPruned.clear();
                }
            }
            constraintGraph[newVar].erase(toUse);
        }
        catch (NoRemainingVals e)
        {
            replaceInDomain(fcPruned);
            fcPruned.clear();
            constraintGraph[newVar].erase(toUse);
            board->clearAssignment(newVar.first, newVar.second);
        }
    }
    
    if (degHeur) degreeMap.update_heap(constrained, true);
    constraintGraph[newVar] = triedValues;
    toAssign.push_front(newVar);
    deadEnds++;
    return false;
}

void backtrackingSolver::getRelatedEntries(int row, int column, KeySet &relatedPairs, bool getAssigned)
{
    relatedPairs = relatedEntries[Key(row, column)];
    return;
}


void backtrackingSolver::buildRelatedEntries(int row, int column, KeySet &constrainedKeys, bool getAssigned)
{
    board->getBoxMembers(row, column, constrainedKeys, getAssigned);
    board->getRowMembers(row, constrainedKeys, getAssigned);
    board->getColMembers(column, constrainedKeys, getAssigned);

    KeySet::iterator removeSelf = constrainedKeys.find(Key(row, column));
    if(removeSelf != constrainedKeys.end()) //If we forward check away the key it won't assign.
    {
        constrainedKeys.erase(removeSelf);
    }
}

void backtrackingSolver::replaceInDomain(std::list<CheckChange> toRestore)
{
    for(std::list<CheckChange>::iterator entry = toRestore.begin(); entry !=  toRestore.end(); entry++)
    {
        Key toFix = entry->first;
        char toAdd = entry->second;
        constraintGraph[toFix].insert(toAdd);
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

CheckChange backtrackingSolver::removeFromDomain(Key entry, char toRemove){
    Domain::iterator domainEntry = constraintGraph[entry].find(toRemove);
        if(!(domainEntry == constraintGraph[entry].end())){
                if (constraintGraph[entry].size() == 1){
                   throw (NoRemainingVals());
                }
                
constraintGraph[entry].erase(domainEntry);
                return CheckChange(entry, toRemove);
       }
}

void backtrackingSolver::forwardCheck(int row, int column, char assigned, std::list<CheckChange> &changeList, KeySet &potentialChanges)
{
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
    
    if (degHeur)
    {
        degreeMap.fill_map_heap(relatedEntries);
    }

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
