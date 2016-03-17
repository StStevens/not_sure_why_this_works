#include <algorithm>
#include "backtrackingSolver.h"


class TimeOutError{};
class NoRemainingVals{};


/**
 * Constructor for our solver.
 * 
 * Builds the Solver and initializes the constraint graph.
 *
 * @param[in]   SudokuBoard* toSolve Pointer to the SudokuBoardObject being solved
 * @param[in]   int::maxTime         The max time to spend solving in seconds
 * @param[in]   bool::FC             Enable forward checking
 * @param[in]   bool::MRV            Enable Minimum Remaining Value
 * @param[in]   bool::DH             Enable Degree Heuristic
 * @param[in]   bool::LCV            Enable Least Constraining Value Heuristic
 *
 * @returns None
 */
backtrackingSolver::backtrackingSolver(SudokuBoard* toSolve, int maxTime, bool FC, bool MRV, bool DH, bool LCV)
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

/**
 * Builds the constraint
 *
 * Builds the constraint graph and maps cordinates to their domains.
 * Also builds the map of cordiantes to their related entries.
 */
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

/**
 * Generates the string representing the output file
 * for the board.
 *
 * @returns str The output file as a string
 */
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


/**
 * Selects the next unassigned variable.
 *
 * If no heuristics are selected this move left to right
 * top to bottom.
 *
 * @returns Key Contains the (row, column) location to asssign
 */
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
/**
 * Gets the next value to assign
 *
 * Selects the next value to asssign for the given variable.
 * If no heuristics starts with the lowest values.
 *
 * @param[in]   int::row    The row of the variable
 * @param[in]   int::column The column of the variable
 * @param[in]   KeySet::&relatedEntries Referenced to the set of related entries created earlier
 *
 * @returns char The domain value of the variable to asssign to it.
 */
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

/**
 * Does the actual search for a solution
 *
 * Recurisved assings values and attempts to create a complete and
 * consistent board.
 *
 * @param[in] int::level    Indicates how deep we are in the search
 *
 * @returns bool    Returns true if a consistent & complete solution was found.
 */
bool backtrackingSolver::backTrackingSearch(int level)
{
    time(&endTime); //The current time
    if (endTime - startTime >= timeout) throw (TimeOutError()); //Check if we've run out of time

    //If we have nothing to assign we've solved the board
    if (toAssign.empty())
        return true;
    
    //List of Changes that have been made
    std::list<CheckChange> fcPruned;

    //Selects the next variable to assign
    Key newVar = selectUnassignedVariable();
    nodeCount++;
    
    KeySet potentialChanges;
    KeySet constrained;
    //Places all the related board locations into potentialChanges
    getRelatedEntries(newVar.first, newVar.second, potentialChanges);
    
    //Keeps track of the domain values already tried.
    Domain triedValues;
    //While we still have domain values left, keep trying
    while(!constraintGraph[newVar].empty())
    {
        //Get the next domain value to try.
        char toUse = selectUnassignedValue(newVar.first, newVar.second, potentialChanges);
        //Track the used ones
        triedValues.insert(toUse);

        try
        {   //Attempt to make the assigment
            if ( board->makeAssignment(newVar.first, newVar.second, toUse) )
            {
                //Run forward checking if enabled
                if(forwardCheckingEnabled)
                    forwardCheck(newVar.first, newVar.second, toUse, fcPruned, potentialChanges);
                
                //Keep degree Heuristic up to date if enabled
                if (degHeur)
                {
                    buildRelatedEntries(newVar.first, newVar.second, constrained);
                    degreeMap.update_heap(constrained, false);
                }

                //Try to assign the next level. If we can return true and go back up.
                if (backTrackingSearch(level+1)) return true;

                //If we fail clear the asssignment.
                board->clearAssignment(newVar.first, newVar.second);

                //If we failed and forward is enabled un do the pruning
                if(forwardCheckingEnabled)
                {
                    replaceInDomain(fcPruned);
                    fcPruned.clear();
                }
            }
            //Remove the variable from the list to try
            constraintGraph[newVar].erase(toUse);
        }
        catch (NoRemainingVals e) //If we foward check away the last var. Undo changes
        {
            replaceInDomain(fcPruned);
            fcPruned.clear();
            constraintGraph[newVar].erase(toUse);
            board->clearAssignment(newVar.first, newVar.second);
        }
    }
    //Updated our degree heuristic tracker.
    if (degHeur) degreeMap.update_heap(constrained, true);
    //Place the values back in the constraint graph.
    constraintGraph[newVar] = triedValues;
    //Put the variable back on the list of things to assign
    toAssign.push_front(newVar);
    //This branch failed
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
