#ifndef CLASS_CARGO_YOM
#define CLASS_CARGO_YOM

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <iomanip>

class Cargo
{private:
	char* crates;
    int height, width;
    int* colHeights;
    

 public:
    Cargo(std::ifstream& ifs_input);
    ~Cargo ();
    int getColHeight(int colId);
    Cargo* move_crates(int src_col, int dest_col, int movesCount);
    Cargo* move_crates_9001(int src_col, int dest_col, int movesCount);
    friend std::ostream &operator << (std::ostream &os, Cargo &cargo);
    char& operator () (int row, int col);
};

#endif
