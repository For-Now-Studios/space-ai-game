#pragma once
#include <vector>

using namespace std;

template<class V, class N>
class Graph{
	Node<N> node;
	Edge<V> edge;

	class Edge{
		Node *from;
		Node *to;
		V value;

		Edge(Node *f, Node* t, V v){
			value = v;
			from = f;
			to = t;
		}
	};

	class Node{
		N *data;
		vector<Edge<V> *> edges;

		Node(){
			data = nullptr;
		}
		Node(N *d){
			data = d;
		}

		addEdge(Edge<V> *edge){
			edges.push_back(edge);
		}
		addEdge(Node *to){
			addEdge(new Edge<V>(this, to, 0);
		}
		addEdge(Node *to, V val){
			addEdge(new Edge<V>(this, to, val);
		}
	};
};
