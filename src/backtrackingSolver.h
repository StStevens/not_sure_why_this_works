#ifndef BACKTRACKINGSOLVER_H
#define BACKTRACKINGSOLVER_H

#include <map>
#include <set>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include <iterator>
#include <algorithm>
#include <queue>
#include <ctime>
#include "SudokuBoard.h"
#include "mapheap.h"


 // typedefs
typedef std::pair<int, int> Key;
typedef std::set<char> Domain;
typedef std::map<Key, KeySet> RelatedKeyMap;    // map of keys constrained with each other
typedef std::map<Key, Domain> ConstraintGraph;
typedef std::pair< Key, char > CheckChange;     // undo changes

class backtrackingSolver
{
	private:
        // Constraint graph and board
		SudokuBoard* board;
		ConstraintGraph constraintGraph;
        RelatedKeyMap relatedEntries;
		std::list<Key> toAssign;

        // output file variables
        int timeout;
		time_t startTime;
		time_t prepStartTime;
		time_t endPrepTime;
		time_t searchStartTime;
		time_t endSearchTime;
		time_t endTime;
       	bool hasSolution;
       	int deadEnds;
	    int nodeCount;
	    std::string endResult;

		// Config
		bool forwardCheckingEnabled;
        bool minRemVar;
        bool degHeur;
        bool LCV;

        // Forward Checking
		void forwardCheck(char assigned, std::list<CheckChange> &changeList, KeySet &potentialChanges);
		CheckChange removeFromDomain(Key entry, char toRemove);

        // Least Constraining Variable
        int getLCVScore(char assigned, KeySet &relatedEntries);

        // Degree Heursitic
        map_heap degreeMap;
      

        // Member Functions
		void generateConstraintGraph();
        void buildRelatedEntries(int row, int column, KeySet &constrainedKeys, bool getAssigned = false);
		void getRelatedEntries(int row, int column, KeySet &relatedPairs, bool getAssigned = false);

		Key selectUnassignedVariable();
        char selectUnassignedValue(int row, int column, KeySet &relatedEntries);

		bool backTrackingSearch(int level);
		void replaceInDomain(std::list<CheckChange> &toRestore);
		
		
	public:
		backtrackingSolver(SudokuBoard* toSolve, int maxTime, bool FC, bool MRV, bool DH, bool LCV);  // Constructor
        std::string generateOfp();   // generatres the string for the output file
		void solve();
};

#endif
