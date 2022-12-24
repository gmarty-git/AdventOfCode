#ifndef CLASS_GRID_2D_YOM
#define CLASS_GRID_2D_YOM

#include <iostream>
#include <ostream>
#include <vector>
#include <utility>
#include <optional>


class Grid_2D
{
    public:
        Grid_2D(unsigned int width = 0, unsigned int heigth = 0, std::optional<int> x_topLeft = std::nullopt, std::optional<int> y_topLeft = std::nullopt);
        int getAllocatedHeight();
        int getAllocatedWidth();
        std::vector<char> getRow(int y);
        char& operator() (int x, int y);
        friend std::ostream& operator<< (std::ostream &os, Grid_2D &grid2D);

    private:
        std::vector< std::vector<char> > grid;
        std::optional<int> x_topLeft, y_topLeft;
        char gridInitValue;
};

#endif
