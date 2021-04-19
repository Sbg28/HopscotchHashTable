/*
 * Hopscotch Hashing
 * HopscotchHashTable.hpp
*/

#ifndef INCLUDED_HOPSCOTCHHASHTABLE_HPP
#define INCLUDED_HOPSCOTCHHASHTABLE_HPP

class HopscotchHashTable {
public:
    // Constructor {Default}
    HopscotchHasTable();
    // Constructor {length only}
    HopscotchHasTable(int length);
    // Constructot {length and neighborhoodsize provided}
    HopscotchHasTable(int length, int neighborhoodSize);
    // initialize the table itself
    initializeTable();

    void addValue(int input);

    void removeValue(int input);

    void searchValue(int input);

private:
    int neighborhoodSize_;
    int length_;
    struct Bucket {
        bool* bitmap = nullptr;
        bool filled;
        int value;
        int homeHashValue;
    };
    Bucket* table = nullptr;

    int getValue(int index);
    void insert(int hashValue);
    int findNextAvailable(int hashValue);
    bool openSpace(int desiredSpace, int& nextOpen, int& diff);
    void recursivelyOpenSpace(int& currentSpace);
    bool inNeighborhood(int homeValue, int searchValue, int& diff);

}



#endif
