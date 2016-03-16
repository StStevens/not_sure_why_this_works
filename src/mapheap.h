#ifndef MAPHEAP_H
#define MAPHEAP_H

#include <iostream>
#include <cstdlib>
#include <map>
#include <list>
#include <set>
#include <stdio.h>

typedef std::pair<int, int> Key;
typedef std::pair<Key, int> KeyVal;
typedef std::set<Key> KeySet;
typedef std::map<Key, KeySet> RelatedKeyMap;

class map_heap
{
    std::map<Key, int> keyMap;
    std::map<int, Key> valMap;    

    public:
    map_heap();
    void fill_map_heap(std::map<Key, KeySet> &relatedEntries);
    Key mapheap_top();
    void update_heap(KeySet &constrainedEntries, bool add);
    void fill_valMap(std::list<Key> &constrained);
    void print_heap(int type);
};

#endif
