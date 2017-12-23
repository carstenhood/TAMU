////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// BreadthFirstSearch.cpp: Breadth First Search implementation
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <queue>


#define ADJMATRIX
#ifndef ADJMATRIX
#include "Graph.h"
#else
#include "AdjMatGraph.h"
#endif

using namespace std;

// return a vector of vertices listed in the breadth-first search traversal order rooted at source
template <typename G>
vector<typename G::Vertex*>
BFS(G& g, typename G::Vertex* source) {
  typedef typename G::Vertex* VertexPtrType;
  typedef std::vector<VertexPtrType> VectorType; 

  VectorType visitation;

  VectorType vertices = g.vertices();
  for(typename VectorType::iterator it = vertices.begin(); it != vertices.end(); ++it)
    (*it)->color = 0;

  std::queue<VertexPtrType> q;
  q.push(source);

  while(!q.empty()) {
    VertexPtrType v = q.front();
    q.pop();
    if(v->color == 0) {
      visitation.push_back(v);
      v->color = 1;
      VectorType adj = g.adjacentVertices(v);

      for(typename VectorType::iterator u = adj.begin(); u != adj.end(); ++u)
        q.push(*u);

      v->color = 2;
    }
  }

  for(typename VectorType::iterator it = visitation.begin(); it != visitation.end(); ++it)
    std::cout << (*it)->element << " ";
  std::cout << std::endl;

  return visitation;
}

template <typename G>
void populate_complete(G& g, size_t n) {
  // add vertices
  for(size_t i = 0; i < n; ++i)
    g.insertVertex('a'+i);

  typedef std::vector<typename G::Vertex*> VerticesType;
  VerticesType vert = g.vertices();

  // add edges
  for(typename VerticesType::iterator it = vert.begin(); it != vert.end(); ++it)
    for(typename VerticesType::iterator eit = vert.begin(); eit != vert.end(); ++eit)
      if(*it != *eit)
        g.insertEdge(*it, *eit, rand() % 100);
}


int main(int argc, char* argv[]) {
  typedef char VertexProperty;
  typedef int EdgeProperty;
  typedef graph<VertexProperty, EdgeProperty> GraphType;

  const size_t n = 20;

#ifndef ADJMATRIX
  graph<VertexProperty, EdgeProperty> g;
#else
  graph<VertexProperty, EdgeProperty> g(n);
#endif

  // test adding vertices and edges
  populate_complete(g, n);

  cout << "calling BFS." << endl;
  BFS(g, g.vertices()[0]);
  cout << "done calling BFS!" << endl;

  return 0;
}

