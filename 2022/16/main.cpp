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


unsigned int playVerticesSequence(Graph& g, array_type& shortestDistancesMap,std::vector<unsigned int> verticesIdxSeq, unsigned int timeout){
    if (verticesIdxSeq.size() < 2)
        return 0;

    unsigned int releasedPressure = 0, totalFlow = 0, sourceVertexIdx, targetVertexIdx, moveDuration;
    int time = timeout;

    for (auto v = verticesIdxSeq.begin() + 1 ; v != verticesIdxSeq.end() ; ++v){
                
        //Move to next valve - some pressure is released during move
        sourceVertexIdx =  *(v-1);
        targetVertexIdx =  *v;
        moveDuration = shortestDistancesMap[sourceVertexIdx][targetVertexIdx];
        releasedPressure += (unsigned long long) totalFlow*moveDuration;
        time -= moveDuration;
        
        //If it remains time open valve - some pressure is released during valve opening
        if (time > 0){
            time--;
            releasedPressure += (unsigned long long)totalFlow;
            totalFlow +=  g[*v].flow;
        }
        else { //If time exceed timeout, corrects relased pressure
            releasedPressure -= (unsigned long long) (-time)*totalFlow;
            break;
        }
    }
    
    if (time > 0)
        releasedPressure += (unsigned long long) time*totalFlow;
        
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
                        std::vector< std::pair< std::vector<unsigned int>, std::vector<unsigned int> > >& allPermutations,
                        bool withBrolephant = false,
                        std::pair< std::vector<unsigned int>, std::vector<unsigned int> > currentPermutation = {},
                        std::vector<bool> usedIdx = {},
                        std::pair<unsigned int, unsigned int> usedTime = {0, 0},
                        std::pair<int, int> nextVertexIdx = {-1, -1}){

    unsigned int verticesCount = getGraphVerticesCount(g);

    //Initialise permutation to 'AA'
    if (currentPermutation.first.empty()){
        unsigned int AA_idx;
        Graph::vertex_iterator v, vend;
        for ( std::tie(v, vend) = boost::vertices(g); v != vend; ++v) {
            if (g[*v].name == "AA"){
                AA_idx = g[*v].id;
                break;
            }
        }

        currentPermutation.first.reserve(verticesCount);
        currentPermutation.first.push_back(AA_idx);
        usedIdx.resize(verticesCount, false);
        usedIdx[AA_idx] = true;

        if (withBrolephant){
            currentPermutation.second.reserve(verticesCount);
            currentPermutation.second.push_back(AA_idx);
        }
        else
            usedTime.second = timeout + 1;

    }

    //Add specified vertex to permutation and update usedTime
    //-1 in case of root call or if valve has been allocated to other charater
    if (nextVertexIdx.first != -1){
        usedIdx[nextVertexIdx.first] = true;
        currentPermutation.first.push_back(nextVertexIdx.first);
        usedTime.first += shortestDistancesMap[currentPermutation.first[currentPermutation.first.size()-2]][nextVertexIdx.first] + 1 ; //+1 time to open valve
    }
    if (nextVertexIdx.second != -1){
        usedIdx[nextVertexIdx.second] = true;
        currentPermutation.second.push_back(nextVertexIdx.second);
        usedTime.second += shortestDistancesMap[currentPermutation.second[currentPermutation.second.size()-2]][nextVertexIdx.second] + 1 ; //+1 time to open valve
    }



    //If timeout is reached or all valves have been open then save current permutation and break recursion
    if ( ((usedTime.first >= timeout) && (usedTime.second >= timeout)) || (std::all_of(usedIdx.begin(), usedIdx.end(), [](bool v) { return v; })) ) {
        allPermutations.push_back(currentPermutation);
        return;
    }
    else{
        unsigned int nextVertexIdx_element = 0;
        for (auto usedV = usedIdx.begin() ; usedV != usedIdx.end() ; ++usedV){
            if (!*usedV){
                if (usedTime.first < timeout)
                    getPermutations(g, shortestDistancesMap, timeout, allPermutations, withBrolephant, currentPermutation, usedIdx, usedTime, {nextVertexIdx_element, -1});
                if (usedTime.second < timeout)
                    getPermutations(g, shortestDistancesMap, timeout, allPermutations, withBrolephant, currentPermutation, usedIdx, usedTime, {-1, nextVertexIdx_element});
            }
            
            nextVertexIdx_element++;
        }   
    }    
}

unsigned int getMaxReleasedPressure(Graph& g, array_type& shortestDistancesMap, 
                                    unsigned int timeout, 
                                    std::pair< std::vector<unsigned int>, std::vector<unsigned int> >& maxPreleased_perm,
                                    bool withBrolephant = false){

    std::vector< std::pair< std::vector<unsigned int>, std::vector<unsigned int> > > allPermutations;

    //Use Part1 result to reduce Part2 problem size
    if ( (!maxPreleased_perm.first.empty()) && (maxPreleased_perm.second.empty()) ){
        maxPreleased_perm.first.resize(5);
        unsigned int verticesCount = getGraphVerticesCount(g);
        std::vector<bool> usedIdx (verticesCount, false);
        std::pair<unsigned int, unsigned int> usedTime (0, 0);
        usedIdx[maxPreleased_perm.first[0]] = true;
        for (auto i = 1 ; i < maxPreleased_perm.first.size() ; ++i){
            usedIdx[maxPreleased_perm.first[i]] = true;
            usedTime.first += shortestDistancesMap[maxPreleased_perm.first[i-1]][maxPreleased_perm.first[i]] + 1;
        }
        maxPreleased_perm.second.push_back(maxPreleased_perm.first[0]);

        getPermutations(g, shortestDistancesMap, timeout, allPermutations, withBrolephant, maxPreleased_perm, usedIdx, usedTime);
    }
    else
        getPermutations(g, shortestDistancesMap, timeout, allPermutations, withBrolephant);

    //Naive approach (don't kow if there are better solutions): simulate all paths which take less than timeout
    unsigned int max_releasedP = 0, releasedP = 0;
    for (auto perm = allPermutations.begin() ; perm != allPermutations.end() ; ++perm){
        releasedP = playVerticesSequence(g, shortestDistancesMap, (*perm).first, timeout) + playVerticesSequence(g, shortestDistancesMap, (*perm).second, timeout);
        if (releasedP > max_releasedP){
            max_releasedP = releasedP;
            maxPreleased_perm = *perm;
        }
    }

    return max_releasedP;
}

/*void getPermutations(   Graph& g, 
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

    //If not root call, then add specified vertex to permutation and update usedTime
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

unsigned int getMaxReleasedPressure(Graph& g, array_type& shortestDistancesMap, unsigned int timeout){
    std::vector<std::vector<unsigned int> > allPermutations;
    std::vector<unsigned int> maxPreleasedSequence;
    getPermutations(g, shortestDistancesMap, timeout, allPermutations);

    //Naive approach (don't kow if there are better solutions): simulate all paths which take less than timeout
    unsigned int max_releasedP = 0, releasedP = 0;
    for (auto perm = allPermutations.begin() ; perm != allPermutations.end() ; ++perm){
        releasedP = playVerticesSequence(g, shortestDistancesMap, *perm, timeout);
        if (releasedP > max_releasedP){
            max_releasedP = releasedP;
            maxPreleasedSequence = *perm;
        }
    }

    return max_releasedP;
}*/

template <class Name, class Flow, class Id>
class vertex_writer {
    public:
        vertex_writer(Name _name, Flow _flow, Id _id, bool _hideVId) : name(_name), flow(_flow), id(_id) {hideVerticesId = _hideVId;}
        template <class VertexOrEdge>
        void operator()(std::ostream& out, const VertexOrEdge& v) const {
            std::string str_VertexLabel = "[label=\"", str_VertexFillColor = "";
            if (!hideVerticesId)
                str_VertexLabel += std::to_string(id[v]) + ".";

            str_VertexLabel += name[v];

            if (flow[v] != 0){
                str_VertexLabel += "\\nflow=" + std::to_string(flow[v]);
                str_VertexFillColor = "[style=filled][fillcolor=\"yellow\"]";

            }
            str_VertexLabel += "\"]";

            if ( name[v] == "AA" )
                str_VertexFillColor = "[style=filled][fillcolor=\"aqua\"]";

            out << str_VertexLabel << str_VertexFillColor;
        }
            private:
                Id id;
                Name name;
                Flow flow;
                bool hideVerticesId;
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

void writeGraphToDotFile(std::string filename, Graph& g, bool hideVerticesId = false, bool hideEdgeWeight = false){
    std::ofstream ofs(filename, std::ios::out);
    write_graphviz(ofs, g,  vertex_writer(boost::get(&VertexProperties::name, g), 
                                            boost::get(&VertexProperties::flow, g),
                                            boost::get(&VertexProperties::id, g),
                                            hideVerticesId),
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

    std::pair< std::vector<unsigned int>, std::vector<unsigned int> > maxPreleased_perm;

    cout << "Day 16 - Part1  anwswer : " << getMaxReleasedPressure(g, shortestDistancesMap, 30, maxPreleased_perm)  << endl;
    cout << "Day 16 - Part2  anwswer : " << getMaxReleasedPressure(g, shortestDistancesMap, 26, maxPreleased_perm, true) << endl;

    exit(0);
}
