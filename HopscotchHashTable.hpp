/*
 * Hopscotch Hashing
 * HopscotchHashTable.hpp
*/

#ifndef INCLUDED_HOPSCOTCHHASHTABLE_HPP
#define INCLUDED_HOPSCOTCHHASHTABLE_HPP

class HopscotchHashTable {
public:
    // Constructor {Default}
    HopscotchHashTable();
    // Constructor {length only}
    HopscotchHashTable(int length);
    // Constructot {length and neighborhoodsize provided}
    HopscotchHashTable(int length, int neighborhoodSize);

    ~HopscotchHashTable();

    // add a value to the table
    void addValue(int input);
    // remove a value from the table
    void removeValue(int input);
    // search for a specific value
    int searchValue(int input);
    // print the table contents
    void printTable();
    // print the bitmaps for each bucket
    void printBitmaps();

    // clear the table
    void clearTable();

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

    // initialize the table itself
    void initializeTable();

    int getValue(int index);
    void insert(int hashValue, int input);
    int findNextAvailable(int hashValue);
    bool openSpace(int desiredSpace, int& nextOpen, int& diff);
    void recursivelyOpenSpace(int& currentSpace);
    bool inNeighborhood(int homeValue, int searchValue, int& diff);
    void swapPositions(int i, int current);
    int search(int hashValue, int input, int& offset);
    void deleteEntry(int hashValue, int location, int offset);
    bool openNeighborhood(int homeValue);
    bool willLoop(int homeValue);
};

#endif
