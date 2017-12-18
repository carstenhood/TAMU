////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Graph.h: implement graph in adjacency list representation
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <limits>
#include <string>
#include <map>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

template <typename Object, typename Weight>
class graph {
public:
    
    class Vertex;
    
    class Edge {
    public:
        Edge(Vertex* v, Vertex* w, Weight setweight) {
            start = v;
            end = w;
            v->edge.push_back(this);
            w->inedge.push_back(this);
            weight = setweight;
            explored = false;
        }
        Edge() {
            explored = false;
        }
        Weight weight;
        Vertex* start;
        Vertex* end;
        bool explored;
    };
    
    class Vertex {
    public:
        Vertex(Object setelement) {
            level = 0;
            connectedcomponent = 0;
            element = setelement;
            back = NULL;
            explored = false;
        }
        Vertex() {
            level = 0;
            connectedcomponent = 0;
            back = NULL;
            explored = false;
        }
        Object element;
        vector<Edge*> edge;
        vector<Edge*> inedge;
        
        double value;
        size_t starttime, finishtime;
        size_t level;
        size_t connectedcomponent;
        float rank;
        Vertex* back;
        int color;
        bool explored;
    };
    
    /////////////////////////////////////////////////////////////////////////
private:
    
    vector<Edge*> edge;
    
    vector<Vertex*> vertex;
    
    size_t counter;
    
public:
    /////////////////////////////////////////////////////////////////////////
    graph();
    
    graph(graph& G);
    
    ~graph();
    
    void reset();
    void resetBack();
    void resetValues();
    void resetLevels();
    void resetExplored();
    void resetConnectedComponents();
    
    vector<Vertex*> incidentVertices(Vertex* v);
    vector<Edge*> incidentEdges(Vertex* v);
    vector<Edge*> outgoingEdges(Vertex* v);
    vector<Vertex*> adjacentVertices(Vertex* v);
    size_t indegree(Vertex* v);
    size_t outdegree(Vertex* v);
    size_t degree(Vertex* v);
    Vertex* startVertex(Edge* e);
    Vertex* endVertex(Edge* e);
    // is there an edge from v to w ?
    bool isAdjacent(Vertex* v, Vertex* w);
    
    Vertex* insertVertex(Object o);
    void insertEdge(Vertex* v, Vertex* w, Weight t);
    void insertUndirectedEdge(Vertex* v, Vertex* w, Weight t);
    void removeVertex(Vertex* v);
    void removeEdge(Edge* e);
    
    size_t numVertices();
    size_t numEdges();
    vector<Vertex*> vertices();
    vector<Edge*> edges();
    
    void print();
    void read_file(std::string filename);
    /////////////////////////////////////////////////////////////////////////

};


template <typename Object, typename Weight>
graph<Object, Weight>::graph() {
    ///////////////////////////////////////////
    // Implement default constructor here
    ///////////////////////////////////////////
    counter = 0;
}

template <typename Object, typename Weight>
graph<Object, Weight>::graph(graph<Object, Weight>& G) {
    ///////////////////////////////////////////
    // Implement copy constructor here
    ///////////////////////////////////////////
    
    //delete old elements
    for (typename vector<typename graph<Object, Weight>::Edge*>::iterator it = this->edge.begin(); it != this->edge.end(); ++it)
        delete *it;
    for (typename vector<typename graph<Object, Weight>::Vertex*>::iterator it = this->vertex.begin(); it != this->vertex.end(); ++it)
        delete *it;
    
    //map from old to new pointers
    map<Vertex*, Vertex*> pointers;
    
    //copy vertices
    for (size_t i=0; i<G.vertex.size(); ++i)
        pointers[G.vertex[i]] = insertVertex(G.vertex[i]->element);

    //copy edges
    for (size_t i=0; i<G.edge.size(); ++i)
        insertEdge(pointers[G.edge[i]->start], pointers[G.edge[i]->end], G.edge[i]->weight);
}

template <typename Object, typename Weight>
graph<Object, Weight>::~graph() {
    ///////////////////////////////////////////
    // Implement destructor here
    ///////////////////////////////////////////
   
    //delete edges
    for (typename vector<typename graph<Object, Weight>::Edge*>::iterator it = this->edge.begin(); it != this->edge.end(); ++it)
        delete *it;
    
    //delete vertices
    for (typename vector<typename graph<Object, Weight>::Vertex*>::iterator it = this->vertex.begin(); it != this->vertex.end(); ++it)
        delete *it;
}

template <typename Object, typename Weight>
void graph<Object, Weight>::reset() {
    counter = 0;
    resetBack();
    resetValues();
    resetLevels();
    resetExplored();
    resetConnectedComponents();
}

template <typename Object, typename Weight>
void graph<Object, Weight>::resetBack() {
    for(size_t i=0; i<vertex.size(); i++)
        vertex[i]->back = NULL;
}

template <typename Object, typename Weight>
void graph<Object, Weight>::resetValues() {
    for(size_t i=0; i<vertex.size(); i++)
        vertex[i]->value = numeric_limits<int>::max();
}

template <typename Object, typename Weight>
void graph<Object, Weight>::resetLevels() {
    for(size_t i=0; i<vertex.size(); i++)
        vertex[i]->level = 0;
}

template <typename Object, typename Weight>
void graph<Object, Weight>::resetExplored() {
    for(size_t i=0; i<vertex.size(); i++)
        vertex[i]->explored = false;
}

template <typename Object, typename Weight>
void graph<Object, Weight>::resetConnectedComponents() {
    for(size_t i=0; i<vertex.size(); i++)
        vertex[i]->connectedcomponent = 0;
}

template <typename Object, typename Weight>
vector<typename graph<Object, Weight>::Edge*> graph<Object, Weight>::incidentEdges(Vertex* v) {
    vector<Edge*> result;
    ///////////////////////////////////////////////
    // Implement collecting incident edges here
    ///////////////////////////////////////////////
    result = v->inedge;
    return result;
}

template <typename Object, typename Weight>
vector<typename graph<Object, Weight>::Edge*> graph<Object, Weight>::outgoingEdges(Vertex* v) {
    vector<Edge*> result;
    ///////////////////////////////////////////////
    // Implement collecting outgoing edges here
    ///////////////////////////////////////////////
    result = v->edge;
    return result;
}

template <typename Object, typename Weight>
vector<typename graph<Object, Weight>::Vertex*> graph<Object, Weight>::incidentVertices(Vertex* v) {
    vector<Vertex*> result;
    ///////////////////////////////////////////////
    // Implement filling result vector here
    ///////////////////////////////////////////////
    
    //collect start vertices
    for (int i=0; i < v->inedge.size(); ++i)
        result.push_back(v->inedge[i]->start);
        
    return result;
}

template <typename Object, typename Weight>
vector<typename graph<Object, Weight>::Vertex*> graph<Object, Weight>::adjacentVertices(Vertex* v) {
    vector<Vertex*> result;
    ///////////////////////////////////////////////
    // Implement filling result vector here
    ///////////////////////////////////////////////
    
    //collect end vertices
    for (int i=0; i < v->edge.size(); ++i)
        result.push_back(v->edge[i]->end);
    
    return result;
}

template <typename Object, typename Weight>
size_t graph<Object, Weight>::outdegree(Vertex* v) {
    ///////////////////////////////////////////////
    // Implement calculating outdegree here
    ///////////////////////////////////////////////
    return v->edge.size();
}

template <typename Object, typename Weight>
size_t graph<Object, Weight>::indegree(Vertex* v) {
    ///////////////////////////////////////////////
    // Implement calculating indegree here
    ///////////////////////////////////////////////
    return v->inedge.size();
}

template <typename Object, typename Weight>
size_t graph<Object, Weight>::degree(Vertex* v) {
    ///////////////////////////////////////////////
    // Implement calculating degree here
    ///////////////////////////////////////////////
    return indegree(v) + outdegree(v);
}

template <typename Object, typename Weight>
typename graph<Object, Weight>::Vertex* graph<Object, Weight>::startVertex(Edge* e) {
    return e->start;
}

template <typename Object, typename Weight>
typename graph<Object, Weight>::Vertex* graph<Object, Weight>::endVertex(Edge* e) {
    return e->end;
}

// is there an edge from v to w ?
template <typename Object, typename Weight>
bool graph<Object, Weight>::isAdjacent(Vertex* v, Vertex* w) {
    ///////////////////////////////////////////////
    // Implement the adjacency checking here
    ///////////////////////////////////////////////
    
    //use vertex with less edges to check
    if (degree(v) < degree(w)) {
        //check all opposite vertices
        for (int i=0; i < v->edge.size(); ++i)
            if (v->edge[i]->end == w)
                return true;
    } else {
        //check all opposite vertices
        for (int i=0; i < w->edge.size(); ++i)
            if (w->edge[i]->end == v)
                return true;
    }
    
    return false;
}

template <typename Object, typename Weight>
typename graph<Object, Weight>::Vertex* graph<Object, Weight>::insertVertex(Object o) {
    ///////////////////////////////////////////////
    // Implement vertex insertion here
    ///////////////////////////////////////////////
    Vertex* vert = new Vertex(o);
    vertex.push_back(vert);
    return vert;
}

template <typename Object, typename Weight>
void graph<Object, Weight>::insertEdge(Vertex* v, Vertex* w, Weight t) {
    ///////////////////////////////////////////////
    // Implement edge insertion here
    ///////////////////////////////////////////////
    Edge* e = new Edge(v, w, t);
    edge.push_back(e);
}

template <typename Object, typename Weight>
void graph<Object, Weight>::removeEdge(Edge* e) {
    ///////////////////////////////////////////////
    // Implement removing edge here
    ///////////////////////////////////////////////
    
    //remove start vertex's pointer
    e->start->edge.erase(std::remove(e->start->edge.begin(), e->start->edge.end(), e), e->start->edge.end());
    
    //remove end vertex's pointer
    e->end->inedge.erase(std::remove(e->end->inedge.begin(), e->end->inedge.end(), e), e->end->inedge.end());
    
    //remove edge
    edge.erase(std::remove(edge.begin(), edge.end(), e), edge.end());
    delete *e;
}

template <typename Object, typename Weight>
void graph<Object, Weight>::insertUndirectedEdge(Vertex* v, Vertex* w, Weight t) {
    ///////////////////////////////////////////////
    // Implement inserting undirected edge here
    ///////////////////////////////////////////////
    insertEdge(v, w, t);
    insertEdge(w, v, t);
}

template <typename Object, typename Weight>
void graph<Object, Weight>::removeVertex(Vertex* v) {
    ///////////////////////////////////////////////
    // Implement removing vertex here
    ///////////////////////////////////////////////
    
    //remove all edges
    typename vector<typename graph<Object, Weight>::Edge*>::iterator it;
    for (it = v->edge.end(); it != v->edge.begin(); --it)
        removeEdge(*it);
    
    delete *v;
}

template <typename Object, typename Weight>
size_t graph<Object, Weight>::numVertices() {
    return vertex.size();
}

template <typename Object, typename Weight>
size_t graph<Object, Weight>::numEdges() {
    return edge.size();
}

template <typename Object, typename Weight>
vector<typename graph<Object, Weight>::Vertex*> graph<Object, Weight>::vertices() {
    return vertex;
}

template <typename Object, typename Weight>
vector<typename graph<Object, Weight>::Edge*> graph<Object, Weight>::edges() {
    return edge;
}

template <typename Object, typename Weight>
void graph<Object, Weight>::print() {
    cout << "vertices:" << endl;
    for(size_t i=0; i<vertex.size(); i++)
        cout << vertex[i]->element << endl;
    cout << "edges:" << endl;
    for(size_t i=0; i<edge.size(); i++)
        cout << "(" << edge[i]->start->element << "," << edge[i]->end->element << ")" << endl;
}

template <typename Object, typename Weight>
void graph<Object, Weight>::read_file(std::string filename) {
    ///////////////////////////////////////////////
    // Implement reading file here
    ///////////////////////////////////////////////
    
    ifstream stream;
    stream.open(filename.c_str());
    
    //initialize graph with number of vertices equal to first value
    int num_vertices;
    stream >> num_vertices;
    for (int i=0; i<num_vertices; ++i)
        insertVertex(i);
    
    //insert edges until file end
    int x, y, w;
    while (stream >> x >> y >> w)
        insertEdge(vertex[x], vertex[y], w);
}

#endif




