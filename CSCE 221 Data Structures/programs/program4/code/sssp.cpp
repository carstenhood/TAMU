////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// sssp.cpp: single source shortest path implementation
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>
#include <map>      //for output map
#include <limits>   //to get maximum to define infinity for a templated type

//#include "AdjMatGraph.h"
#include "Graph.h"

#include "timing.h" //timing

//return a map from vertices to the length of the shortest path from the source
template <typename Object, typename Weight>
std::map<typename graph<Object, Weight>::Vertex*, Weight>
single_source_shortest_path(graph<Object, Weight>& g,
                            typename graph<Object, Weight>::Vertex* source) {
    
    //type definitions
    typedef typename graph<Object, Weight>::Vertex* VertexPtrType;
    typedef typename graph<Object, Weight>::Edge* EdgePtrType;
    typedef std::map<VertexPtrType, Weight> ReturnType;
    
    Weight INF = numeric_limits<Weight>::max();
    
    ReturnType paths; //output map from vertices to shortest path length
    vector<VertexPtrType> vertices = g.vertices(); //graph vertices

    //initialize vertex properties
    for (size_t i = 0; i < vertices.size(); ++i) {
        
        //set all vertices to unexplored
        vertices[i]->explored = false;
        
        if (vertices[i] == source)
            paths[vertices[i]] = 0; //set source distance to 0
        else
            paths[vertices[i]] = INF; //set other vertices to infinity
    }
    
    //main algorithm loop
    for (int i = 0; i < vertices.size()-1; ++i) {
        
        //find next closest vertex
        Weight min = INF;
        VertexPtrType x = NULL;
        for (int k = 0; k < vertices.size(); ++k) {
            
            VertexPtrType nearest = vertices[k];
            Weight nearest_dist = paths[nearest];
            
            //set vertex to nearest if closer to the root than nearest
            if (!nearest->explored && nearest_dist <= min) {
                x = nearest;
                min = nearest_dist;
            }
        }
        
        //break the algorithm if no vertex found
        if (x == NULL)
            break;
        
        x->explored = true;
        vector<EdgePtrType> x_edges = g.outgoingEdges(x);
        Weight x_dist = paths[x];
        
        //iterate through outgoing edges
        for (int k = 0; k < x_edges.size(); ++k) {
        
            //get edge
            EdgePtrType e = x_edges[k];

            //find next adjacent vertex
            VertexPtrType y = e->end;
            
            //get current distance from map
            Weight y_dist = paths[y];
            
            //calculate total distance from root through x to adjacent vertex y
            Weight new_dist = x_dist + e->weight;
            
            //if shorter path is found update path for adjacent vertex y
            if (!y->explored && x_dist != INF && new_dist < y_dist)
                paths[y] = new_dist;
        }
    }
    
    return paths;
}

template <typename G>
void populate_mesh(G& g, size_t n) {
    
    //add vertices
    for (size_t i = 0; i < n*n; ++i)
        g.insertVertex('a' + i);
    
    typedef std::vector<typename G::Vertex*> VerticesType;
    VerticesType vert = g.vertices();
    
    //add edges
    for (size_t i=0; i<n*n; i++) {
        size_t row_start = (i/n)*n;
        size_t x = i+1;
        size_t y = i+n;
        
        if (x < row_start+n)
            g.insertUndirectedEdge(vert[i], vert[x], i+x);
        if (y < n*n)
            g.insertUndirectedEdge(vert[i], vert[y], i+y);
    }
}

template <typename Vector>
void print_edges(Vector const& v) {
    for (typename Vector::const_iterator it = v.begin(); it != v.end(); ++it)
        std::cout << "(" << (*it)->start->element << ", "
        << (*it)->end->element << ") weight = "
        << (*it)->weight << std::endl;
}

int main(int argc, char* argv[]) {
    typedef char VertexProperty;
    typedef int EdgeProperty;
    typedef graph<VertexProperty, EdgeProperty> GraphType;
    
    //-------------------------------------
    //perform time experiments with timing.h
    
    cout << "\n------------------------" << endl;
    cout << "TEST TIMING\n" << endl;
    time_algorithm<GraphType>();
    
    //-------------------------------------
    //test correctness of algorithm on simple graph
    
    cout << "\n------------------------" << endl;
    cout << "TEST CORRECTNESS\n" << endl;
    const size_t n = 3;

    //GraphType g(n*n);
    GraphType g;

    // add vertices and edges
    //g.read_file("power.g");
    populate_mesh(g, n);
    
    // print out graph
    std::cout << "Graph:" << std::endl;
    print_edges(g.edges());
    
    GraphType::Vertex* src = g.vertices()[0];
    
    // compute SSSP
    std::map<GraphType::Vertex*, EdgeProperty> shortest_paths = single_source_shortest_path(g, src);
    
    // print out SSSP
    std::cout << "\nSSSP:" << std::endl;
    
    std::map<GraphType::Vertex*, EdgeProperty>::iterator it = shortest_paths.begin();
    std::map<GraphType::Vertex*, EdgeProperty>::iterator eit = shortest_paths.end();
    
    for(; it != eit; ++it)
        std::cout << it->first->element << " is " << it->second << " hops away from " << src->element << std::endl;
    
    cout << setprecision(10);
    
    return 0;
}
