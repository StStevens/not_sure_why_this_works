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
		ConstraintGraph constraintGraph;
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
        bool degHeur;

        //Forward Checking
		void forwardCheck(int row, int column, char assigned, std::list<CheckChange> &changeList);

        // Degree Heuristic
        Key findHighestDegree(std::list<Key> &toCheck);

		void generateConstraintGraph();
        std::list<Key>::iterator find_in_list(Key find, std::list<Key> toAssign);
		Key selectUnassignedVariable();
		bool backTrackingSearch(int level);
		CheckChange removeFromDomain(Key entry, char toRemove);
		void replaceInDomain(std::list<CheckChange> toRestore);
		void getRelatedEntries(int row, int column, KeySet &relatedPairs);
		
		
	public:
		backtrackingSolver(SudokuBoard* toSolve, int maxTime, bool FC, bool ACP, bool MAC, bool MRV, bool DH, bool LCV);
        std::string generateOfp();
		void solve();
};

#endif
