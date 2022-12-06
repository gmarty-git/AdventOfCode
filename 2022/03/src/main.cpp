#include <iostream>
#include <algorithm>
#include <set>
#include "AdventOfCode_utils.hpp"

using namespace std;

int getPriority(char c);


int main (int argc, char **argv) {
    string str_input = readTxtFile2String("input_03.txt");
    vector<string> strVect_split = str_split(str_input, '\n');

    //Part 1
    //#############################################################################
    int int_sumP = 0;
    string::iterator it_dupplicatedPrio;
    for (auto it = strVect_split.begin() ; it != strVect_split.end(); ++it) {
        it_dupplicatedPrio = find_first_of( (*it).begin(), (*it).begin() + ((*it).size())/2, (*it).begin() + ((*it).size())/2, (*it).end());
        int_sumP += getPriority(*it_dupplicatedPrio);
    }

    cout << "Day 3 - Part1  anwswer : " << int_sumP << endl;
    //#############################################################################

    //Part 2
    //#############################################################################
    int_sumP = 0;

    vector<string>::iterator it_shortestStr;
    for (auto it_grp = strVect_split.begin() ; it_grp != strVect_split.end(); it_grp=it_grp+3) {

        //List items(char) contains in each bag of current group, without duplication
        set<char> set_bag1_content, set_bag2_content, set_bag3_content;
        set_bag1_content.insert((*it_grp).begin(), (*it_grp).end());
        set_bag2_content.insert((*(it_grp+1)).begin(), (*(it_grp+1)).end());
        set_bag3_content.insert((*(it_grp+2)).begin(), (*(it_grp+2)).end());


        for (auto it_char=set_bag1_content.begin() ; it_char != set_bag1_content.end() ; ++it_char) {
            if (auto it_find = set_bag2_content.find(*it_char); it_find != set_bag2_content.end()){
                if (auto it_find = set_bag3_content.find(*it_char); it_find != set_bag3_content.end()) {
                    int_sumP += getPriority(*it_char);
                    break;
                }
            }
        }
    }

    cout << "Day 3 - Part2  anwswer : " << int_sumP << endl;
    //#############################################################################

    return 0;
}

int getPriority(char c) {

    //Upper case letters are before lower case ones in ASCII table, but there are not consecutive

    if ( ((int) c < (int) 'A') || ((int) c > (int) 'z') || ( ((int) c > (int) 'Z') && ((int) c < (int) 'a')) ) {
        cout << "getPriority : specified char '" << c << "' is outside expected range [a-zA-A]" << endl;
        exit;
    }

    if ((int) c < (int) 'a') {
        return 27 + (int) c - (int) 'A' ;
    }
    else {
        return 1 + (int) c - (int) 'a' ;
    }
}
