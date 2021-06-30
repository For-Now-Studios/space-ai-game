#pragma once
#include <vector>
#include <unordered_map>
#include <queue>
#include <stdio.h>
#include <limits.h>
#include <cstddef>
#include <utility>

template<class N, class V> class Edge;
template<class N, class V> class Node;
template<class N, class V> class Graph;

template<class N> std::vector<N> *dijkstra(Graph<N, int> *g, N a, N b);

#include "graph.cpp"
