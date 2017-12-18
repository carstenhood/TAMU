// Carsten Hood
// UIN: 922009787
// CSCE 411-200H Fall 2016
// HW P due 2016/12/05
// prims.h

#ifndef prims_h
#define prims_h

#include "priorityqueue.h"  // for prim's

using namespace std;

// Prim's algorithm implementation
vector<edge> prim(Graph &graph) {
    vector<edge> mst; // set of output edges
    
    // initialize algorithm structures
    PriorityQueue pq(graph.size());
    int infinity = graph.get_infinite_weight();
    vector<int> best_weight(graph.size());  // maps Vs to weight of shortest edge
    vector<int> best_vertex(graph.size());  // maps Vs to nearest neighbor
    vector<bool> is_in_pq(graph.size());    // true if V is still in the PQ
    for (int i = 0; i < graph.size(); ++i) {
        best_vertex[i] = -1;            // closest vertices are intially unknown
        best_weight[i] = infinity;      // weigh initially high number
        pq.insert(make_pair(best_weight[i], i));    // insert into priority queue
        is_in_pq[i] = true;             // all vertices initially in the PQ
    }
    
    int v0 = 0;  // begin main loop with arbitrary first vertex
    pq.decrease_key(v0, 0);
    while (!pq.is_empty()) {        // main loop
        int u = pq.extract_min();
        is_in_pq[u] = false;
        if (u != v0) {              // doesn't process this block on first iteration
            int x = best_vertex[u];
            int weight = graph.get_edge_weight(u, x);   // matrix lookup
            mst.push_back(edge(u, x, weight));          // add edge to output MST
        }
        for (int v = 0; v < graph.size(); ++v) {        // process all neighbors of u
            if (v == u) continue;
            int weight = graph.get_edge_weight(u, v);
            // update nearest vertex of v to u if u is nearer than previous nearest
            if (weight > 0 && is_in_pq[v] && weight < best_weight[v]) {
                best_vertex[v] = u;
                best_weight[v] = weight;
                pq.decrease_key(v, weight);
            }
        }
    }
    return mst;
}

#endif /* prims_h */
