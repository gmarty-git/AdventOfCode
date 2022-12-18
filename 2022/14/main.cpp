#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
/*

#include <algorithm>
#include <utility>
#include <numeric>   */

using namespace std;

class Slice {
    public:
        Slice(unsigned int width, unsigned int height) {
            abyssHeight = 0;
            view_Xmin = 499;
            view_Xmax = 501;
            view_Ymin = 0;
            view_Ymax = 0;
            for (int i = 0 ; i <  height; ++i)
                grid.emplace_back(width, '.');
        }

        char& operator() (unsigned int x, unsigned int y) {
            return grid.at(y).at(x);
        }

        void readsRocksFromFile(string fileName) {
            ifstream ifs(fileName);
            stringstream ss;
            string str_line, str_buffer;
            unsigned int x_prev, y_prev, x, y, x_increment, y_increment;
       
            while (getline(ifs, str_line)){
                ss.str(str_line);
                ss.clear();
                ss >> x_prev;
                ss.ignore(1, ',');
                ss >> y_prev;
                if (y_prev > abyssHeight)
                    abyssHeight = y_prev;
                (*this)(x_prev,y_prev) = '#';

                view_Xmin = x_prev < view_Xmin ? x_prev : view_Xmin ;
                view_Xmax = x_prev > view_Xmax ? x_prev : view_Xmax ;
                view_Ymax = y_prev > view_Ymax ? y_prev : view_Ymax ;

                while (ss >> str_buffer) {
                    ss >> x;
                    ss.ignore(1, ',');
                    ss >> y;
                    if (y > abyssHeight)
                        abyssHeight = y;
                    x_increment = x > x_prev ? 1 : -1 ;
                    y_increment = y > y_prev ? 1 : -1 ;
                    for (int i_x = x_prev ; i_x != x + x_increment ; i_x += x_increment) {
                        for (int i_y = y_prev ; i_y != y + y_increment ; i_y += y_increment) {
                            (*this)(i_x, i_y) = '#';

                            view_Xmin = i_x < view_Xmin ? i_x : view_Xmin ;
                            view_Xmax = i_x > view_Xmax ? i_x : view_Xmax ;
                            view_Ymax = i_y > view_Ymax ? i_y : view_Ymax ;
                        }
                    }
                        x_prev = x;
                        y_prev = y;
                }
            }
        }

        bool generateSand(unsigned int unitsCount = 1) {
            bool sandReachedAbyss = false, obstructesSource = true, moved;
            unsigned int x, y;            

            for (int i = 0 ; i < unitsCount ; ++i){
                x=500, y=0;
                
                do {
                    moved = false;
                    if ((*this)(x, y+1) == '.'){
                        y++;
                        moved = true;
                    }
                    else if ((*this)(x-1, y+1) == '.'){
                        y++;
                        x--;
                        moved = true;
                    }
                    else if ((*this)(x+1, y+1) == '.'){
                        y++;
                        x++;
                        moved = true;
                    }
                    if (y >= abyssHeight) {
                        moved = false;
                        sandReachedAbyss = true;
                    }

                    if (moved)
                        obstructesSource = false;
                } while (moved);
            
                if (y < abyssHeight)
                    (*this)(x, y) = 'o';
             }  

            return !sandReachedAbyss && !obstructesSource;
        }
        
        ostream& print(ostream &os) {
            for (int i_y = view_Ymin ; i_y <= view_Ymax ; ++i_y) {
               for (int i_x = view_Xmin ; i_x <= view_Xmax ; ++i_x)
                  os << (*this)(i_x, i_y) ;

                os << endl;
                }               
            
            return os;
        }     

        void setFloor(unsigned int floorHeight = 0) {
            if (floorHeight == 0)
                floorHeight = abyssHeight + 2;
            for (auto it = grid.at(floorHeight).begin() ; it != grid.at(floorHeight).end() ; ++it)
                *it = '#';

            abyssHeight =  floorHeight          ;

            if (floorHeight > view_Ymax)
                view_Ymax =  floorHeight;
        }

    private:
       std::vector<vector<char>> grid;
       unsigned int abyssHeight, view_Xmin, view_Xmax, view_Ymin, view_Ymax;
};

unsigned int playSliceFile(string filename, bool enableFloor = false) {
    Slice caveSlice(1000, 200);
    caveSlice.readsRocksFromFile(filename);
    
    if (enableFloor)
        caveSlice.setFloor();

    unsigned int restedCount = 0;
    while (caveSlice.generateSand()) {
        restedCount++;
        //cout << endl ;
        //caveSlice.print(cout) << endl;
    }
    
    return restedCount;
}

int main (int argc, char **argv) {
   unsigned int restedSandCount, restedSandCountWithFloor;
   restedSandCount = playSliceFile("input_14.txt");
   restedSandCountWithFloor = playSliceFile("input_14.txt", true);  

   cout << "Day 14 - Part1  anwswer : " <<  restedSandCount << endl;
   cout << "Day 14 - Part2  anwswer : " <<  restedSandCountWithFloor + 1 << endl;
    
   exit(0);
}
