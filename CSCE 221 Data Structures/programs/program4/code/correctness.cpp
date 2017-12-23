#include <iostream>
#include <fstream>
#include <algorithm>

#ifndef ADJMATRIX
#include "Graph.h"
#else
#include "AdjMatGraph"
#endif

void print(graph<double,double>::Vertex* v) {
  std::cout << v->element << ", " << v->level << ", " << v->starttime << ", " << v->finishtime << std::endl;
}

template <typename G>
void populate_complete(G& g, size_t n) {
  // add vertices
  for(size_t i=0; i<n; ++i)
    g.insertVertex('a'+i);

  typedef std::vector<typename G::Vertex*> VerticesType;
  VerticesType vert = g.vertices();

  // add edges
  for(typename VerticesType::iterator it = vert.begin(); it != vert.end(); ++it)
    for(typename VerticesType::iterator eit = vert.begin(); eit != vert.end(); ++eit)
      if(*it != *eit)
        g.insertEdge(*it, *eit, rand() % 100);
}

template <typename Graph>
struct edge_equal {
  Graph* m_g;
  typename Graph::Vertex* m_v;
  bool m_end;

  edge_equal(Graph* g, typename Graph::Vertex* v, bool end)
    : m_g(g), m_v(v), m_end(end)
  { }

  bool operator()(typename Graph::Edge* const& e) {
    return (m_end ? m_g->endVertex(e) : m_g->startVertex(e)) == m_v;
  }
};

int main(int argc, char* argv[]) {
  typedef char VertexProperty;
  typedef int EdgeProperty;
  typedef graph<VertexProperty, EdgeProperty> GraphType;

  const size_t n = 20;
  GraphType g;

  // test adding vertices and edges
  populate_complete(g, n);

  // test number of vertices
  if(g.numVertices() != n)
    cerr << "Size of graph is incorrect." << endl;

  // test number of edges
  if(g.numEdges() != n*n-n)
    cerr << "Number of edges is incorrect." << endl;

  // get vertices
  typedef std::vector<GraphType::Vertex*> VerticesType;
  VerticesType vert = g.vertices();

  bool passed = true;

  // test adjacency of every vertex
  for(VerticesType::iterator it = vert.begin(); it != vert.end(); ++it)
    for(VerticesType::iterator eit = vert.begin(); eit != vert.end(); ++eit)
      if(*it != *eit)
        passed &= g.isAdjacent(*it, *eit);
      else
        passed &= !g.isAdjacent(*it, *eit);

  if(!passed) 
    cerr << "Adjacency information is incorrect." << endl;


  // grab random vertex v
  GraphType::Vertex* v = vert.front();

  // test degrees of v
  if(g.indegree(v) != n-1)
    cerr << "In-degree is incorrect." << endl;
  if(g.outdegree(v) != n-1)
    cerr << "Out-degree is incorrect." << endl;
  if(g.degree(v) != 2*(n-1))
    cerr << "Degree is incorrect." << endl;

  // test incident vertices of v
  VerticesType incident = g.incidentVertices(v);
  passed = true;

  // make sure every vertex (except v) appears exactly once in list of incident vertices
  for(VerticesType::iterator it = vert.begin(); it != vert.end(); ++it)
    if(*it != v)
      passed &= std::count(incident.begin(), incident.end(), *it) == 1;

  if(!passed)
    cerr << "Incident vertices is incorrect." << endl;

  // test adjacent vertices of v
  VerticesType adjacent = g.adjacentVertices(v);
  passed = true;

  // make sure every vertex (except v) appears exactly once in list of adjacent vertices
  for(VerticesType::iterator it = vert.begin(); it != vert.end(); ++it)
    if(*it != v)
      passed &= std::count(adjacent.begin(), adjacent.end(), *it) == 1;

  if(!passed)
    cerr << "Adjacent vertices is incorrect." << endl;


  // get outgoing edges of v
  typedef std::vector<GraphType::Edge*> EdgesType;
  EdgesType out_edges = g.outgoingEdges(v);
  passed = true;

  // make sure every vertex (except v) appears exactly once as the target of an outgoing edge of v
  for(VerticesType::iterator it = vert.begin(); it != vert.end(); ++it)
    if(*it != v)
      passed &= std::count_if(out_edges.begin(), out_edges.end(), edge_equal<GraphType>(&g, *it, true)) == 1;

  if(!passed)
    cerr << "Outgoing edges or end vertex is incorrect." << endl;

  // make sure the source of all outgoing edges is v
  passed = std::count_if(out_edges.begin(), out_edges.end(), edge_equal<GraphType>(&g, v, false)) == (int)out_edges.size();

  if(!passed)
    cerr << "Outgoing edges or start vertex is incorrect." << endl;

  // get incident edges of v
  typedef std::vector<GraphType::Edge*> EdgesType;
  EdgesType in_edges = g.incidentEdges(v);
  passed = true;

  // make sure every vertex (except v) appears exactly once as the source of an incident edge of v
  for(VerticesType::iterator it = vert.begin(); it != vert.end(); ++it)
    if(*it != v)
      passed &= std::count_if(in_edges.begin(), in_edges.end(), edge_equal<GraphType>(&g, *it, false)) == 1;

  if(!passed)
    cerr << "Incident edges or start vertex is incorrect." << endl;

  // make sure the target of all incident edges is v
  passed = std::count_if(in_edges.begin(), in_edges.end(), edge_equal<GraphType>(&g, v, true)) == (int)in_edges.size();

  if(!passed)
    cerr << "Incident edges or end vertex is incorrect." << endl;

  // create copy of graph
  GraphType copy(g);

  if(g.numVertices() != copy.numVertices() || g.numEdges() != copy.numEdges())
    cerr << "Size of copy is incorrect." << endl;

  // test reading in from a file
  GraphType g_file;
  std::string filename = "power.g";

  g_file.read_file(filename);

  ifstream stream;
  stream.open(filename.c_str());
  int num_vertices;
  stream >> num_vertices;

  if(num_vertices != (int)g_file.numVertices())
    cerr << "Size of graph from file is incorrect." << endl;

  std::cout << "All tests passed!" << std::endl;

  return 0;
}

