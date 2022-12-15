#ifndef CLASS_MAP_YOM
#define CLASS_MAP_YOM

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

struct Coord {
    int x, y;
};

class Position {
  friend class Map;

  private:
    int elevation, pos;
    bool accessLock;

  public:
    Position(char c, int pos);
    char getElevation();
    bool getAccessLock();
};

class Map {
  private:
    int height , width, currentPos, dstPos;
    std::vector<Position> positions;
    bool *accessLocks;

  public:
    Map(std::istream &is);
    ~Map();
    std::vector<bool> getPositionsLocks();
    std::vector<int> shortestRouteStepsCount;
    Map* setPositionsLocks(std::vector<bool> locks);
    int getShortestRouteStepsCount(std::vector<int> &reachedPositions, int &firstLvlZero, int stepCount = 0, int max_recursiveLoopDepth=-1);
    Coord getCoord(int pos, int rowWidth = 0);
    int getPos(Coord coord, int rowWidth = 0);
    bool checkedMove(Coord coord);
    bool moveLeft();
    bool moveRight();
    bool moveBot();
    bool moveTop();
    std::vector<Position>& getPositions();
    Position& operator() (int i);
    friend std::ostream& operator<< (std::ostream &os, Map &map);
}; 
#endif
