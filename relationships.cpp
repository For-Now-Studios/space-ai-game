#include<algorithm>
#include<random>
#include "structs.h"
#include "graph.h"
#include"click.h"
#include"clickfunctions.h"
#include"engine.h"

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

/*
	### THE DIFFERENT RELATIONSHIP EVENTS ###
	Each function should have the same return value and the same parameters.
	* Characters, for all characters alive
	* currChar for the current character this event activated for
	* relationships, the relationship graph
	* dre, the random engine.
*/

GameObject* getRandomRoom(CurrentClick *cc, default_random_engine dre) {
	//Randomly order the rooms
	vector<Room*> rooms(cc->rooms);
	shuffle(rooms.begin(), rooms.end(), dre);

	//Select the first room that isn't private
	Room* chosen = nullptr;
	for (Room* r : rooms) {
		if (r->flag & PRIVATE) continue;
		chosen = r;
		break;
	}

	return (GameObject*)chosen;
}

bool test = false;
void fallout(CurrentClick *cc, vector<CharacterObject*>& characters,
			CharacterObject* currChar, Graph<CharacterObject*,
				Relation>& relatonships, default_random_engine dre) {

	//if (test) return;
	test = true;

	// Randomly order the characters
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);

	// Select the first available character to be the second half of the event
	for (CharacterObject* otherChar : chars) {
		//Skip self
		if (otherChar == currChar) continue;

		//Get a location for the event to go down
		GameObject* location = getRandomRoom(cc, dre); //whichRoom(&cc->rooms, currChar);
		if (location == nullptr) {
			whichRoom(&cc->rooms, currChar);
			printf("%s is trying to have a fallout with %s, but can't find a appropiate location\n", currChar->name, otherChar->name);
		}

		printf("Ordering %s and %s to have a fallout\n", currChar->name,
									otherChar->name);

		Task* currCharTask = new Task(location, nullptr,
		(void*)(new FalloutEffectPars{currChar, otherChar, relatonships}),
							1, 200, "FALLOUT", AGAINSTENEMY);

		Task* otherCharTask = new Task(location, nullptr, nullptr, 100, 300,
								"Nothing", AGAINSTENEMY);

		currChar->addTask(currCharTask);
		otherChar->addTask(otherCharTask);

		return;
	}
}

void cheating(CurrentClick *cc, vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine dre) {
	uniform_int_distribution<int> distribution(1, 100);
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* cobj : chars) {
		if (cobj == currChar) continue;
		if (relatonships.getEdgeValue(currChar, cobj) != Dating && validSex(currChar, cobj)) {
			printf("%s is cheating with %s, that is bad\n", currChar->name, cobj->name);
			int roll = distribution(dre);
			if (roll < 50) {
				//changeStress(20, currChar);
				printf("%s's stress just increased, it is now %d\n", currChar->name, currChar->stress);
			}
			roll = distribution(dre);
			if (roll < 50) {
				//changeStress(20, cobj);
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
void confession(CurrentClick *cc, vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine dre) {
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* cobj : chars) {
		if (cobj == currChar) continue;
		if (relatonships.getEdgeValue(currChar, cobj) != Dating && validRomance(currChar, cobj)) {
			printf("%s just confessed to %s, they are now dating\n", currChar->name, cobj->name);

			currChar->dating = true;
			cobj->dating = true;

			//changeStress(-20, currChar);
			printf("%s's stress just decreased, it is now %d\n", currChar->name, currChar->stress);
			//changeStress(-20, cobj);
			printf("%s's stress just decreased, it is now %d\n", cobj->name, cobj->stress);
			relatonships.updateEdge(currChar, cobj, Dating);
			relatonships.updateEdge(cobj, currChar, Dating);
			return;
		}
	}
	printf("%s is trying to confess to someone, but can't find someone who wants to date them\n", currChar->name);
}

void birthday(CurrentClick *cc, vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine dre) {
	printf("%s is throwing a birthday, everyones stress is lowered.\n", currChar->name);
	for (CharacterObject* character : characters) {
		//changeStress(-10, character);
		printf("%s's stress: %d\n", character->name, character->stress);
	}
}

void cuddles(CurrentClick *cc, vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine dre) {
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* cobj : chars) {
		if (cobj == currChar) continue;
		if (relatonships.getEdgeValue(currChar, cobj) == Dating) {
			printf("%s is cuddling with %s, it is hella great\n", currChar->name, cobj->name);

			//changeStress(-20, currChar);
			printf("%s's stress just decreased, it is now %d\n", currChar->name, currChar->stress);
			//changeStress(-20, cobj);
			printf("%s's stress just decreased, it is now %d\n", cobj->name, cobj->stress);
			return;
		}
	}
	printf("%s wants to cuddle, but can't find anyone.\n", currChar->name);
}

void support(CurrentClick *cc, vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine dre) {
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* otherChar : chars) {
		if (otherChar == currChar) continue;
		if (relatonships.getEdgeValue(currChar, otherChar) != Dating) {
			relatonships.updateEdge(currChar, otherChar, Friends);
			relatonships.updateEdge(otherChar, currChar, Friends);
		}

		printf("%s is being supprotive and supporting %s, so %s is less stressed and this makes them more positive to each other\n", currChar->name, otherChar->name, otherChar->name);
		//changeStress(-20, otherChar);
		printf("%s's stress just decreased, it is now %d\n", otherChar->name, otherChar->stress);
		return;
	}
}
