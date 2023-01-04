#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <utility>  //pair
#include <algorithm> //next_permutation (not used), all_of
//#include <numeric> //iota
#include <regex>
#include <boost/multi_array.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/graphviz.hpp>

//using std::literals::string_literals::operator""s;
using namespace std;

struct VertexProperties 
{ 
   std::string name;
   unsigned id, flow;
   VertexProperties() : name(""), id(0), flow(0) {}
   VertexProperties(std::string const& n, unsigned i, unsigned f) : name(n), id(i), flow(f) {}
};

struct EdgeProperties 
{ 
   unsigned weight;
   EdgeProperties() : weight(1) {}
   EdgeProperties(unsigned w) : weight(w) {}
};

typedef boost::adjacency_list<
    boost::setS, boost::vecS, boost::undirectedS,
    VertexProperties, EdgeProperties
> Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef std::pair<string, string> Edge;

void readGraphReportFromFile(const string filename, Graph& g){
    ifstream ifs(filename);
    stringstream ss;
   
    regex regexPattern(R"_(^Valve ([A-Z]+) has flow rate=(\d+); tunnels? leads? to valves? (([A-Z]+,? ?)+))_", std::regex::ECMAScript);  
    smatch regexMatch;
    
    unsigned int vertexIdx, vertexTargetIdx, vertexFlow;
    string vertexName, vertexTargetName, vertexTargetsList;
    map<string, unsigned int> verticesNameIdx_map;
    vector<Edge> edgesVec;
    
    vertexIdx = -1;
    for (string line; getline(ifs, line);){
        vertexIdx++;
        if (regex_search(line, regexMatch, regexPattern)){
            vertexName = regexMatch.str(1);
            vertexFlow = stoi(regexMatch.str(2));
            verticesNameIdx_map[vertexName]=vertexIdx;          
            vertex_t v = boost::add_vertex(VertexProperties(vertexName, vertexIdx, vertexFlow), g);
            
            vertexTargetsList = regexMatch.str(3);
            vertexTargetsList.erase (std::remove(vertexTargetsList.begin(), vertexTargetsList.end(), ' '), vertexTargetsList.end());    
            ss.str(vertexTargetsList);
            ss.clear();
            while(getline(ss, vertexTargetName, ','))
                edgesVec.push_back(make_pair(vertexName, vertexTargetName));
       } 
    }

    for (auto edge = edgesVec.begin() ; edge != edgesVec.end() ; ++edge)
         boost::add_edge(verticesNameIdx_map[edge->first], verticesNameIdx_map[edge->second], g);  
}

void simplifyGraph(Graph& g){
    Graph::vertex_iterator v, vend;
    Graph::out_edge_iterator e, eend, e2, e2end; 
    bool graphUpdate = true;
    unsigned newEdgeWeight, vertexIdx;
    
    while (graphUpdate){
        graphUpdate = false;
        vertexIdx = 0;
        for (tie(v, vend) = boost::vertices(g); v != vend; ++v) {
            if ( (g[*v].flow == 0) && (g[*v].name != "AA") ){
                for (tie(e, eend) = boost::out_edges(*v, g) ; e != eend; ++e) {
                    for (tie(e2, e2end) = boost::out_edges(*v, g); e2 != e2end; ++e2) {
                        newEdgeWeight = g[*e].weight + g[*e2].weight; 
                        if ( boost::target(*e, g) != boost::target(*e2, g) )                   
                            boost::add_edge(boost::target(*e, g), boost::target(*e2, g), EdgeProperties(newEdgeWeight), g);
                    }
                }        
        
                boost::clear_vertex(*v, g);
                boost::remove_vertex(*v, g);
                graphUpdate = true;
                break;        
            }
            
            g[*v].id = vertexIdx;
            vertexIdx++;    
        }
    }
}

unsigned int getGraphVerticesCount(Graph& g){
    Graph::vertex_iterator v, vend;
    tie(v, vend) = boost::vertices(g);
    return std::distance(v, vend);
}

typedef boost::multi_array<unsigned int, 2> array_type;
array_type getShortestDistanceMatrix(Graph& g){
    unsigned int verticesCount = getGraphVerticesCount(g);  
    array_type D(boost::extents[verticesCount][verticesCount]);
    boost::johnson_all_pairs_shortest_paths(g, D, boost::weight_map(boost::get(&EdgeProperties::weight, g)));
    
    return D;
}


unsigned long long playVerticesSequence(Graph& g, array_type& shortestDistancesMap,std::vector<unsigned int> verticesIdxSeq, unsigned int timeout){
    unsigned long long  releasedPressure = 0ULL;
    unsigned int totalFlow = 0, sourceVertexIdx, targetVertexIdx, moveDuration;
    for (auto v = verticesIdxSeq.begin() + 1 ; v != verticesIdxSeq.end() ; ++v){
                
        //Move to next valve - some pressure is released during move
        sourceVertexIdx =  *(v-1);
        targetVertexIdx =  *v;
        moveDuration = shortestDistancesMap[sourceVertexIdx][targetVertexIdx];
        releasedPressure += (unsigned long long) totalFlow*moveDuration;
        timeout -= moveDuration;
        
        //If it remains time open valve - some pressure is released during valve opening
        if (timeout > 0){
            timeout--;
            releasedPressure += (unsigned long long)totalFlow;
            totalFlow +=  g[*v].flow;
        }
        else { //If time exceed timeout, corrects relased pressure
            releasedPressure -= (unsigned long long) (-timeout)*totalFlow;
            break;
        }
    }
    
    if (timeout > 0)
        releasedPressure += (unsigned long long) timeout*totalFlow;
        
    return releasedPressure;
}

/*unsigned int playVerticesSequence(Graph& g, array_type& shortestDistancesMap,std::vector<vertex_t> verticesSeq, unsigned int timeout){
    unsigned int releasedPressure = 0, totalFlow = 0, sourceVertexIdx, targetVertexIdx, moveDuration;
    for (auto v = verticesSeq.begin() + 1 ; v != verticesSeq.end() ; ++v){
                
        //Move to next valve - some pressure is released during move
        sourceVertexIdx =  g[*(v-1)].id;
        targetVertexIdx =  g[*v].id;
        moveDuration = shortestDistancesMap[sourceVertexIdx][targetVertexIdx];
        releasedPressure += totalFlow*moveDuration;
        timeout -= moveDuration;
        
        //If it remains time open valve - some pressure is released during valve opening
        if (timeout > 0){
            timeout--;
            releasedPressure += totalFlow;
            totalFlow +=  g[*v].flow;
        }
        else { //If time exceed timeout, corrects relased pressure
            releasedPressure += timeout*totalFlow;
            break;
        }
    }
    
    if (timeout > 0)
        releasedPressure += timeout*totalFlow;
        
    return releasedPressure;
}*/

void getPermutations(   Graph& g, 
                        array_type& shortestDistancesMap, 
                        unsigned int timeout, 
                        std::vector<std::vector<unsigned int> >& allPermutations, 
                        std::vector<unsigned int> currentPermutation = {},
                        std::vector<bool> usedIdx = {},
                        unsigned int usedTime = 0,
                        int nextVertexIdx = -1){

    unsigned int verticesCount = getGraphVerticesCount(g);

    //Initialise permutation to 'AA'
    if (currentPermutation.empty()){
        unsigned int AA_idx;
        Graph::vertex_iterator v, vend;
        for ( std::tie(v, vend) = boost::vertices(g); v != vend; ++v) {
            if (g[*v].name == "AA"){
                AA_idx = g[*v].id;
                break;
            }
        }

        currentPermutation.reserve(verticesCount);
        currentPermutation.push_back(AA_idx);
        usedIdx.resize(verticesCount, false);
        usedIdx[AA_idx] = true;
    }

    //If not root call, then add specify vertex to permutation and update usedTime
    if (nextVertexIdx != -1){
        usedIdx[nextVertexIdx] = true;
        currentPermutation.push_back(nextVertexIdx);
        usedTime += shortestDistancesMap[currentPermutation[currentPermutation.size()-2]][nextVertexIdx] + 1 ; //+1 time to open valve
    }

    //If timeout is reached or all valves have been open then save current permutation and break recursion
    if ( (usedTime >= timeout) || (std::all_of(usedIdx.begin(), usedIdx.end(), [](bool v) { return v; })) ) {
        allPermutations.push_back(currentPermutation);
        return;
    }
    else{
        nextVertexIdx = 0;
        for (auto usedV = usedIdx.begin() ; usedV != usedIdx.end() ; ++usedV){
            if (!*usedV)
                getPermutations(g, shortestDistancesMap, timeout, allPermutations, currentPermutation, usedIdx, usedTime, nextVertexIdx);
            
            nextVertexIdx++;
        }   
    }    
}

unsigned long long getMaxReleasedPressure(Graph& g, array_type& shortestDistancesMap, unsigned int timeout){
    std::vector<std::vector<unsigned int> > allPermutations;
    getPermutations(g, shortestDistancesMap, timeout, allPermutations);

    //Naive approach : simulate all paths, even obviously ineficient ones
    unsigned long long max_releasedP = 0ULL, releasedP = 0ULL;
    for (auto perm = allPermutations.begin() ; perm != allPermutations.end() ; ++perm){
        releasedP = playVerticesSequence(g, shortestDistancesMap, *perm, timeout);
        if (releasedP > max_releasedP)
            max_releasedP = releasedP;
    }
    
    return max_releasedP;
}

template <class Name, class Flow>
class vertex_writer {
    public:
        vertex_writer(Name _name, Flow _flow) : name(_name), flow(_flow) {}
        template <class VertexOrEdge>
        void operator()(std::ostream& out, const VertexOrEdge& v) const {
        if (flow[v] != 0){
            out << "[label=\"" << name[v] << "\\nflow=" << flow[v] << "\"]";
            out << "[style=filled][fillcolor=\"yellow\"]";
        }
        else
            out << "[label=\"" << name[v] << "\"]";
          
    if ( name[v] == "AA" )
        out << "[style=filled][fillcolor=\"aqua\"]";
}
    private:
        Name name;
        Flow flow;
};

template <class Weight>
class edge_writer {
    public:
        edge_writer(Weight _weight, bool _hideEw) : weight(_weight) {hideEdgeWeight = _hideEw;}
        template <class VertexOrEdge>
        void operator()(std::ostream& out, const VertexOrEdge& v) const {
            if (!hideEdgeWeight)
                out << "[label=\"" << weight[v] << "\"]";
            
            out << "[len=" << weight[v] << "]";
        }
    
    private:
    bool hideEdgeWeight;
    Weight weight;
};

struct graph_writer {
    void operator()(std::ostream& out) const {
        out << "node [shape=square][style=rounded]" << std::endl;
    }
};

void writeGraphToDotFile(std::string filename, Graph& g, bool hideEdgeWeight = false){
    std::ofstream ofs(filename, std::ios::out);
    write_graphviz(ofs, g,  vertex_writer(boost::get(&VertexProperties::name, g), 
                                            boost::get(&VertexProperties::flow, g)),
                            edge_writer(boost::get(&EdgeProperties::weight, g), hideEdgeWeight), 
                            graph_writer());
}



int main (int argc, char **argv) { 

    Graph g;
    readGraphReportFromFile("input_16.txt", g);
    writeGraphToDotFile("tunnelsGraph.dot", g, true);
    
    simplifyGraph(g);
    writeGraphToDotFile("valvesGraph.dot", g);
    array_type shortestDistancesMap = getShortestDistanceMatrix(g);
 
    
    cout << "Day 16 - Part1  anwswer : " << getMaxReleasedPressure(g, shortestDistancesMap, 30)  << endl;
    cout << "Day 16 - Part2  anwswer : " << "" << endl;

    exit(0);
}
