// Carsten Hood
// UIN: 922009787
// CSCE 411-200H Fall 2016
// HW P due 2016/12/05
// kruskals.h


#ifndef kruskals_h
#define kruskals_h

#include "disjointsets.h"   // for kruskal's
#include "quicksort.h"      // for kruskal's

using namespace std;

// Kruskal's algorithm implementation
vector<edge> kruskal(Graph &graph) {
    DisjointSets disjoint_sets(graph.size());
    vector<edge> mst; // output edges
    
    // add all vertices to disjoint sets data structure
    for (int i = 0; i < graph.size(); ++i) {
        disjoint_sets.make_set(graph.get_vertex(i));
    }
    
    // sort all edges in ascending order by weight
    vector<edge> edge_queue = graph.get_edges();
    quick_sort(edge_queue, 0, edge_queue.size() - 1);
    
    // main loop processes edges in ascending order
    for (int i = 0; i < edge_queue.size(); i++) {
        edge e = edge_queue[i];
        // check if edge has already been added
        if (!disjoint_sets.is_connected(e.vert1, e.vert2)) {
            // add edge to output mst
            disjoint_sets.make_union(e.vert1, e.vert2);
            mst.push_back(e);
        }
    }
    
    return mst;
}

#endif /* kruskals_h */
