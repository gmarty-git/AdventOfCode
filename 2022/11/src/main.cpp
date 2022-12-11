#include <iostream>
#include "class_MonkeyGang.hpp"

using namespace std;

int main (int argc, char **argv) {

    ifstream ifs_input ("input_11.txt");
    MonkeyGang stupidMonkeys(ifs_input);
    cout << stupidMonkeys << endl;
    stupidMonkeys.resolveRounds(20);
    cout << "Day 11 - Part1  anwswer : " <<  stupidMonkeys.getMonkeyBusinessLevel() << endl;

    ifs_input.clear();
    ifs_input.seekg(0);
    MonkeyGang reallyStupidMonkeys(ifs_input);
    reallyStupidMonkeys.resolveRounds(10000, false);
    cout << "Day 11 - Part2  anwswer : " << reallyStupidMonkeys.getMonkeyBusinessLevel() << endl;
   
    return 0;
}
