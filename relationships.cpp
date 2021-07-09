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

void fallout(CurrentClick *cc, vector<CharacterObject*>& characters,
			CharacterObject* currChar, Graph<CharacterObject*,
			Relation>& relatonships, default_random_engine dre,
							vector<Image *> *images) {
	// Randomly order the characters
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);

	// Select the first available character to be the second half of the event
	for (CharacterObject* otherChar : chars) {
		//Skip self
		if (otherChar == currChar) continue;

		//Get a location for the event to go down
		GameObject* location = getRandomRoom(cc, dre);
		if (location == nullptr) {
			printf("%s is trying to have a fallout with %s, but can't find a appropiate location\n", currChar->name, otherChar->name);
			return;
		}
		printf("Ordering %s and %s to have a fallout\n", currChar->name,
									otherChar->name);
		Task* currCharTask = new Task(location, falloutEffect,
		(void*)(new FalloutEffectPars{currChar, otherChar, relatonships}),
			0, 0, "FALLOUT", AGAINSTENEMY | WAITINGFOR, otherChar,
									images->at(35));
		Task* otherCharTask = new Task(location, nullptr, nullptr, 0, 0,
					"Nothing", AGAINSTENEMY|WAITINGFOR, currChar,
									images->at(35));
		currChar->addTask(currCharTask);
		otherChar->addTask(otherCharTask);

		return;
	}
}

void cheating(CurrentClick *cc, vector<CharacterObject*>& characters,
				CharacterObject* currChar, Graph<CharacterObject*,
				Relation>& relatonships, default_random_engine dre,
							vector<Image *> *images) {
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* cobj : chars) {
		if (cobj == currChar) continue;
		if (relatonships.getEdgeValue(currChar, cobj) != Dating && validSex(currChar, cobj)) {
			//Get a location for the event to go down
			GameObject* location = getRandomRoom(cc, dre);

			if (location == nullptr) {
				printf("%s is trying to cheat with %s, but can't find a appropiate location for coopulation\n", currChar->name, cobj->name);
				return;
			}
			printf("Ordering %s and %s to have an affair\n", currChar->name,
				cobj->name);
			Task* currCharTask = new Task(location, cheatingEffect,
					(void*)(new CheatingEffectPars{ currChar, cobj,
						relatonships, dre }), 100, 0, "CHEATING",
							FORLOVE | WAITINGFOR, cobj,
									images->at(36));
			Task* otherCharTask = new Task(location, nullptr, nullptr, 100,
					0, "Nothing", FORLOVE | WAITINGFOR, currChar,
									images->at(36));
			currChar->addTask(currCharTask);
			cobj->addTask(otherCharTask);
			return;
		}
	}
	printf("%s is trying to cheat, but can't find someone to cheat with\n", currChar->name);
}
/*
Two people that are on bad terms can start dating.
*/
void confession(CurrentClick *cc, vector<CharacterObject*>& characters,
				CharacterObject* currChar, Graph<CharacterObject*,
				Relation>& relatonships, default_random_engine dre,
							vector<Image *> *images) {
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* cobj : chars) {
		if (cobj == currChar) continue;
		if (relatonships.getEdgeValue(currChar, cobj) != Dating && validRomance(currChar, cobj)) {
			//Get a location for the event to go down
			GameObject* location = getRandomRoom(cc, dre);

			if (location == nullptr) {
				printf("%s is trying to confess with %s, but can't find an appropiate location for coopulation\n", currChar->name, cobj->name);
				return;
			}
			printf("Ordering %s to date %s\n", currChar->name, cobj->name);

			Task* currCharTask = new Task(location, confessionEffect,
				(void*)(new ConfessionEffectPars{ currChar, cobj,
									relatonships }),
				100, 0, "CONFESSION", FORLOVE | WAITINGFOR, cobj,
									images->at(37));
			Task* otherCharTask = new Task(location, nullptr, nullptr, 100,
					0, "Nothing", FORLOVE | WAITINGFOR, currChar,
									images->at(37));
			currChar->addTask(currCharTask);
			cobj->addTask(otherCharTask);
			return;
		}
	}
	printf("%s is trying to confess to someone, but can't find someone who wants to date them\n", currChar->name);
}

void birthday(CurrentClick *cc, vector<CharacterObject*>& characters,
				CharacterObject* currChar, Graph<CharacterObject*,
				Relation>& relatonships, default_random_engine dre,
							vector<Image *> *images) {
	//Get a location for the event to go down
	GameObject* location = getRandomRoom(cc, dre);
	Task** tasksToDelete = new Task*[characters.size()-1];
	int i = 0;
	if (location == nullptr) {
		printf("%s is trying to hold their birthday, but can't find any location\n", currChar->name);
		return;
	}
	printf("Ordering %s to celebrate their birthday\n", currChar->name);
	for (CharacterObject* cobj : characters) {
		if (cobj == currChar) {
			Task* currCharTask = new Task(location, birthdayEffect,
				(void*)(new BirthdayEffectPars{ currChar, characters,
						relatonships, tasksToDelete, cc }),
				10000, 0, "BIRTHDAYPARTY", FORLOVE, images->at(38));
			currChar->addTask(currCharTask);
		} else {
			Task* otherCharTask = new Task(location, nullptr, nullptr, 100,
					0, "Nothing", FORLOVE | WAITINGFOR, currChar,
									images->at(38));
			cobj->addTask(otherCharTask);
			tasksToDelete[i] = otherCharTask;
			i++;
		}
	}
}

void cuddles(CurrentClick *cc, vector<CharacterObject*>& characters,
				CharacterObject* currChar, Graph<CharacterObject*,
				Relation>& relatonships, default_random_engine dre,
							vector<Image *> *images) {
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* cobj : chars) {
		if (cobj == currChar) continue;
		if (relatonships.getEdgeValue(currChar, cobj) == Dating) {
			//Get a location for the event to go down
			GameObject* location = getRandomRoom(cc, dre);

			if (location == nullptr) {
				printf("%s is trying to cuddle with %s, but can't find a good cuddle spot\n", currChar->name, cobj->name);
				return;
			}
			printf("Ordering %s to cuddle with %s\n", currChar->name,
				cobj->name);
			Task* currCharTask = new Task(location, cuddleEffect,
				(void*)(new CuddleEffectPars{ currChar, cobj, relatonships }),
				100, 0, "CUDDLE", FORLOVE | WAITINGFOR, cobj);
			Task* otherCharTask = new Task(location, nullptr, nullptr, 100, 0,
				"Nothing", FORLOVE | WAITINGFOR, currChar);
			currChar->addTask(currCharTask);
			cobj->addTask(otherCharTask);
		}
	}
	printf("%s wants to cuddle, but can't find anyone.\n", currChar->name);
}

void support(CurrentClick *cc, vector<CharacterObject*>& characters,
				CharacterObject* currChar, Graph<CharacterObject*,
				Relation>& relatonships, default_random_engine dre,
							vector<Image *> *images) {
	vector<CharacterObject*> chars(characters);
	shuffle(chars.begin(), chars.end(), dre);
	for (CharacterObject* cobj : chars) {
		if (cobj == currChar) continue;
		//Get a location for the event to go down
		GameObject* location = getRandomRoom(cc, dre);

		if (location == nullptr) {
			printf("%s is trying to support %s, but can't find a good quiet spot\n", currChar->name, cobj->name);
			return;
		}
		printf("Ordering %s to support %s\n", currChar->name,
			cobj->name);
		Task* currCharTask = new Task(location, supportEffect,
			(void*)(new SupportEffectPars{ currChar, cobj, relatonships }),
			100, 0, "SUPPORT", FORFRIENDS | WAITINGFOR, cobj);
		Task* otherCharTask = new Task(location, nullptr, nullptr, 100, 0,
			"Nothing", FORFRIENDS | WAITINGFOR, currChar);
		currChar->addTask(currCharTask);
		cobj->addTask(otherCharTask);
		return;
	}
}
