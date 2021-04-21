/*
 * Hopscotch Hashing
 * HopscotchHashTable.cpp
*/
#include "HopscotchHashTable.hpp"
#include <string>
#include <iostream>

// constructor (default)
HopscotchHashTable::HopscotchHashTable(){
    neighborhoodSize_ = 5;
    length_ = 10;    
    initializeTable();
}

// constructor (specified length)
// @param length int Length of the table
HopscotchHashTable::HopscotchHashTable(int length){
    neighborhoodSize_ = 5;
    length_ = length;  
    initializeTable();
}

// constructor (specified length and neighborhood size)
// @param length int Length of the table
// @param neighborhoodSize int size of the bitmaps and neighborhoods for hopscotch
HopscotchHashTable::HopscotchHashTable(int length, int neighborhoodSize){
    neighborhoodSize_ = neighborhoodSize;
    length_ = length;  
    initializeTable();
}

// Destructor
HopscotchHashTable::~HopscotchHashTable(){

    // Create the bitmaps for each bucket
    for(int i = 0; i < length_; i++){
        delete table[i].bitmap;
    }
    delete table;
}

// initialize the table of buckets
void HopscotchHashTable::initializeTable(){
    table = new Bucket[length_];
    // Create the bitmaps for each bucket
    for(int i = 0; i < length_; i++){
        table[i].bitmap = new bool[neighborhoodSize_];
        for(int j = 0; j < neighborhoodSize_; j++){
            table[i].bitmap[j] = false;
        }
        table[i].value = -1;
        table[i].filled = false;
    }
}

// getValue
// get the value stored in a specific location in the table
// @param index location in the table
// @param return value at given index
int HopscotchHashTable::getValue(int index){
    if(index < 0 || index >= length_){
        return -1;
    }
    
    return table[index].value;
}

// addValue
// add a specific value to the table
// @param input int value to add
void HopscotchHashTable::addValue(int input){
    // Get Hash Value
    int hashValue = input % length_;
    // try insert value
    try{
        insert(hashValue, input);
    }
    catch (std::string e){
        std::cout << e << std::endl;
    }
}

// insert
// insert a value into the table
// @param hashValue int the hashed value of the input
// @param input int value to insert
void HopscotchHashTable::insert(int hashValue, int input){
    // diff is the difference between the homeValue and the actual location
    int diff = 0;

    if(table[hashValue].filled){    // If there is an entry in the home location
        int next = findNextAvailable(hashValue);    // Find next open space
        // if in neighborhood of home location : Store in found space
        if(inNeighborhood(hashValue, next, diff)){
            table[next].value = input;
            table[hashValue].bitmap[diff] = true;
            table[next].filled = true;
            table[next].homeHashValue = hashValue;
        }
        // if not in neighborhood of home   : Open space until there is an opening in the neighborhood and assign there
        else if(openSpace(hashValue, next, diff)){
            table[next].value = input;
            table[hashValue].bitmap[diff] = true;
            table[next].filled = true;
            table[next].homeHashValue = hashValue;
        }
        else{
            std::cout << "Throwing Error" << std::endl;
            throw "ERROR";
        }
    }
    else { // The home location is empty : Assign here
        table[hashValue].value = input;
        table[hashValue].bitmap[0] = true;
        table[hashValue].filled = true;
        table[hashValue].homeHashValue = hashValue;
    }
}

// findNextAvailable
// find the next open slot 
// @param homeValue int the start value for the search
// @param return int the location of next empty bucket
int HopscotchHashTable::findNextAvailable(int homeValue){
    int i = homeValue; //Start at the home Value
    bool loop = false;
    // While we are encountering filled spots
    while(table[i].filled){
        ++i;
        // if we have exceeded the length of the table : loop to beginning
        if(i >= length_){
            i = 0;
            loop = true;
        }
        else if (i == homeValue && loop == true)    // If we have looped and return back to the homeValue, there are no open spots
            return -1;
    }
    return i;
}

// openSpace
// maneuver entries to make space in home neighborhood
// @param homeValue int the slot whose neighborhood we want to open
// @param nextOpen int& the next open empty slot
// @param diff int& the offset for the bitmaps
// @param return bool success or not
bool HopscotchHashTable::openSpace(const int homeValue, int& nextOpen, int& diff){
    int currentSpace = nextOpen;    // Current location
    bool flag = true;
    // Check if there is even space in the home neighborhood bitmap
    for(int i = 0; i < neighborhoodSize_; ++i){
        if(table[homeValue].bitmap[i] == false)
            flag = false;
            break;
    }
    if(!flag)
        return false;

    bool loop = false;
    // if the desired neighborhood will loop to the beginning
    if(homeValue + neighborhoodSize_-1 >= length_){
        loop = true;

    }
    const int endNeighborhood = homeValue + neighborhoodSize_ - 1;   //End location of the home neighborhood
    // Loop until the currentSpace is within the neighborhood or the number of swaps exceeds a given value, typically the length of the table as no operation would require more swaps than elements. 
    int count = 0;
    while ((currentSpace > endNeighborhood || currentSpace < homeValue ) && count <= length_){

        recursivelyOpenSpace(currentSpace);
        if(inNeighborhood(homeValue, currentSpace, diff)){
            break;
        }
        count++;
    }
    if(count > length_)
        return false;
    // set the newest open space to the space we just made. 
    nextOpen = currentSpace;
    
    // Calculate the offset for the bitmap
    // if loop to beginning, need to adjust offset calculation
    if (nextOpen < homeValue)
        diff = nextOpen + length_ - homeValue;
    else
        diff = nextOpen - homeValue;

    return true;
}

// recursivelyOpenSpace(int& currentSpace)
// Moves a given empty space up the table through neighborhoods to get to a homeValue
// @param currentSpace location to move
void HopscotchHashTable::recursivelyOpenSpace(int& currentSpace){
    int current = currentSpace;
    if(current < neighborhoodSize_ - 1){
        current = current + length_;
    }
    // get start of current neighborhood
    int neighborhoodStart = current - neighborhoodSize_ + 1;
    int index;
    int diff;
    // for each item in current neighborhood, check if the current space is in its home neighborhood
    for(int i = neighborhoodStart; i < current; i++){
        if(i >= length_){
            index = i - length_;
        }
        else{
            index = i;
        }
        if(inNeighborhood(table[index].homeHashValue, current, diff)){
            swapPositions(i, currentSpace);
            currentSpace = i;
            return;
        }
    }
}

// inNeighborhood(int, int, int&)
// determines if a given value can be found within the neighborhood of the homevalue
// @param homeValue Home Value to search around
// @param searchValue position to check
// @param diff Offset of found value to the home value
// return true if found, false otherwise
bool HopscotchHashTable::inNeighborhood(int homeValue, int searchValue, int& diff){
    int index = 0;
    for(int i = 0; i < neighborhoodSize_; ++i){
        if(i + homeValue >= length_)
            index = (i + homeValue) - length_;
        else
            index = i + homeValue;
        if(index == searchValue){
            diff = i;
            return true;
        }
    }
    return false;
}

// swapPostions(int i, int current)
// swaps the positions of two values in the table with the current value being empty
// @param i location of first element
// @param current location of empty element
void HopscotchHashTable::swapPositions(int i, int current){
    // Save old values
    int tempVal = table[i].value;
    int tempHome = table[i].homeHashValue;

    // Calculate offsets for bitmaps
    int tempBit = i - tempHome;     // offset from home hash to old postion
    int tempOffset;
    if(current < tempHome){
        tempOffset = current + length_ - 1 - tempHome;
    }
    else{
        tempOffset = current - tempHome;    // offset from home hash to new position
    }
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

// printTable
// print the entire table to std::cout to view
void HopscotchHashTable::printTable(){
    for(int i = 0; i < length_; i++){
        std::cout << i << ": " << table[i].value << std::endl;
    }
}

// printBitmaps
// print the bitmaps for each bucket to std::cout to view
void HopscotchHashTable::printBitmaps(){
    for(int i = 0; i < length_; i++){
        std::cout << i << ": [";
        for(int j = 0; j < neighborhoodSize_; j++){
            std::cout << table[i].bitmap[j] << " ";
        }
        std::cout << "]" << std::endl;
    }
}

// removeValue
// remove a given value from the table
// @param input int value to remove
void HopscotchHashTable::removeValue(int input){
    // Get Hash Value
    int hashValue = input % length_;
    int offset = 0;
    // search
    try{
        int location = search(hashValue, input, offset);
        deleteEntry(hashValue, location, offset);
    }
    catch (std::string e){
        std::cout << e << std::endl;
    }
}

// searchValue
// search for a given value in the table
// @param input int value to search for
// @param return int the location of the value in the table
int HopscotchHashTable::searchValue(int input){
    // Get Hash Value
    int hashValue = input % length_;
    int offset = 0;
    // search
    try{
        return search(hashValue, input, offset);
    }
    catch (std::string e){
        std::cout << e << std::endl;
    }
}

// search
// search the bitmaps for a given home value for the desired value
// @param hashValue int homeValue of neighborhood
// @param input int value to search for
// @param offset int& bitmap offset for the found value
// @param return int location of the value in the table
int HopscotchHashTable::search(int hashValue, int input, int& offset){
    int index = 0;
    int loopAfter = length_;
    if(neighborhoodSize_ + hashValue > length_){
        loopAfter = length_ - hashValue - 1;
    }
    for (int i = 0; i < neighborhoodSize_; ++i){
        if(table[hashValue].bitmap[i] == true){
            if(i > loopAfter){
                index = i - loopAfter - 1;
            }
            else{
                index = hashValue + i;
            }
            if(table[index].value == input){
                offset = i;
                return index;
            }
        }
    }
    return -1;
}

// deleteEntry
// delete a specific value from the table
// @param hashValue int home value for entry
// @param location int location of desired entry to remove
// @param offset int bitmap offset between the location and the home hash value
void HopscotchHashTable::deleteEntry(int hashValue, int location, int offset){
    table[hashValue].bitmap[offset] = false;
    table[location].value = -1;
    table[location].homeHashValue = -1;
    table[location].filled = false;
}

// clearTable
// reset the table of all entries
void HopscotchHashTable::clearTable(){
    for(int i = 0; i < length_; ++i){
        table[i].value = -1;
        table[i].filled = false;
        table[i].homeHashValue = -1;
        for(int j = 0; j < neighborhoodSize_; ++j){
            table[i].bitmap[j] = false;
        }
    }
}
