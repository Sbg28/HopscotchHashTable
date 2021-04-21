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

    // initialize the table itself
    void initializeTable();

    void addValue(int input);

    void removeValue(int input);

    int searchValue(int input);

    void printTable();

    void printBitmaps();

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
    void insert(int hashValue, int input);
    int findNextAvailable(int hashValue);
    bool openSpace(int desiredSpace, int& nextOpen, int& diff);
    void recursivelyOpenSpace(int& currentSpace);
    bool inNeighborhood(int homeValue, int searchValue, int& diff);
    void swapPositions(int i, int current);
    int search(int hashValue, int input);

};



#endif
