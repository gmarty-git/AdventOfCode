#include <iostream>
#include <map>
#include "AdventOfCode_utils.hpp"

using namespace std;

int eval_score (vector<string> strVect_split, bool bool_part2Modifier);

int main (int argc, char **argv) {
    string str_input02 = readTxtFile2String("input_02.txt");
    vector<string> strVect_split = str_split(str_input02, '\n');

    cout << "Day 2 - Part1  anwswer : " << eval_score(strVect_split, false) << endl;
    cout << "Day 2 - Part2  anwswer : " << eval_score(strVect_split, true) << endl;

    return 0;
}

int eval_score (vector<string> strVect_split, bool bool_part2Modifier){
    int int_eval_score=0;

    map <string, int> map_roundScore;
    map_roundScore["A X"] = 4;
    map_roundScore["B X"] = 1;
    map_roundScore["C X"] = 7;
    map_roundScore["A Y"] = 8;
    map_roundScore["B Y"] = 5;
    map_roundScore["C Y"] = 2;
    map_roundScore["A Z"] = 3;
    map_roundScore["B Z"] = 9;
    map_roundScore["C Z"] = 6;

    map <string, string> map_roundSymbol;
    if (bool_part2Modifier) {
        map_roundSymbol["A X"] = "A Z";
        map_roundSymbol["B X"] = "B X";
        map_roundSymbol["C X"] = "C Y";
        map_roundSymbol["A Y"] = "A X";
        map_roundSymbol["B Y"] = "B Y";
        map_roundSymbol["C Y"] = "C Z";
        map_roundSymbol["A Z"] = "A Y";
        map_roundSymbol["B Z"] = "B Z";
        map_roundSymbol["C Z"] = "C X";
    }


    for(auto it = strVect_split.begin(); it != strVect_split.end(); ++it)
        if (!bool_part2Modifier) {
            int_eval_score += map_roundScore[(*it)];
        }
        else {
            int_eval_score += map_roundScore[map_roundSymbol[(*it)]];
        }

    return int_eval_score;

}

