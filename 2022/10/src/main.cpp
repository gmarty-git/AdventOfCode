#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

int main (int argc, char **argv) {

    ifstream ifs_input ("input_10.txt");
    string cmd = "";
    int X = 1, V = 0;
    unsigned int cycle = 1, sigStrenghtSum = 0, currentPixHpos = 0;
    bool addxLock = false;
    char screenBuffer[240];
    fill(screenBuffer, screenBuffer + 240, '.');

    while (cycle <= 240) {
        if (cycle%40 == 20)//Part1
            sigStrenghtSum += cycle * X;

        currentPixHpos = (cycle-1)%40;
        if ( (currentPixHpos == X-1) || (currentPixHpos == X)  || (currentPixHpos == X+1)  ) //Part2
            screenBuffer[cycle-1] = '#';

        if (addxLock) {
            X += V;
            addxLock = false;
        }
        else {
            ifs_input >> cmd;
            if (cmd == "addx"){
                ifs_input >> V;
                addxLock =true;
            }
        }
        
        cycle++; 
    }
    

    cout << "Day 10 - Part1  anwswer : " << sigStrenghtSum << endl;
    cout << "Day 10 - Part2  anwswer : " << "" << endl;

     //ScreenRendering
    string str_border = " -- ";
    for (int i = 0 ; i < 240 ; ++i) {
        if (i%40 == 0)
            cout << str_border;

        cout << screenBuffer[i] ;
        
        if (i%40 == 39)
            cout << str_border << endl;
    } 
    cout << endl;
   
    return 0;
}
