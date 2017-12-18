////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// sssp.cpp: single source shortest path implementation
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <ctime>    //for timing
#include <iomanip>   //for setprecision
#include <math.h>

//#include "AdjMatGraph.h"
#include "Graph.h"

//---------------------------------------------------------------
//functions for generating graphs

template <typename G>
void populate_real(G& g, size_t n) {
    
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
            g.insertUndirectedEdge(vert[i], vert[x], 1+rand()%n);
        if (y < n*n)
            g.insertUndirectedEdge(vert[i], vert[y], 1+rand()%n);
    }
}

template <typename G>
void populate_dense(G& g, size_t n) {
    
    //add vertices
    for (size_t i = 0; i < n*n; ++i)
        g.insertVertex('a' + i);
    
    typedef std::vector<typename G::Vertex*> VerticesType;
    VerticesType vert = g.vertices();
    
    //add edges
    for (size_t i=0; i<n*n-1; i++) {
        for (size_t k=i+1; k<n*n; ++k) {
            g.insertUndirectedEdge(vert[i], vert[k], 1+rand()%n);
        }
    }
}

template <typename G>
void populate_sparse(G& g, size_t n) {
    
    //add vertices
    for (size_t i = 0; i < n*n; ++i)
        g.insertVertex('a' + i);
    
    typedef std::vector<typename G::Vertex*> VerticesType;
    VerticesType vert = g.vertices();
    
    //add edges
    for (size_t i=0; i<n*n-1; i++) {
        g.insertUndirectedEdge(vert[i], vert[i+1], 1+rand()%n);
    }
}

//---------------------------------------------------------------
//functions for timing

//averages the running time over _num_itr iterations and returns the average time.
template <typename G>
double run_test(G &g, size_t _num_itr) {
    
    typedef int EdgeProperty;
    typedef char VertexProperty;
    typedef graph<VertexProperty, EdgeProperty> GraphType;
    
    //determine source vertex
    GraphType::Vertex* src = g.vertices()[0];
    
    //initialize output map
    std::map<GraphType::Vertex*, EdgeProperty> shortest_paths;
    
    //start timing at new tick
    clock_t k = clock();
    clock_t start;
    do start = clock();
    while (start == k);
    
    //compute SSSP
    for (int itr=0; itr<_num_itr; ++itr)
         shortest_paths = single_source_shortest_path(g, src);
    
    //end timing
    clock_t end = clock();
    
    //show size and iterations
    //cout << shortest_paths.size() << ", ";
    //cout << _num_itr << ", ";
    
    //calculate average time per iteration
    double elapsed_time = double(end - start) / double(CLOCKS_PER_SEC);
    //cout << elapsed_time << ", "; //shows total time
    return elapsed_time / double(_num_itr);
}

//calls run_test over each size
template <typename G>
void initiate_test(string which_graph) {
    
    //definitions
    typedef char VertexProperty;
    typedef int EdgeProperty;
    typedef graph<VertexProperty, EdgeProperty> GraphType;
    
    //set size range
    size_t lower = 2;
    size_t upper = (size_t)pow(2, 13);
    
    //iterate from lower size bound to upper size bound
    for (size_t n = lower; n*n < upper; n*=1.5) {
        size_t iterations = size_t(pow(upper, 1) / (1 + pow(n, 2) - pow(lower, 2)));
        if (iterations < 1) iterations = 1;
        
        //initialize graph and fill
        typedef graph<VertexProperty, EdgeProperty> GraphType;
        
        //GraphType g(n*n);
        GraphType g;

        //choose graph type
        if (which_graph == "real")
            populate_real(g, n);
        else if (which_graph == "sparse")
            populate_sparse(g, n);
        else if (which_graph == "dense")
            populate_dense(g, n);
        
        //run test over iterations
        cout << run_test(g, iterations) << endl;
    }
}

//calls initiate_test for each graph type
template <typename G>
void time_algorithm() {

    //determines which tests are performed
    vector<string> test_types;
    test_types.push_back("real");
    test_types.push_back("sparse");
    test_types.push_back("dense");
    
    for (int i=0; i<test_types.size(); ++i) {
        //display which graph type is being tested
        cout << "-- Testing " << test_types[i] << "() graph:" << endl;
        
        cout << "Size, Iter, Time(sec)" << endl;
        initiate_test<G>(test_types[i]);
        
        cout << endl;
    }
}