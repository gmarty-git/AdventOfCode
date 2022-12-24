#include "Grid_2D.hpp"

Grid_2D::Grid_2D(unsigned int width, unsigned int heigth, std::optional<int> x_topLeft, std::optional<int> y_topLeft){
    gridInitValue = '.';
    grid.resize(heigth, std::vector<char>(width, gridInitValue));
    this->x_topLeft = x_topLeft;
    this->y_topLeft = y_topLeft;          
}

int  Grid_2D::getAllocatedWidth() {
    if (grid.size() == 0)
        return 0;
    else
        return grid.at(0).size();
}

int  Grid_2D::getAllocatedHeight() {
    return grid.size();
}

char&  Grid_2D::operator() (int x, int y) {
   
    if (!this->x_topLeft.has_value())
        this->x_topLeft = x;
    
    if (!this->y_topLeft.has_value())
        this->y_topLeft = y;

    if (getAllocatedHeight() == 0)
        grid.push_back(std::vector<char>(1, gridInitValue));

    if ( x < x_topLeft.value() ){
        int newColCount = x_topLeft.value() - x;
        for (auto it = grid.begin() ; it != grid.end() ; ++it)
            it->insert(it->begin(), newColCount, gridInitValue);
            
        x_topLeft = x;
    }
    else if ( x > x_topLeft.value() + getAllocatedWidth() - 1){
        int newColCount = x - x_topLeft.value() - getAllocatedWidth() + 1;
        for (auto it = grid.begin() ; it != grid.end() ; ++it)
            it->insert(it->end(), newColCount, gridInitValue);
    }

    if ( y < y_topLeft.value() ) {
        int newRowCount = y_topLeft.value() - y;
        grid.insert(grid.begin(), newRowCount, std::vector<char>(getAllocatedWidth(), gridInitValue));
        y_topLeft = y;
    }
    else if ( y > y_topLeft.value()  + getAllocatedHeight() - 1 ){
        int newRowCount =  y - y_topLeft.value() - getAllocatedHeight() + 1;
        grid.insert(grid.end(), newRowCount, std::vector<char>(getAllocatedWidth(), gridInitValue));
    }
    
    return grid.at( y - y_topLeft.value() ).at( x - x_topLeft.value() );
}

std::vector<char> Grid_2D::getRow(int y) {
    return grid.at( y - y_topLeft.value() );
}

std::ostream& operator<< (std::ostream &os, Grid_2D &grid2D) {
    for ( auto it_row = grid2D.grid.begin() ; it_row != grid2D.grid.end() ; ++it_row){
        for ( auto it_col = it_row->begin() ; it_col != it_row->end() ; ++it_col){
            os << *it_col ;
        }
        os << std::endl;
    }
    return os;
}

//int main (int argc, char **argv) {
//    Grid_2D grid2D;
//    grid2D(2,18) = 'o';
//    grid2D(2,19) = 'o';
//    grid2D(2,19) = '+';
//    grid2D(8,-1) = '-';
//    grid2D(8,-1) = '+';
//    std::cout << grid2D << std::endl;
//    
//    exit(0);
//}
