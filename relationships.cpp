#include "structs.h"
#include "graph.h"

/*
	Checks if two people can be romantically attracted to eachother
*/
bool validRomance(CharacterObject *a, CharacterObject *b){
	affectionTrait *aRom = a->romance;
	affectionTrait *bRom = b->romance;

	bool valid = false;

	//Check if person A can be romantically attracted to the gender of person B
	for(int i = 0; i < aRom->n; i++){
		if(strcmp(aRom->genders[i], "any") == 0
					 || strcmp(aRom->genders[i], b->gender) == 0){
			valid = true;
			break;
		}
	}

	if(!valid) return false;
	else valid = false;

	//Check if person B can be romantically attracted to the gender of person A
	for(int i = 0; i < bRom->n; i++){
		if(strcmp(bRom->genders[i], "any") == 0
					 || strcmp(bRom->genders[i], a->gender) == 0){
			valid = true;
			break;
		}
	}

	return valid;
}

/*
	Checks if two people can be sexualy attracted to one another
*/
bool validSex(CharacterObject *a, CharacterObject *b){
	affectionTrait *aRom = a->sexuality;
	affectionTrait *bRom = b->sexuality;

	bool valid = false;

	//Check if person A can be sexually attracted to the gender of person B
	for(int i = 0; i < aRom->n; i++){
		if(strcmp(aRom->genders[i], "any") == 0
					 || strcmp(aRom->genders[i], b->gender) == 0){
			valid = true;
			break;
		}
	}

	if(!valid) return false;
	else valid = false;

	//Check if person B can be sexually attracted to the gender of person A
	for(int i = 0; i < bRom->n; i++){
		if(strcmp(bRom->genders[i], "any") == 0
					 || strcmp(bRom->genders[i], a->gender) == 0){
			valid = true;
			break;
		}
	}

	return valid;
}

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
