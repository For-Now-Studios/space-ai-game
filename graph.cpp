#pragma once
#include <vector>
#include <unordered_map>
#include <queue>
#include <stdio.h>
#include <limits.h>
#include <cstddef>
#include <utility>

using namespace std;

template<class N, class V> class Node;

// NOTE: A very collision prone hash function that hashes the class pair
// TODO: Make this less collision prone
struct pairHash {
	template<class A, class B> size_t operator () (const pair<A, B> &p) const {
		size_t t = (size_t)(p.first)*10000 + (size_t)(p.second);
		
		return hash<int>{}(t);
    }
};

/*
	Utility function to allow Pairs to be used within a priority queue
*/
struct pairCmp{
	template<class N> bool operator () (pair<N, int> a, pair<N, int> b){
		return a.second > b.second;
	}
};

template<class N, class V> class Edge{
	public:
	Node<N, V> *from;
	Node<N, V> *to;
	V value;

	Edge(){

	}

	Edge(Node<N, V> *f, Node<N, V> *t, V v){
		value = v;
		from = f;
		to = t;
	}

	//Prints the edge showing the pointer to from and to, and treat value as a num
	void print(){
		printf("Edge: %p From: %p, To: %p, Value: %d\n", this, from, to, value);
	}
};

template<class N, class V> class Node{

	public:
	N data;
	unordered_map<pair<N, N>, Edge<N, V> *, pairHash> edges;

	Node(){
		data = nullptr;
	}
	Node(N d){
		data = d;
	}

	// Add a new edge going from this node to the specified node
	Edge<N, V> *addEdge(Node<N, V> *to, V val){
		Edge<N, V> *t = new Edge<N, V>(this, to, val);

		auto r = edges.emplace(pair<N, N>(data, to->data), t);

		if(!r.second){
			auto it = r.first;
			it->second->value = val;
			delete t;
			t = it->second;
		}

		return t;
	}
	// Add a preexisting edge to this nodes list (mainly for undirected graphs)
	Edge<N, V> *addEdge(Edge<N, V> *edge){
		edges.emplace(pair<N, N>(edge->from->data, edge->to->data), edge);
	}

	void removeEdge(Edge<N, V> *e){
		// Remove the edge from the graphs list
		auto r = edges.find(pair<N, N>(e->from->data, e->to->data));
		if(r != edges.end()) edges.erase(r);
	}

	void removeEdge(N to){
		auto r = edges.find(pair<N, N>(data, to));
		if(r != edges.end()) edges.erase(r);
	}

	void print(){
		printf("Node: %p, Data: %d\n", this, data);
		for(typename unordered_map<pair<N, N>, Edge<N, V> *>::iterator it =
						edges.begin(); it != edges.end(); it++){
			printf("\t");
			it->second->print();
			printf("\n");
		}
	}
};

template<class N, class V> class Graph{
	public:
	unordered_map<N, Node<N, V> *> nodes;
	unordered_map<pair<N, N>, Edge<N, V> *, pairHash> edges;

	Graph(){
		//Empty
	}

	~Graph(){
		for(pair<N, Node<N, V> *> p : nodes){
			delete p.second;
		}

		for(pair<pair<N, N>, Edge<N, V> *> p : edges){
			delete p.second;
		}
	}
	void addNode(N key){
		Node<N, V> *n = new Node<N, V>(key);
		auto r = nodes.emplace(key, n);

		//If the node allready existed in the graph, delete the latest
		if(!r.second) delete n;
	}

	Edge<N, V> *addEdge(N from, N to, V val){
		Node<N, V> *n = nodes.at(from);
		Edge<N, V> *e = n->addEdge(nodes.at(to), val);
		edges.emplace(pair<N, N>(from, to), e);

		return e;
	}
	Edge<N, V> *addEdge(Node<N, V> *from, Node<N, V> *to, V val){
		Edge<N, V> *e = from->addEdge(to, val);
		edges.emplace(pair<N, N>(from->data, to->data), e);

		return e;
	}
	
	//Remove the specified edge
	void removeEdge(Edge<N, V> *e){
		edges.erase(edges.find(pair<N, N>(e->to->data, e->from->data)));

		// Remove the edge from the lists of nodes it goes between
		e->from->removeEdge(e);
		e->to->removeEdge(e);

		delete e;
	}
	//Remove an edge going from the specified node to the specified node
	void removeEdge(N from, N to){
		auto it = edges.find(pair<N, N>(from, to));
		Edge<N, V> *e = it->second;
		edges.erase(it);
		e->from->removeEdge(e);
		e->to->removeEdge(e);

		delete e;
	}

	//Updates the value connected to the edge going between the two specified nodes
	void updateEdge(N from, N to, V value){
		edges.at(pair<N, N>(from, to))->value = value;
	}

	N getEdgeValue(N from, N to){
		return edges.at(pair<N, N>(from, to))->value;
	}

	void print(){
		printf("Nodes:\n");
		for(typename unordered_map<N, Node<N, V> *>::iterator it = nodes.begin();
							it != nodes.end(); it++){
			it->second->print();
		}
		printf("\nEdges:\n");
		for(typename unordered_map<pair<N, N>, Edge<N, V> *>::iterator it =
						edges.begin(); it != edges.end(); it++){
			it->second->print();
		}
	}
};

/*
	Finds the shortest path from point a to point b in a undirected graph, where the
	edges are assumed to be positive integers
*/
template<class N> std::vector<N> *dijkstra(Graph<N, int> *g, N a, N b){
	unordered_map<N, int> dist;
	unordered_map<N, N> prev;
	dist.reserve(g->nodes.size());
	prev.reserve(g->nodes.size());

	//Initialize the cost of reaching a node to "infinity"
	for(pair<N, Node<N, int> *> p : g->nodes){
		dist.emplace(p.first, INT_MAX);
		prev.emplace(p.first, p.first);
	}

	priority_queue<pair<N, int>, vector<pair<N, int>>, pairCmp> q;
	dist.at(a) = 0;
	q.push(pair<N, int>(a, dist.at(a)));

	while(!q.empty()){
		//Check if the queued element has been updated (hence outdated)
		if(dist.at(q.top().first) != q.top().second){
			q.pop();
			continue;
		}

		N cur = q.top().first;
		q.pop();

		if(cur == b) break; //The node with lowest cost is our target, hence done

		for(pair<pair<N, N>, Edge<N, int> *> p : g->nodes.at(cur)->edges){
			N neighbour = p.second->to->data;
			int weight = p.second->value;

			if(dist.at(neighbour) > dist.at(cur) + weight){
				dist.at(neighbour) = dist.at(cur) + weight;
				prev.at(neighbour) = cur;

				q.push(pair<N, int>(neighbour, dist.at(neighbour)));
			}
		}
	}

	if(prev.at(b) == b) return nullptr;

	vector<N> *path = new vector<N>();
	path->push_back(b);

	N pre = prev.at(b);
	while(pre != a){
		path->push_back(pre);
		pre = prev.at(pre);
	}

	return path;
}
