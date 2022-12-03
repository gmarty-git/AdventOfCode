#include <iostream>
#include <algorithm>
#include <numeric>
#include "AdventOfCode_utils.hpp"

using namespace std;

int sum_TopNCalories (vector<string> strVect_split, int n);

int main (int argc, char **argv) {
    string str_input01 = readTxtFile2String("input_01.txt");
    vector<string> strVect_split = str_split(str_input01, '\n');

    cout << "Day 1 - Part1  anwswer : " << sum_TopNCalories(strVect_split, 1) << endl;
    cout << "Day 1 - Part2  anwswer : " << sum_TopNCalories(strVect_split, 3) << endl;

    return 0;
}

int sum_TopNCalories (vector<string> strVect_split, int n){
    int int_currentElfCalories=0;
    vector<int> intVec_topNCal(n);
    vector<int>::iterator  intAdr_topNCalMin;

    for(auto it = strVect_split.begin(); it != strVect_split.end(); ++it) {
        if (!(*it).empty()){
            int_currentElfCalories =  int_currentElfCalories + stoi(*it);
        }

        if (((*it).empty()) || ( (next(it)) == strVect_split.end() )) {
            intAdr_topNCalMin = min_element(intVec_topNCal.begin(), intVec_topNCal.end());
            if (int_currentElfCalories > *intAdr_topNCalMin) {
                *intAdr_topNCalMin = int_currentElfCalories;
            }
                int_currentElfCalories = 0;
        }
    }

    return accumulate(intVec_topNCal.begin(), intVec_topNCal.end(), 0);
}

