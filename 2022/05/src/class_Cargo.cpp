#include "class_Cargo.hpp"

Cargo::Cargo (std::ifstream& ifs)
{
    //Locate column Id row and deduce size of cargo
    while(ifs.get() != '1');
    std::streampos streamSavedPos = ifs.tellg();
    while(ifs.get() != '\n');
    width = (ifs.tellg() - streamSavedPos + 2) / 4; // +2 : first ' ' + \n | /4 : 4 char by crate
    height = ifs.tellg() / (4 * width) - 1;  //  4*width : number of char per line including \n

    //Fill cargo
    height  = width * height; //Extend cargo height to be able to contain all crates stacked in one column.
    int int_cargo_size = width * height;
    crates = new char[int_cargo_size * sizeof(char)];
    memset(crates, ' ', int_cargo_size);
    ifs.seekg(0);
    for (int i_crate = 0 ; i_crate <  height  ; i_crate++) { // height = size of input.txt grid
        ifs.ignore(1,'[');
        crates[int_cargo_size - (height) + i_crate] = ifs.get(); // height = size of input.txt grid
        ifs.ignore(2);
    }  
    
    //Detect height of each column
    colHeights = new int[width * sizeof(int)];
    for (int i_col = 1 ; i_col <= width ; i_col++) {
        colHeights[i_col-1] = height;
        for (int i_row = 1 ; i_row <= height ; i_row++) {
            if ((*this)(i_row, i_col) == ' ')
                colHeights[i_col-1]--;
            else
                break;
        }
    }

}

Cargo::~Cargo ()
{
    delete crates;
    delete colHeights;
}

Cargo* Cargo::move_crates( int src_col, int dest_col, int movesCount){
    for (int i = 0 ; i < movesCount ; i++) {
        if (colHeights[src_col-1] != 0) {
            (*this)(height - getColHeight(dest_col), dest_col) = (*this)(height - getColHeight(src_col)+1, src_col);
            (*this)(height - getColHeight(src_col)+1, src_col) = ' ';
            colHeights[src_col-1]--;
            colHeights[dest_col-1]++;
        }
    }

    return this;
}

Cargo* Cargo::move_crates_9001( int src_col, int dest_col, int movesCount){
    if (colHeights[src_col-1] >= movesCount) {
        for (int i = 0 ; i < movesCount ; i++) {
            (*this)(height - getColHeight(dest_col) - movesCount + i + 1, dest_col) = (*this)(height - getColHeight(src_col)+ 1 + i, src_col);
            (*this)(height - getColHeight(src_col)+ 1 + i, src_col) = ' ';
            
        }
            colHeights[src_col-1] -= movesCount;
            colHeights[dest_col-1] += movesCount;
    }

    return this;
}

int Cargo::getColHeight(int colId){
    return colHeights[colId-1];
}

char& Cargo::operator () (int row, int col) {
    return crates[(row - 1) * width + col - 1 ]; //Index from 1 as in input.txt
}

std::ostream& operator << (std::ostream &os, Cargo &cargo){

    int int_firstNonEmptyRow = cargo.height - (*std::max_element(cargo.colHeights, cargo.colHeights + cargo.width)) + 1;

    for (int i = int_firstNonEmptyRow; i <= cargo.height ; i++) {
        os << std::setw(4) << i << ": ";         

        for (int j=1 ; j <= cargo.width ; j++) {
            if (cargo(i,j) == ' ') 
                os << "    ";
            else
                os << "[" << cargo(i,j) << "] ";
        }
        
        os << std::endl;
    }
    
    os << std::setw(4) << "" << ": ";    
    for (int i=1 ; i <= cargo.width ; i++) {
        os <<std::setw(3) << cargo.getColHeight(i) << ' ';
    }
    os << " (col. height)"<< std::endl;

     os << std::setw(4) << "" << ": ";  
    for (int i=1 ; i <= cargo.width ; i++) {
        os << std::setw(3) << i << ' ';
    }
    os << " ( col. Id.)"<< std::endl;

    return os;
}
