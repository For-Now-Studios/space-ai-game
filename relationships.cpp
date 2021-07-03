#include<algorithm>
#include<random>
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

void fallout(vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine dre) {
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* otherChar : chars) {
		if (otherChar == currChar) continue;
		printf("%s had a fallout with %s, so they are on bad terms now\n", currChar->name, otherChar->name);
		if (relatonships.getEdgeValue(currChar, otherChar) == Dating) {
			printf("%s and %s have broken up\n", currChar->name, otherChar->name);
			currChar->dating = false;
			otherChar->dating = false;
		}
		relatonships.updateEdge(currChar, otherChar, On_Bad_Terms);
		relatonships.updateEdge(otherChar, currChar, On_Bad_Terms);
		currChar->stress = currChar->stress + 20 > -1 ? currChar->stress + 20 : 0;
		printf("%s's stress just increased, it is now %d\n", currChar->name, currChar->stress);
		otherChar->stress = otherChar->stress + 20 > -1 ? otherChar->stress + 20 : 0;
		printf("%s's stress just increased, it is now %d\n", otherChar->name, otherChar->stress);
		return;
	}
}

void cheating(vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine dre) {
	uniform_int_distribution<int> distribution(1, 100);
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* cobj : chars) {
		if (cobj == currChar) continue;
		if (relatonships.getEdgeValue(currChar, cobj) != Dating && validSex(currChar, cobj)) {
			printf("%s is cheating with %s, that is bad\n", currChar->name, cobj->name);
			int roll = distribution(dre);
			if (roll < 50) {
				currChar->stress = currChar->stress+20 > -1 ? currChar->stress + 20 : 0;
				printf("%s's stress just increased, it is now %d\n", currChar->name, currChar->stress);
			}
			roll = distribution(dre);
			if (roll < 50) {
				cobj->stress = cobj->stress + 20 > -1 ? cobj->stress + 20 : 0;
				printf("%s's stress just increased, it is now %d\n", cobj->name, cobj->stress);
			}
			return;
		}
	}
	printf("%s is trying to cheat, but can't find someone to cheat with\n", currChar->name);
}
/*
Two people that are on bad terms can start dating.
*/
void confession(vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine dre) {
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* cobj : chars) {
		if (cobj == currChar) continue;
		if (relatonships.getEdgeValue(currChar, cobj) != Dating && validRomance(currChar, cobj)) {
			printf("%s just confessed to %s, they are now dating\n", currChar->name, cobj->name);

			currChar->dating = true;
			cobj->dating = true;

			currChar->stress = currChar->stress - 20 > -1 ? currChar->stress - 20 : 0;
			printf("%s's stress just decreased, it is now %d\n", currChar->name, currChar->stress);
			cobj->stress = cobj->stress - 20 > -1 ? cobj->stress - 20 : 0;
			printf("%s's stress just decreased, it is now %d\n", cobj->name, cobj->stress);
			relatonships.updateEdge(currChar, cobj, Dating);
			relatonships.updateEdge(cobj, currChar, Dating);
			return;
		}
	}
	printf("%s is trying to confess to someone, but can't find someone who wants to date them\n", currChar->name);
}

void birthday(vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine dre) {
	printf("%s is throwing a birthday, everyones stress is lowered.\n", currChar->name);
	for (CharacterObject* character : characters) {
		character->stress = character->stress - 10 > -1 ? character->stress - 10 : 0;
		printf("%s's stress: %d\n", currChar->name, character->stress);
	}
}

void cuddles(vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine dre) {
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* cobj : chars) {
		if (cobj == currChar) continue;
		if (relatonships.getEdgeValue(currChar, cobj) == Dating) {
			printf("%s is cuddling with %s, it is hella great\n", currChar->name, cobj->name);

			currChar->stress = currChar->stress - 20 > -1 ? currChar->stress - 20 : 0;
			printf("%s's stress just decreased, it is now %d\n", currChar->name, currChar->stress);
			cobj->stress = cobj->stress - 20 > -1 ? cobj->stress - 20 : 0;
			printf("%s's stress just decreased, it is now %d\n", cobj->name, cobj->stress);
			return;
		}
	}
	printf("%s wants to cuddle, but can't find anyone.\n", currChar->name);
}

void support(vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine dre) {
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* otherChar : chars) {
		if (otherChar == currChar) continue;
		if (relatonships.getEdgeValue(currChar, otherChar) != Dating) {
			relatonships.updateEdge(currChar, otherChar, Friends);
			relatonships.updateEdge(otherChar, currChar, Friends);
		}

		printf("%s is being supprotive and supporting %s, so %s is less stressed and this makes them more positive to each other\n", currChar->name, otherChar->name, otherChar->name);
		otherChar->stress = otherChar->stress - 20 > -1 ? otherChar->stress - 20 : 0;
		printf("%s's stress just decreased, it is now %d\n", otherChar->name, otherChar->stress);
		return;
	}
}