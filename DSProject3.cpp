/*
    Data Structures Project 3 test file
    DSProject3_test.cpp

*/

#include "HopscotchHashTable.hpp"
#include <iostream>
#include <random>

int main() {
    const int TABLE_SIZE = 100;
    HopscotchHashTable table(TABLE_SIZE);

    for(int i = 0; i < TABLE_SIZE; ++i){
        table.addValue(rand() % 300);
    }
    table.printTable();
    table.printBitmaps();

/*     table.addValue(1);
    table.addValue(2);
    table.addValue(3);
    table.addValue(4);
    table.addValue(5);

    table.addValue(9);
    table.addValue(19);

    table.addValue(29);
    table.addValue(39);
    table.addValue(49);
    table.printTable();
    table.printBitmaps();
    std::cout << table.searchValue(1) << std::endl;
    std::cout << table.searchValue(2) << std::endl;
    std::cout << table.searchValue(3) << std::endl;
    std::cout << table.searchValue(4) << std::endl;

    table.removeValue(49);
    table.removeValue(19);
    table.removeValue(4);
    table.printTable();
    table.printBitmaps();

    table.clearTable();
    table.printTable();
    table.printBitmaps(); */

}


