#ifndef CLASS_ROPE_YOM
#define CLASS_ROPE_YOM

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>


struct Coord {int x, y;};
int64_t packCoord__int64_t(Coord coord);

class Knot
{private:
    Coord coord;

 public:
    Knot ();
    Knot* move(int x_dir, int y_dir);
    Knot* move(char direction, int mvCount=1);  
    Coord getCoord();
    Coord getDistanceTo(const Knot &k2);
};

class Rope
{private:
	Knot k_head, k_tail;    

 public:
    Rope* moveHead(int x_dir, int y_dir);
    Rope* moveHead(char direction, int mvCount=1);
    Knot& getTail();
    Knot& getHead();
};

class ChainedRope
{private:
    std::vector<Rope> links;

 public :
    ChainedRope(unsigned int knotsCount);
    ChainedRope* moveHead(char direction, int mvCount=1);
    Knot& getTail();
    Knot& getHead();
};

std::ostream& operator<< (std::ostream &os, Rope &rope);
int sign(int x);
#endif
