/*
    Data Structures Project 3 test file
    DSProject3_test.cpp

*/

#include "HopscotchHashTable.hpp"
#include <iostream>

int main() {
    HopscotchHashTable table;
    std::cout << "INIT" << std::endl;
    table.addValue(1);
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

}


