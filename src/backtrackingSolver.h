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


typedef std::pair<int, int> Key;
typedef std::set<char> Domain;
typedef std::map<Key, KeySet> RelatedKeyMap;
typedef std::map<Key, Domain> ConstraintGraph;
typedef std::pair< Key, char > CheckChange;

class backtrackingSolver
{
	private:
		SudokuBoard* board;
		ConstraintGraph constraintGraph;
        RelatedKeyMap relatedEntries;
		std::list<Key> toAssign;
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
        int assgntsMade;

		// Config
		bool forwardCheckingEnabled;
        bool minRemVar;
        bool degHeur;
        bool LCV;
        bool pretty;
        bool ACP;

        // Forward Checking
		void forwardCheck(int row, int column, char assigned, std::list<CheckChange> &changeList, KeySet &potentialChanges);

        // LCV
        int getLCVScore(int row, int column, char assigned, KeySet &relatedEntries);

        // Degree Heursitic
        map_heap degreeMap;
      

		void generateConstraintGraph();
        void buildRelatedEntries(int row, int column, KeySet &constrainedKeys, bool getAssigned = false);
		Key selectUnassignedVariable();
        char selectUnassignedValue(int row, int column, KeySet &relatedEntries);
		bool backTrackingSearch(int level);
		CheckChange removeFromDomain(Key entry, char toRemove);
		void replaceInDomain(std::list<CheckChange> toRestore);
		void getRelatedEntries(int row, int column, KeySet &relatedPairs, bool getAssigned = false);
		
		
	public:
		backtrackingSolver(SudokuBoard* toSolve, int maxTime, bool FC, bool ACP, bool MAC, bool MRV, bool DH, bool LCV);
        std::string generateOfp();
		void solve();
};

#endif
