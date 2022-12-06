#include <iostream>
#include "class_Cargo.hpp"

using namespace std;

int main (int argc, char **argv) {

    ifstream ifs_input ("input_05.txt");
    Cargo cargo(ifs_input);
    ifs_input.seekg(0);
    Cargo cargo_9001(ifs_input);
    cout << cargo_9001 << endl;

    string str_buffer ;
    int int_src_col, int_dest_col, int_movesCount;

    getline(ifs_input, str_buffer);
    while (ifs_input >> str_buffer >> int_movesCount >> str_buffer >> int_src_col >> str_buffer >> int_dest_col) {
        cout << endl << "[9000] : Moved " << int_movesCount << " crates from " << int_src_col << " to " << int_dest_col << ":" << endl;
        cargo.move_crates(int_src_col, int_dest_col, int_movesCount);
        cout << endl << cargo << endl;    

        cout << endl << "---"<< endl << "[9001] : Moved " << int_movesCount << " crates from " << int_src_col << " to " << int_dest_col << ":" << endl;
        cargo_9001.move_crates_9001(int_src_col, int_dest_col, int_movesCount);
        cout << endl << cargo_9001 << endl; 
    }
    
    return 0;
}

