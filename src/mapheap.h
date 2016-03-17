#ifndef MAPHEAP_H
#define MAPHEAP_H

#include <iostream>
#include <cstdlib>
#include <map>
#include <list>
#include <set>
#include <stdio.h>


 // typedefs
typedef std::pair<int, int> Key;
typedef std::pair<Key, int> KeyVal;
typedef std::set<Key> KeySet;
typedef std::map<Key, KeySet> RelatedKeyMap;

class map_heap
{
    std::map<Key, int> keyMap;  // Map for storing Key with related degree
    std::map<int, Key> valMap;  // Map for sorting Keys by degree

    public:
    map_heap();     // Constructor

    void fill_map_heap(std::map<Key, KeySet> &relatedEntries);  // access and update functions
    Key mapheap_top();
    void update_heap(KeySet &constrainedEntries, bool add);
    void fill_valMap(std::list<Key> &constrained);
    void print_heap(int type);
};

#endif
