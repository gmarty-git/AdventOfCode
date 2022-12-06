#include <iostream>
#include <set>
#include "AdventOfCode_utils.hpp"

using namespace std;

int findFirst_startMarker(string str_input, int startMarkerLength);

int main (int argc, char **argv) {

    string str_input = readTxtFile2String("input_06.txt");
    cout << "Day 6 - Part1  anwswer : " << findFirst_startMarker(str_input, 4) << endl;
    cout << "Day 6 - Part2  anwswer : " << findFirst_startMarker(str_input, 14) << endl;
   
    return 0;
}


int findFirst_startMarker(string str_input, int startMarkerLength) {
    set<char> set_char;
    
    for (auto it = str_input.begin() ; it != str_input.end() - startMarkerLength + 1 ; ++it)  {
        set_char.insert(it, it + startMarkerLength);
        if (set_char.size() == startMarkerLength)
           return distance(str_input.begin(), it + startMarkerLength);
        
        set_char.clear();
    }

    return 0;
}
