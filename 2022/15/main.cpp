#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <cstdlib> //abs
#include <optional>
#include <boost/icl/discrete_interval.hpp>
#include <boost/icl/interval_set.hpp>

using namespace std;

struct Coord{
    int x, y;
};

struct Sensor{
    Coord sensorCoord, nearestBeaconCoord;
    int nearestBeaconDist;
};

vector<Sensor> readSensorsReportFromFile(const string filename){
    ifstream ifs(filename);
    regex regexPattern(R"_(^Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+))_", std::regex::ECMAScript);  
    smatch regexMatch;
    vector<Sensor> sensorsList;

    for (string line; getline(ifs, line);){
        if (regex_search(line, regexMatch, regexPattern)){
            sensorsList.push_back( Sensor {Coord { stoi(regexMatch.str(1)), stoi(regexMatch.str(2))} , Coord {stoi(regexMatch.str(3)), stoi(regexMatch.str(4))}, -1 } );
            sensorsList.back().nearestBeaconDist = abs(sensorsList.back().nearestBeaconCoord.x - sensorsList.back().sensorCoord.x) + abs(sensorsList.back().nearestBeaconCoord.y - sensorsList.back().sensorCoord.y);
        }
    }   
    
    return sensorsList;
}

int getNotPossibleBeaconLocationCount(const vector<Sensor> &sensorsList, 
                                        const int y,
                                        boost::icl::interval_set<int> &ival__row_SensorRangeOverlap_x,
                                        optional<int> xmin = nullopt,
                                        optional<int> xmax = nullopt) {
      
                                        
    int verticalDist, noBeaconLocCount = 0, x_lower, x_upper;
    ival__row_SensorRangeOverlap_x.clear();
    
    for (auto it = sensorsList.begin(); it != sensorsList.end() ; ++it){
        verticalDist = abs(it->sensorCoord.y - y);
        if ( verticalDist <= it->nearestBeaconDist ){
        
            x_lower = it->sensorCoord.x - it->nearestBeaconDist + verticalDist;
            x_upper =  it->sensorCoord.x + it->nearestBeaconDist - verticalDist;
            if ( (xmin.has_value()) && (xmin.value() > x_lower) )
                x_lower = xmin.value();
            if ( (xmax.has_value()) && (xmax.value() < x_upper) )
                x_upper = xmax.value();         
            
            ival__row_SensorRangeOverlap_x.insert(boost::icl::construct<boost::icl::discrete_interval<int> >(
                                    x_lower,
                                    x_upper, 
                                    boost::icl::interval_bounds::closed()));
        }
        
        if (it->nearestBeaconCoord.y == y)
           noBeaconLocCount--;
    }
    
    for (auto it = ival__row_SensorRangeOverlap_x.begin() ; it != ival__row_SensorRangeOverlap_x.end() ; ++it)
        noBeaconLocCount += it->upper() - it->lower() + 1 ;
    
    return noBeaconLocCount;
}

int getNotPossibleBeaconLocationCount(const vector<Sensor> &sensorsList, const int y) {
    boost::icl::interval_set<int> ival__row_SensorRangeOverlap_x;
    return getNotPossibleBeaconLocationCount(sensorsList, y, ival__row_SensorRangeOverlap_x);
}

long long getTunFreq(const vector<Sensor> &sensorsList, int xy_min, int xy_max) {
    boost::icl::interval_set<int> ival__row_SensorRangeOverlap_x, ival_x;
    long long tunFreq = 0LL;
    
    //Look for row (y) with exactly one possible beacon location
    for (int y = xy_min ; y <= xy_max ; ++y){
        if ( getNotPossibleBeaconLocationCount(sensorsList, y, ival__row_SensorRangeOverlap_x, xy_min, xy_max) == xy_max - xy_min ){
            tunFreq = y;
            break;
        }
    }
    
    //Extract x from row (y)
    //-------------------------------------
    ival_x.insert(boost::icl::construct<boost::icl::discrete_interval<int> >(   xy_min,
                                                                                xy_max, 
                                                                                boost::icl::interval_bounds::closed()));
                                                                                
    for (auto ival = ival__row_SensorRangeOverlap_x.begin() ; ival != ival__row_SensorRangeOverlap_x.end() ; ++ival)
        ival_x.subtract(*ival);

    int x = *boost::icl::elements_begin(ival_x);
    //-------------------------------------

    tunFreq += x * 4000000LL;
    return tunFreq;
}

int main (int argc, char **argv) {
    vector<Sensor> sensorsList = readSensorsReportFromFile("input_15.txt");   
    
    cout << "Day 15 - Part1  anwswer : " << getNotPossibleBeaconLocationCount(sensorsList, 2000000) << endl;
    cout << "Day 15 - Part2  anwswer : " << getTunFreq(sensorsList, 0, 4000000) << endl;

    exit(0);
}
