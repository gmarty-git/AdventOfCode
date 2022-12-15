#include <iostream>
#include "class_Map.hpp"

using namespace std;

int main (int argc, char **argv) {

    ifstream ifs_input ("input_12.txt");
    Map map(ifs_input);
    vector<int> reachedPositions;
    int firstLvlZero = -1;

    cout << "Day 12 - Part1  anwswer : " <<  map.getShortestRouteStepsCount(reachedPositions, firstLvlZero) << endl;
    cout << "Day 12 - Part2  anwswer : " << firstLvlZero << endl;
   
    return 0;
}
