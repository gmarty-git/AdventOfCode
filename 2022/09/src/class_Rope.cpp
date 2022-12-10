#include "class_Rope.hpp"

Knot::Knot () {coord.x = 0 ; coord.y = 0;}

Knot* Knot::move(int x_dir, int y_dir) {
    coord.x += x_dir;
    coord.y += y_dir;
    return this;
}

Knot* Knot::move(char direction, int mvCount) {
    if (direction == 'U')
        return this->move(0, mvCount);

    else if (direction == 'D')
        return this->move(0, -mvCount);

    else if (direction == 'L')
        return this->move(-mvCount, 0);

    else if (direction == 'R') {
        return this->move(mvCount, 0);
    }
    else {
        std::cout << "[ERROR] - Rope::moveRove : Unexpected direction '" << direction << "'. Aborting execution ..." << std::endl;
        std::exit(EXIT_FAILURE);
    } 
}

Coord Knot::getCoord() {
    return Coord {coord.x, coord.y};
}

Coord Knot::getDistanceTo(const Knot &k2) {
    return Coord {k2.coord.x - coord.x , k2.coord.y - coord.y};
}

Rope* Rope::moveHead(int x_dir, int y_dir) {
    k_head.move(x_dir, y_dir);

    Coord dist_tail_head = k_tail.getDistanceTo(k_head);
    if ( (abs(dist_tail_head.x) > 1) || (abs(dist_tail_head.y) > 1) ){
        if (abs(dist_tail_head.x) > abs(dist_tail_head.y))
            k_tail.move(dist_tail_head.x - sign(dist_tail_head.x) , dist_tail_head.y);
        else if (abs(dist_tail_head.x) < abs(dist_tail_head.y))
            k_tail.move(dist_tail_head.x, dist_tail_head.y - sign(dist_tail_head.y));
        else
            k_tail.move(dist_tail_head.x - sign(dist_tail_head.x) , dist_tail_head.y - sign(dist_tail_head.y));
    }

    return this;
}

Rope* Rope::moveHead(char direction, int mvCount) {
    if (direction == 'U')
        return this->moveHead(0, mvCount);

    else if (direction == 'D')
        return this->moveHead(0, -mvCount);

    else if (direction == 'L')
        return this->moveHead(-mvCount, 0);

    else if (direction == 'R') {
        return this->moveHead(mvCount, 0);
    }
    else {
        std::cout << "[ERROR] - Rope::moveRove : Unexpected direction '" << direction << "'. Aborting execution ..." << std::endl;
        std::exit(EXIT_FAILURE);
    } 
}

Knot& Rope::getHead()  { return k_head ;}
Knot& Rope::getTail()  { return k_tail ;}

ChainedRope::ChainedRope(unsigned int knotsCount){
    if (knotsCount<2){
        std::cout << "[ERROR] - ChainedRope::ChainedRope: Expected more than one link (use Rope class instead). Aborting execution ..." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    links.resize(knotsCount-1);
}

ChainedRope* ChainedRope::moveHead(char direction, int mvCount){
    links.front().moveHead(direction, mvCount);

    Coord dist_tail_head;
    for (std::vector<Rope>::iterator it = links.begin()+1 ; it != links.end() ; ++it){
        dist_tail_head = (*it).getHead().getDistanceTo( (*(it-1)).getTail() );

        if ( (dist_tail_head.x == 0) && (dist_tail_head.y == 0) )
            return this;

        else
            (*it).moveHead(dist_tail_head.x, dist_tail_head.y);

    }

    return this;
}

Knot& ChainedRope::getTail() {return links.back().getTail();}
Knot& ChainedRope::getHead() {return links.front().getTail();}

std::ostream& operator<< (std::ostream &os, Rope &rope){
    os << "Head(" << rope.getHead().getCoord().x << "," << rope.getHead().getCoord().y << ") - Tail(" << rope.getTail().getCoord().x << "," << rope.getTail().getCoord().y << ")";
    return os;
}

int sign(int x) {return (x > 0) ? 1 : ((x < 0) ? -1 : 0); }

int64_t packCoord__int64_t(Coord coord){
    int64_t coord_64b;
    
    memcpy(&coord_64b, &coord.x, 4);
    memcpy(reinterpret_cast<char*>(&coord_64b)+4, &coord.y, 4);
    
    return coord_64b;
}
