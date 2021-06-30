#include "structs.h"
#include "graph.h"

Graph<CharacterObject *, Relation> *initRelations(vector<CharacterObject *> *chars){
	Graph<CharacterObject *, Relation> *g = new Graph<CharacterObject *, Relation>();

	for(CharacterObject *c : *chars){
		g->addNode(c);

		for(pair<CharacterObject *, Node<CharacterObject *, Relation> *> p :
									     g->nodes){
			if(c != p.first){
				g->addEdge(c, p.first, Neutral);
				g->addEdge(p.first, c, Neutral);
			}
		}
	}

	return g;
}
