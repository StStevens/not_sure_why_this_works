#ifndef BACKTRACKINGSOLVER_H
#define BACKTRACKINGSOLVER_H

#include <map>
#include <set>
#include <unordered_set>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <iterator>
#include <ctime>
#include "SudokuBoard.h"


typedef std::pair<int, int> Key;
typedef std::unordered_set<char> Domain;
typedef std::map<Key, Domain> ConstraintGraph;
typedef std::pair< Key, char > CheckChange;

class backtrackingSolver
{
	private:
		SudokuBoard* board;
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

		//Config
		bool forwardCheckingEnabled;
        bool minRemVar;

		void generateConstraintGraph();
		Key selectUnassignedVariable();
		bool backTrackingSearch(int level);
		CheckChange removeFromDomain(Key entry, char toRemove);
		void replaceInDomain(std::list<CheckChange> toRestore);
		void forwardCheck(int row, int column, char assigned, std::list<CheckChange> &changeList);
		void getRelatedEntries(int row, int column, KeySet &relatedPairs);
		
		
	public:
        std::string generateOfp();
		backtrackingSolver(SudokuBoard* toSolve, int maxTime, bool FC, bool ACP, bool MAC, bool MRV, bool DH, bool LCV);
		ConstraintGraph constraintGraph;
		void solve();
};

#endif
