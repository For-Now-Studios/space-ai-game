//#include "graph.h"
#include <vector>
#include <unordered_map>
#include <stdio.h>

using namespace std;

template<class N, class V> class Edge;
template<class N, class V> class Node;

template<class N> void vectorErase(vector<N> &v, N e){
	for(typename vector<N>::iterator it = v.begin(); it != v.end(); it++){
		if(*it == e){
			v.erase(it);
			break;
		}
	}	
}

template<class N, class V> class Edge{
	public:
	Node<N, V> *from;
	Node<N, V> *to;
	V value;

	Edge(Node<N, V> *f, Node<N, V> *t, V v){
		value = v;
		from = f;
		to = t;
	}

	//Prints the edge showing the pointer to from and to, and treat value as a num
	void print(){
		printf("From: %p, To: %p, Value: %d\n", from, to, value);
	}
};

template<class N, class V> class Node{
	public:
	N data;
	vector<Edge<N, V> *> edges;

	Node(){
		data = nullptr;
	}
	Node(N d){
		data = d;
	}

	// Add a new edge going from this node to the specified node
	Edge<N, V> *addEdge(Node<N, V> *to, V val){
		Edge<N, V> *t = new Edge<V, N>(this, to, val);

		edges.push_back(t);

		return t;
	}
	// Add a preexisting edge to this nodes list (mainly for undirected graphs)
	Edge<N, V> *addEdge(Edge<N, V> *edge){
		edges.push_back(edge);
	}

	void removeEdge(Edge<N, V> *e){
		// Remove the edge from the graphs list
		vectorErase<Edge<N, V> *>(edges, e);
	}

	void print(){
		printf("Node: %p, Data: %d\n", this, data);
		for(Edge<N, V> *e : edges){
			printf("\t");
			e->print();
			printf("\n");
		}
	}
};

template<class N, class V> class Graph{
	unordered_map<N, Node<N, V> *> nodes;
	vector<Edge<N, V> *> edges;

	public:
	Graph(){
		//Empty
	}

	~Graph(){
		for(pair<N, Node<N, V> *> p : nodes){
			delete p.second;
		}

		for(Edge<N, V> *e : edges){
			delete e;
		}
	}
	void addNode(N key){
		Node<N, V> *n = new Node<N, V>(key);
		nodes.emplace(key, n);
	}

	Edge<N, V> *addEdge(N from, N to, V val){
		Node<N, V> *n = nodes.at(from);
		Edge<N, V> *e = n->addEdge(nodes.at(to), val);
		edges.push_back(e);

		return e;
	}
	
	//Remove the specified edge
	void removeEdge(Edge<N, V> *e){
		// Remove the edge from the graphs list
		vectorErase<Edge<N, V> *>(edges, e);

		// Remove the edge from the lists of nodes it goes between
		e->from->removeEdge(e);
		e->to->removeEdge(e);

		delete e;
	}
	//Remove an edge going from the specified node to the specified node
	//NOTE: Treats the edges as directional
	void removeEdge(N from, N to){		
		Edge<N, V> *e;
		for(typename vector<Edge<N, V> *>::iterator it =
				nodes.at(from)->edges.begin();
					it != nodes.at(from)->edges.end(); it++){
			Edge<N, V> *t = *it;

			if(t->to->data == to){
				nodes.at(from)->edges.erase(it);
				e = t;
				break;
			}
		}
		vectorErase<Edge<N, V> *>(edges, e);

		delete e;
	}

	void print(){
		printf("Nodes:\n");
		for(typename unordered_map<N, Node<N, V> *>::iterator it = nodes.begin();
							it != nodes.end(); it++){
			it->second->print();
		}
		printf("\nEdges:\n");
		for(Edge<N, V> *e : edges){
			e->print();
		}
	}
};

int main(){
	Graph<int, int> g;
	g.addNode(0);
	g.addNode(1);
	g.addEdge(0, 1, 1);
	
	g.print();

	g.removeEdge(0, 1);

	g.print();

	return 0;
}
