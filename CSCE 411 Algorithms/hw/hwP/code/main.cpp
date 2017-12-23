// Carsten Hood
// UIN: 922009787
// CSCE 411-200H Fall 2016
// HW P due 2016/12/05
// main.cpp

#include <cstdlib>
#include <iostream>
#include <vector>
#include <sys/time.h>

#include "graph.h"      // graph data structure
#include "prims.h"      // Prim's algorithm
#include "kruskals.h"   // Kruskal's algorithm

using namespace std;

// prints output MST solutions
void print_mst(Graph &graph, vector<edge> &mst) {
    int total_weight = 0;
    for (int i = 0; i < mst.size(); ++i) {
        //mst[i].print();
        total_weight += mst[i].weight;
    }
    cout << "Total weight: " << total_weight << endl;
}

// outputs time for comparing algorithm complexities
void print_time(timeval &start_time, timeval &end_time) {
    // calculate and output total elapsed microseconds
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long microsecs = end_time.tv_usec - start_time.tv_usec;
    if (microsecs < 0) {
        microsecs += 1000000;
        seconds -= 1;
    }
    long microsec_time = seconds*1000000 + microsecs;
    cout << microsec_time << " microsecs" << endl;
}

// runs and times given algorithm on given graph
void run_algorithm(string alg, Graph &graph) {
    vector<edge> mst;
    timeval start_time;
    timeval end_time;
    
    cout << alg << " runtime: ";
    
    // time algorithm runtime
    gettimeofday(&start_time, NULL);
    if (alg == "Prim's")
        mst = prim(graph);
    else if (alg == "Kruskal's")
        mst = kruskal(graph);
    else
        cout << "Unrecognized algorithm." << endl;
    gettimeofday(&end_time, NULL);
    
    print_time(start_time, end_time);
    // print_mst(graph, mst);
}

// test & time the MST functions
int main() {
    cout << "Commence tests:" << endl;
    
    // runs four different tests for each of i different-sized graphs
    for (int i = 0; i < 4; ++i) {
        int max_weight = 100;
        
        // test increasing powers of 10
        int num_vertices = 1;
        if (i == 0) num_vertices = 2;
        for (int j = 0; j < i; ++j)
            num_vertices *= 10;
            
        cout << "Testing with " << num_vertices << " vertices and "
             << "random weights from 1 to " << max_weight << ":" << endl;
        
        // test completely connected graph
        cout << "CONNECTED GRAPH" << endl;
        Graph complete_graph(num_vertices, max_weight, false);
        //complete_graph.print();
        run_algorithm("Prim's", complete_graph);
        run_algorithm("Kruskal's", complete_graph);
        
        // test linked-list / chain graph
        cout << "CHAINED GRAPH" << endl;
        Graph chain_graph(num_vertices, max_weight, true);
        //chain_graph.print();
        run_algorithm("Prim's", chain_graph);
        run_algorithm("Kruskal's", chain_graph);
        
        cout << endl;
    }
    
    cout << "Tests complete." << endl;
}
