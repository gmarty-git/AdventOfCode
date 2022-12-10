#include <iostream>
#include <fstream>
#include <set>
#include "class_Rope.hpp"

using namespace std;

int main (int argc, char **argv) {

    ifstream ifs_input ("input_09.txt");
    Rope rope;
    ChainedRope chain(10);
    char direction;
    int  movesCount;
    int32_t coord_32b_x, coord_32b_y;
    int64_t coord_64b;
    set<int64_t> visitedCoord_rope, visitedCoord_chain;

    while (ifs_input >> direction >> movesCount) {
        for (int i = 0 ; i < movesCount ; ++i) {  //Loop over moves one by one to be able to see 'intermediates' tail positions   
            rope.moveHead(direction);      
            visitedCoord_rope.insert(packCoord__int64_t(rope.getTail().getCoord()));
       
            chain.moveHead(direction); 
            visitedCoord_chain.insert(packCoord__int64_t(chain.getTail().getCoord()));
        }
    }

    cout << "Day 9 - Part1  anwswer : " << visitedCoord_rope.size() << endl;
    cout << "Day 9 - Part2  anwswer : " << visitedCoord_chain.size() << endl;
   
    return 0;
}
