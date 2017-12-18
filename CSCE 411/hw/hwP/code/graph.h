// Carsten Hood
// UIN: 922009787
// CSCE 411-200H Fall 2016
// HW P due 2016/12/05
// graph.h

// custom graph implementation

#ifndef graph_h
#define graph_h

using namespace std;

// edge struct contains both vertices and edge weight
typedef struct edge {
    int vert1;
    int vert2;
    int weight;
    
    // constructor
    edge(int v1, int v2, int w)
    : vert1(v1), vert2(v2), weight(w) { }
    
    // for debugging
    void print() {
        cout << vert1 << "---" << vert2 << " weighted " << weight << endl;
    }
} edge;

// graph class to be processed by Kruskal's and Prim's MST algorithms
class Graph {
private:
    vector<vector<int>> matrix; // for constant time edge look-up
    vector<edge> edges;
    vector<int> vertices;
    int weight_limit;
public:
    Graph(int size, int max_weight, bool chain);
    int size();
    int edge_count();
    int get_infinite_weight();
    int get_vertex(int i);
    vector<edge> get_edges();
    vector<int> get_neighbors(int v);
    int get_edge_weight(int v1, int v2);
    void print();
};

// generate a random undirected graph with edges between all vertices
Graph::Graph(int size, int max_weight, bool chain) {
    matrix = vector<vector<int>>(size);
    for (int i = 0; i < size; ++i) {
        vector<int> row = vector<int>(size);
        for (int j = 0; j < i; ++j) {
            int weight = -1;
            // if generating a chain only generate one edge per vertex
            if (j < i && (!chain || j == i-1)) {
                weight = 1 + rand() % (max_weight - 1);
                edges.push_back(edge(i, j, weight));    // add to edges
                
                // reflect edge onto other side of matrix
                matrix[j][i] = weight;
            }
            row[j] = weight; // add weight (edge) value to row
        }
        matrix[i] = row; // add new row to matrix structure
        vertices.push_back(i);
    }
    weight_limit = max_weight + 1;
}

// display graph for testing
void Graph::print() {
    for (int i = 0; i < edges.size(); ++i) {
        edges[i].print();
    }
    cout << endl;
}

// access number of vertices, n
int Graph::size() {
    return vertices.size();
}

// access number of edges
int Graph::edge_count() {
    return edges.size();
}

// returns weight value greater than maximum sum of all edge weights
int Graph::get_infinite_weight() {
    return weight_limit * edges.size();
}

// access all edges
vector<edge> Graph::get_edges() {
    return edges;
}

// access a vertex by its index
int Graph::get_vertex(int i) {
    return vertices[i];
}

// access array of a vertex's neighbors
vector<int> Graph::get_neighbors(int v) {
    return matrix[v];
}

// access weight of edge given its two vertices using matrix structure
int Graph::get_edge_weight(int v1, int v2) {
    // cout << v1 << ", " << v2 << endl;
    return matrix[v1][v2];
}

#endif /* graph_h */
