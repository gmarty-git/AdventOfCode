#include "class_Map.hpp"

Position::Position(char c, int pos) {
    if ( c == 'S' )
        elevation = 'a';
    else if ( c == 'E' )
        elevation = 'z';
    else
        elevation = c;    

    this->pos = pos;
    accessLock = false;
}

char Position::getElevation() {
    return elevation;
}

bool Position::getAccessLock() {
   return accessLock;
}

Map::Map(std::istream &is) {    
    width = 0;

    char c;
    while (is.get(c)) {
        
        if ( c == '\n' ){
            if (width == 0)
                width = positions.size();
        }
        else {
            positions.push_back(Position(c, positions.size()));
            if ( c == 'S' )
                dstPos = positions.size() - 1;
                //currentPos = positions.size() - 1;
            else if ( c == 'E' )
                currentPos = positions.size() - 1;
                //dstPos = positions.size() - 1;
        }
    }

    height = positions.size() / width;
    shortestRouteStepsCount.resize(width*height*width*height, -1);
    //accessLocks = new bool[width*height];
    //std::fill_n(accessLocks, width*height, false);
}

Map::~Map() {
    //delete accessLocks;
}

/*std::vector<bool> Map::getPositionsLocks() {
    std::vector<bool>  locks(positions.size());
    for (int i = 0 ; i < positions.size() ; ++i)
        locks[i] = positions[i].accessLock;

    return locks;
}

Map* Map::setPositionsLocks(std::vector<bool> locks) {
    for (int i = 0 ; i < locks.size() ; ++i)
        positions[i].accessLock = locks[i];

    return this;
}*/

//VERSION 1 : donne le bon résultats mais est trop lente
/*
int Map::getShortestRouteStepsCount(int max_recursiveLoopDepth){
    //if (stepsCountPerDirection != nullptr)
    //    std::cout << stepsCountPerDirection[0] << " - " << stepsCountPerDirection[1] << " - " << stepsCountPerDirection[2] << " - " << stepsCountPerDirection[3] << std::endl;
    //std::cout << std::endl << "===========" << std::endl << *this << std::endl;
    
    if (max_recursiveLoopDepth == 0){
       std::cout << "[ERROR] - Map::getShortestRouteStepsCount : Max recurcive depth reached ! Abort execution ..." << std::endl;
       exit(-1);
    }
    else if (max_recursiveLoopDepth > 0)
        max_recursiveLoopDepth--;

    Coord coord_routes = {currentPos, dstPos};
    int pos_routes = getPos(coord_routes, width*height);
    if ( (shortestRouteStepsCount[pos_routes] != -1) &&  (shortestRouteStepsCount[pos_routes] <= height*width) ){
        return shortestRouteStepsCount[pos_routes];
    }

    if (currentPos == dstPos)
        return 0;
    
;
    int currentPosSave = currentPos;
    
    int stepsCountPerDirection[4]; 
    std::fill_n(stepsCountPerDirection, 4, height*width+1);

    positions[currentPos].accessLock = true;
    if (moveLeft()){
           stepsCountPerDirection[0] = 1+getShortestRouteStepsCount(max_recursiveLoopDepth);
           currentPos = currentPosSave;
    }    

    if (moveRight()){
          stepsCountPerDirection[1] = 1+getShortestRouteStepsCount(max_recursiveLoopDepth);
          currentPos = currentPosSave;
    }    

    if (moveTop()){
          stepsCountPerDirection[2] = 1+getShortestRouteStepsCount(max_recursiveLoopDepth);
          currentPos = currentPosSave;
    }    

    if (moveBot()){
          stepsCountPerDirection[3] = 1+getShortestRouteStepsCount(max_recursiveLoopDepth);
          currentPos = currentPosSave;
    }    

    int shortestRoute = *( std::min_element(stepsCountPerDirection, stepsCountPerDirection+4) );
    shortestRouteStepsCount[pos_routes] = shortestRoute;
    positions[currentPos].accessLock = false;
    
    return shortestRoute;
}*/

int Map::getShortestRouteStepsCount(std::vector<int> &reachedPositions, int &firstLvlZero, int stepCount, int max_recursiveLoopDepth) {
    //if (stepsCountPerDirection != nullptr)
    //    std::cout << stepsCountPerDirection[0] << " - " << stepsCountPerDirection[1] << " - " << stepsCountPerDirection[2] << " - " << stepsCountPerDirection[3] << std::endl;
    //std::cout << std::endl << "===========" << std::endl << *this << std::endl;
    
    if (max_recursiveLoopDepth == 0){
       std::cout << "[ERROR] - Map::getShortestRouteStepsCount : Max recurcive depth reached ! Abort execution ..." << std::endl;
       exit(-1);
    }
    else if (max_recursiveLoopDepth > 0)
        max_recursiveLoopDepth--;

    if (currentPos == dstPos)
        return 0;

    if (stepCount == 0)
        reachedPositions.push_back(currentPos);

    int currentPosSave = currentPos;
    int pos_routes;
    positions[currentPos].accessLock = true;
    std::vector<int> reachedPositions_new;

    for (auto it = reachedPositions.begin() ; it != reachedPositions.end() ; ++it ) {
        
        currentPos =  *it;

        if (currentPos != -1) {

            if (moveLeft()){
                   reachedPositions_new.push_back(currentPos);
                   pos_routes = getPos(Coord {currentPosSave, currentPos}, width*height);
                   shortestRouteStepsCount[pos_routes] = stepCount + 1;
                   if ( (firstLvlZero == -1) && (positions[currentPos].getElevation() == 'a') ) //Part2
                        firstLvlZero = stepCount + 1;

                   currentPos = *it;
            }    

            if (moveRight()){
                  reachedPositions_new.push_back(currentPos);
                  pos_routes = getPos(Coord {currentPosSave, currentPos}, width*height);
                  shortestRouteStepsCount[pos_routes] = stepCount + 1;
                  if ( (firstLvlZero == -1) && (positions[currentPos].getElevation() == 'a') ) //Part2
                        firstLvlZero = stepCount + 1;

                  currentPos = *it;
            }    

            if (moveTop()){
                  reachedPositions_new.push_back(currentPos);
                  pos_routes = getPos(Coord {currentPosSave, currentPos}, width*height);
                  shortestRouteStepsCount[pos_routes] = stepCount + 1;
                  if ( (firstLvlZero == -1) && (positions[currentPos].getElevation() == 'a') )
                        firstLvlZero = stepCount + 1;
    
                  currentPos = *it;
            }    

            if (moveBot()){
                reachedPositions_new.push_back(currentPos);
                pos_routes = getPos(Coord {currentPosSave, currentPos}, width*height);
                shortestRouteStepsCount[pos_routes] = stepCount + 1;
                if ( (firstLvlZero == -1) && (positions[currentPos].getElevation() == 'a') )
                        firstLvlZero = stepCount + 1;

                currentPos = *it;
            }
        }
    }

    currentPos = currentPosSave;
    pos_routes = getPos(Coord {currentPos, dstPos}, width*height);
  
    if ( !reachedPositions_new.empty())
        getShortestRouteStepsCount(reachedPositions_new, firstLvlZero, ++stepCount, max_recursiveLoopDepth);

    return shortestRouteStepsCount[pos_routes];

}

Coord Map::getCoord(int pos, int rowWidth){ 
    if (rowWidth == 0)
        rowWidth = width;
   
    Coord coord;
    coord.x = pos%rowWidth;
    coord.y = (pos - coord.x) / rowWidth;
    return coord;
}

int Map::getPos(Coord coord, int rowWidth){
    if (rowWidth == 0)
        rowWidth = width;

    return coord.y * rowWidth + coord.x; // 0 in top-right corner, x horizontal, y vertical
}

Position& Map::operator() (int i) {
    return positions[i];
}

bool Map::checkedMove(Coord coord){
    int destPos = getPos(coord);
    //if ( !(positions[destPos].accessLock) && (positions[destPos].elevation - positions[currentPos].elevation < 2) ) {   
    if ( !(positions[destPos].accessLock) && (positions[currentPos].elevation - positions[destPos].elevation < 2) ) {         
          //std::cout<<std::endl<<"dstLock="<<positions[destPos].accessLock<<" - dst="<< destPos <<"-"<<positions[destPos].elevation<<" - src="<<positions[currentPos].elevation<<" --> TRUE"<<std::endl;
            currentPos = destPos;
            positions[destPos].accessLock = true;        
            return true;
        }
        else{
           // std::cout<<std::endl<<"dstLock="<<positions[destPos].accessLock<<" - dst="<< destPos <<"-"<<positions[destPos].elevation<<" - src="<<positions[currentPos].elevation<<" --> FALSE"<<std::endl;
            return false;
        }
}

bool Map::moveLeft() {
    Coord coord = getCoord(currentPos);

    if (coord.x == 0)
        return false;
    else {
        coord.x--;
        return checkedMove(coord);
    }      
}

bool Map::moveRight() {
    Coord coord = getCoord(currentPos);

    if (coord.x == width - 1)
        return false;
    else {
        coord.x++;
        return checkedMove(coord);
    }      
}

bool Map::moveTop() {
    Coord coord = getCoord(currentPos);

    if (coord.y == 0)
        return false;
    else {
        coord.y--;
        return checkedMove(coord);
    }      
}

bool Map::moveBot() {
    Coord coord = getCoord(currentPos);

    if (coord.y == height - 1)
        return false;
    else { 
        coord.y++;
        return checkedMove(coord);
    }      
}

std::ostream& operator<< (std::ostream &os, Map &map) {
    for (int i = 0 ; i < map.positions.size() ; ++i) {
        
        if (map.positions[i].getAccessLock())
            os << '.';
        else if ( i == map.dstPos )
            os << 'E';
        else if ( i == map.currentPos )
            os << 'S';
        else
            os << map.positions[i].getElevation();

        if (i%map.width == map.width - 1)
            os << std::endl;
    }

    return os;
}
