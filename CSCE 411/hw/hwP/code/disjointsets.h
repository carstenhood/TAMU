// Carsten Hood
// UIN: 922009787
// CSCE 411-200H Fall 2016
// HW P due 2016/12/05
// disjointsets.h

// custom disjoint sets implementation

// significant references:
// http://thecodersportal.com/mst-krushkal/;

#ifndef disjointsets_h
#define disjointsets_h

#include <map>

using namespace std;

// disjoint set node
typedef struct set {
    int identifier;
    struct set* parent;
    int weight;
} set;


// disjoint sets class, used by Kruskal's algorithm
class DisjointSets {
private:
    int total_sets;             // max number of elements
    int set_count;              // track number of sets
    map <int, set*> node_map;   // map node identifiers to their addresses
    set* find_set_helper(set* node);
public:
    DisjointSets(int n) { total_sets = n; }
    bool is_connected(int x, int y);
    void make_set(int identifier);
    int find_set(int x);
    void make_union(int x, int y);
};

// create a new disjoint set with given identifier as representative element
void DisjointSets::make_set(int identifier) {
    set* node = new set;
    node->identifier = identifier;
    node_map[identifier] = node;
    node->weight = 0;
    node->parent = node;
    ++set_count;
}

// find a node given its identifier
int DisjointSets::find_set(int x) {
    set* node = node_map[x];
    node = find_set_helper(node);
    return node->identifier;
}

// helper function to find_set
// uses path compression
set* DisjointSets::find_set_helper(set* node) {
    if (node != node->parent)
        node->parent = find_set_helper(node->parent);
    return node->parent;
}

// use union by weight to merge two sets
void DisjointSets::make_union(int x, int y) {
    x = find_set(x);
    y = find_set(y);
    if (is_connected(x,y))
        return;
    
    set* node1 = node_map[x];
    set* node2 = node_map[y];
    if (node1->weight > node2->weight)
        node2->parent = node1;
    else
        node1->parent = node2;
    if (node1->weight == node2->weight) {
        node1->weight = node1->weight+1;
    }
    --set_count;
}

// checks whether given elements are in the same set
bool DisjointSets::is_connected(int x, int y) {
    return (find_set(x) == find_set(y));
}

#endif
/* disjointsets_h */
