#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

struct ElvesPair
{
	int elf1_b, elf1_e, elf2_b, elf2_e;
};
istream &operator >> (istream &is, ElvesPair &elvesPair);
ostream &operator << (ostream &os, ElvesPair &elvesPair);



int main (int argc, char **argv) {

    ifstream ifs_input ("input_04.txt");

    ElvesPair elvesPair;
    int int_sumFullOverlap = 0, int_sumPartialOverlap = 0;

    while (ifs_input >> elvesPair) {
        if (  ((elvesPair.elf1_b >= elvesPair.elf2_b) && (elvesPair.elf1_e <= elvesPair.elf2_e)) || ((elvesPair.elf2_b >= elvesPair.elf1_b) && (elvesPair.elf2_e <= elvesPair.elf1_e)) ){
            int_sumFullOverlap++;
            cout << "[DEBUG] : FULL overlap    : " << elvesPair << endl;
        }
        else if (!( (elvesPair.elf1_e < elvesPair.elf2_b) || (elvesPair.elf1_b > elvesPair.elf2_e) )){
            int_sumPartialOverlap++;
            cout << "[DEBUG] : PARTIAL overlap : " << elvesPair << endl;
        }
        else
            cout << "[DEBUG] : NO overlap      : " << elvesPair << endl;
    }

    cout << "Day 4 - Part1  anwswer : " << int_sumFullOverlap << endl;
    cout << "Day 4 - Part2  anwswer : " << int_sumFullOverlap + int_sumPartialOverlap << endl;

    return 0;
}


istream &operator >> (istream &is, ElvesPair &elvesPair)
{
    is >> elvesPair.elf1_b;
    is.ignore(1,'-');
    is >> elvesPair.elf1_e;
    is.ignore(1,',');
    is >> elvesPair.elf2_b;
    is.ignore(1,'-');
    is >> elvesPair.elf2_e;
	return is;
}

ostream &operator << (ostream &os, ElvesPair &elvesPair)
{
	os << "[" << elvesPair.elf1_b << "-" << elvesPair.elf1_e << "] -- [" << elvesPair.elf2_b << "-" << elvesPair.elf2_e << "]";
	return os;
}
