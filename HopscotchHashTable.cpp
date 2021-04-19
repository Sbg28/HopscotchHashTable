/*
 * Hopscotch Hashing
 * HopscotchHashTable.cpp
*/
#include "HopscotchHashTable.hpp"

HopscotchHashTable::HopscotchHashTable(): neighborhoodSize_(5), length_(10){
    intializeTable();
}

HopscotchHashTable::HopscotchHashTable(int length): neighborhoodSize_(5), length_(length){
    intializeTable();
}

HopscotchHashTable::HopscotchHashTable(int length, int neighborhoodSize): neighborhoodSize_(neighborhoodSize), length_(length){
    intializeTable();
}

void HopscotchHashTable::initializeTable(){
    table = new Bucket[length_];
    for(int i = 0; i < length_; i++){
        table[i].bitmap = new bool[neighborhoodSize_];
    }
}

int HopscotchHashTable::getValue(int index){
    return table[index].value;
}

void HopscotchHashTable::addValue(int input){
    int hashValue = input % length_;
    try{
        insert(hashValue, input);
    }
    catch {

    }
}

void HopscotchHashTable::insert(int hashValue, int input){
    int diff = 0;
    if(table[hashValue].filled){
        int next = findNextAvailable(hashValue);
        // if in neighborhood of home
        if(inNeighborhood(hashValue, next, diff)){
            table[next].value = input;
            table[hashValue].bitmap[diff] = true;
            table[next].filled = true;
            table[next].homeHashValue = hashValue;
        }
        // if not in neighborhood of home
        else if(openSpace(hashValue, next, diff)){
            table[next].value = input;
            table[hashValue].bitmap[diff] = true;
            table[next].filled = true;
            table[next].homeHashValue = hashValue;
        }
        else 
            throw "ERROR";
    }

    table[hashValue].value = input;
    table[hashValue].bitmap[0] = true;
    table[hashValue].filled = true;
    table[hashValue].homeHashValue = hashValue;
}

int HopscotchHashTable::findNextAvailable(int hashValue){
    int i = hashValue;
    bool loop = false;
    while(table[i].filled){
        ++i;
        if(i > length_){
            i = 0;
            loop = true;
        }
        else if (i == hashValue && loop == true)
            return -1
    }
    return i;
}

bool HopscotchHashTable::openSpace(const int desiredSpace, int& nextOpen, int& diff){
    int currentSpace = nextOpen;
    const int endNeighborhood = desiredSpace + neighborhoodSize_;
    while (currentSpace > endNeighborhood || currentSpace < desiredSpace){
        recursivelyOpenSpace(currentSpace);

        if(/*If Stalling*/){return false;}

    }
    nextOpen = currentSpace;
    
    // if loop to beginning
    if (nextOpen < desiredSpace)
        diff = nextOpen + length_ - desiredSpace;
    else
        diff = nextOpen - desiredSpace;

    return true;
}

void HopscotchHashTable::recursivelyOpenSpace(int& currentSpace){
    int current = currentSpace;
    int neighborhoodStart = current - neighborhoodSize_ - 1;
    for(int i = neighborhoodStart; i < current; i++){
        if(table[i].homeHashValue + neighborhoodSize_ >= current){
            swapPositions(i, current);
            currentSpace = i;
            return;
        }
    }
}

bool HopscotchHashTable::inNeighborhood(int homeValue, int searchValue, int& diff){

    for(int i = 0; i < neighborhoodSize_; ++i){
        if(table[homeValue + i].value == searchValue){
            diff = i;
            return true;
        }
    }
    return false;
}


void HopscotchHashTable::swapPostions(int i, int current){
    // Save old values
    int tempVal = table[i].value;
    int tempHome = table[i].homeHashValue;

    // Calculate offsets for bitmaps
    int tempBit = i - tempHome - 1;     // offset from home hash to old postion
    int tempOffset = current - tempHome - 1;    // offset from home hash to new position

    // populate in empty location
    table[current].value = table[i].value;
    table[current].homeHashValue = table[i].homeHashValue;
    table[current].filled = true;

    // clear from old location
    table[i].filled = false;
    table[i].homeHashValue = 0;

    // edit the bitmap for home
    table[tempHome].bitmap[tempBit] = false;
    table[tempHome].bitmap[tempOffset] = true;
}