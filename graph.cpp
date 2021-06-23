//#include "graph.h"
#include <vector>
#include <unordered_map>
#include <stdio.h>

using namespace std;

template<class N, class V> class Edge;
template<class N, class V> class Node;

// NOTE: A very collision prone hash function that hashes the class pair
// TODO: Make this less collision prone
struct pairHash {
	template<class A, class B> size_t operator () (const pair<A, B> &p) const {
		//size_t h1 = hash<A>{}(p.first);
		//size_t h2 = hash<B>{}(p.second);
		size_t t = (size_t)(p.first)*10000 + (size_t)(p.second);
		
		return hash<int>{}(t);
    }
};

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
		Edge<N, V> *t = new Edge<V, N>(this, to, val);

		auto r = edges.emplace(pair<N, N>(data, to->data), t);

		if(!r.second){
			auto it = r.first;
			it->second->value = val;
			delete t;
			t = it->second;
		}

		printf("%p\n", t);

		return t;
	}
	// Add a preexisting edge to this nodes list (mainly for undirected graphs)
	Edge<N, V> *addEdge(Edge<N, V> *edge){
		edges.emplace(pair<N, N>(edge->from->data, edge->to->data), edge);
	}

	void removeEdge(Edge<N, V> *e){
		// Remove the edge from the graphs list
		//vectorErase<Edge<N, V> *>(edges, e);
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

		// Remove the edge from the graphs list
		//vectorErase<Edge<N, V> *>(edges, e);

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

		/*for(typename vector<Edge<N, V> *>::iterator it =
				nodes.at(from)->edges.begin();
					it != nodes.at(from)->edges.end(); it++){
			Edge<N, V> *t = *it;

			if(t->to->data == to){
				nodes.at(from)->edges.erase(it);
				e = t;
				break;
			}
		}
		vectorErase<Edge<N, V> *>(edges, e);*/
		
		printf("%p\n", e);

		delete e;
	}

	//Updates the value connected to the edge going between the two specified nodes
	void updateEdge(N from, N to, V value){
		/*for(typename vector<Edge<N, V> *>::iterator it =
				nodes.at(from)->edges.begin();
					it != nodes.at(from)->edges.end(); it++){
			Edge<N, V> *t = *it;

			if(t->to->data == to){
				t->value = value;
				break;
			}
		}*/
		edges.at(pair<N, N>(from, to))->value = value;
	}

	N getEdgeValue(N from, N to){
		/*for(typename vector<Edge<N, V> *>::iterator it =
				nodes.at(from)->edges.begin();
					it != nodes.at(from)->edges.end(); it++){
			Edge<N, V> *t = *it;

			if(t->to->data == to){
				return t->value;
			}
		}*/
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
		/*for(Edge<N, V> *e : edges){
			e->print();
		}*/
	}
};

int main(){
	Graph<int, int> *g = new Graph<int, int>();
	g->addNode(0);
	g->addNode(1);
	g->addEdge(0, 1, 1);
	
	g->print();

	g->updateEdge(0, 1, 5);
	g->print();

	g->updateEdge(0, 1, g->getEdgeValue(0, 1) + 1);
	g->print();

	g->removeEdge(0, 1);

	g->print();

	for(int i = 0; i < 0; i++){
		g->addNode(i);
	}
	printf("Done with adding nodes!\n");

	for(int i = 0; i < g->nodes.size(); i++){
		for(int j = i+1; j < g->nodes.size(); j++){
			g->addEdge(g->nodes.at(i), g->nodes.at(j), 1);
		}
	}
	printf("Done with adding edges!\n");

	//g->print();

	/*for(Edge<int, int> *e : g.edges){
		g.removeEdge(e);

		//if(a % 10000 == 0) printf("%d\n", a);

		a++;
	}*/
	/*while(g->edges.size() > 0){
		g->removeEdge(g->edges.at(0));
	}*/

	for(int i = 0; i < g->nodes.size(); i++){
		for(int j = i+1; j < g->nodes.size(); j++){
			g->removeEdge(g->nodes.at(i)->data, g->nodes.at(j)->data);
		}
	}

	delete g;
	printf("Done with removing edges!\n");

	int a = 0;
	for(int i = 0; i < 10000000000000000; i++){
		a += 1;
	}

	return 0;
}
