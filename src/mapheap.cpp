#include "mapheap.h"

map_heap::map_heap() {}

void map_heap::fill_map_heap(RelatedKeyMap &relatedEntries)
{
    for (RelatedKeyMap::iterator iter = relatedEntries.begin(); iter != relatedEntries.end(); iter++)
        keyMap[iter->first] = (iter->second).size();
}

Key map_heap::mapheap_top()
{
    if (valMap.empty()) return Key{-1, -1};

    std::map<int, Key>::reverse_iterator riter = valMap.rbegin();
    Key top = riter->second;
    return top;
}

void map_heap::fill_valMap(std::list<Key> &constrained)
{
    valMap.clear(); 
    for (std::list<Key>::iterator iter = constrained.begin(); iter != constrained.end(); iter++)
        valMap[keyMap[*iter]] = *iter;
}

void map_heap::update_heap(KeySet &constrainedEntries, bool add)
{
    for (KeySet::iterator iter = constrainedEntries.begin(); iter != constrainedEntries.end(); iter++)
        (add) ? keyMap[*iter]++ : keyMap[*iter]--;
}

void map_heap::print_heap(int type)
{
    if (type == 2 || type == 0)
    {
        printf("\nkeyMap:\n");
        for (std::map<Key, int>::iterator iter = keyMap.begin(); iter != keyMap.end(); iter++)
            printf("%d, %d:     %d\n", (iter->first).first, (iter->first).second, iter->second);
    }
    if (type == 2 || type == 1)
    {
        printf("\nvalMap:\n");
        for (std::map<int, Key>::iterator it = valMap.begin(); it != valMap.end(); it++)
            printf("%d:     %d, %d\n", it->first, (it->second).first, (it->second).second);
    }
}
