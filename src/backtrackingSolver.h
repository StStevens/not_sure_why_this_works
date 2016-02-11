#ifndef BACKTRACKINGSOLVER_H
#define BACKTRACKINGSOLVER_H

#include <map>
#include <set>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <iterator>
#include <ctime>
#include "SudokuBoard.h"


typedef std::pair<int, int> Key;
typedef std::set<char> Domain;
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

		void generateConstraintGraph();
		Key selectUnassignedVariable();
		bool backTrackingSearch(int level);
		CheckChange removeFromDomain(Key entry, char toRemove);
		void replaceInDomain(std::list<CheckChange> toRestore);
		std::list<CheckChange> forwardCheck(int row, int column, char assigned);
		KeySet getRelatedEntries(int row, int column);
		
		
	public:
	        std::string generateOfp();
		backtrackingSolver(SudokuBoard* toSolve, int maxTime);
		ConstraintGraph constraintGraph;
		void solve();
};

#endif
