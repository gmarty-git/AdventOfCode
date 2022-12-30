#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <utility>  //pair
#include <regex>
#include <boost/graph/adjacency_list.hpp>
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
    unsigned newEdgeWeight;
    
    while (graphUpdate){
        graphUpdate = false;
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
        }
    }
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

    Graph g, g_simplified;
    readGraphReportFromFile("input_16.txt", g);
     
    g_simplified = g;
    simplifyGraph(g_simplified);
    
    writeGraphToDotFile("tunnelsGraph.dot", g, true);
    writeGraphToDotFile("valvesGraph.dot", g_simplified);
    cout << "Day 16 - Part1  anwswer : " << "" << endl;
    cout << "Day 16 - Part2  anwswer : " << "" << endl;

    exit(0);
}
