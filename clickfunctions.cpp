#include "structs.h"
#include "engine.h"
#include "globals.h"
#include "click.h"
#include "clickfunctions.h"

void btnHello(void *cntxt)
{
	btnHelloParameter *parameters = (btnHelloParameter*)cntxt;
	printf("Hello %s!\n", parameters->name);
}
void testClosePopUp(void *cntxt) {
	closePopUpPars *pars = (closePopUpPars*)cntxt;

	pars->pPUP->poppedUp = false;
	ClickReciept *cr = pars->cr;

	//Delete necessary GameObejcts and IsClickable objects.
	//NOTE: This deletes the button which we were called from
	int i = 0;
	for (GameObject* obj : *cr->renderObjs) {
		delete obj;
		i++;
	}
	closePopup(cr);
	printf("Hello %s!\n", "Closed a popup");
}
void testPopPopUp(void *cntxt) {
	popPopUpPars *pars = (popPopUpPars*)cntxt;
	if (!pars->poppedUp) {
		vector<GameObject*> objs;
		vector<IsClickable*> clicks;

		GameObjClick *p = new GameObjClick(200, 200, pars->media->images.at(0), btnHello,
			(void*)(new btnHelloParameter{ "p!" }));

		GameObjClick *p0 = new GameObjClick(200, 200, pars->media->images.at(1), btnHello,
			(void*)(new btnHelloParameter{ "p0!" }));

		closePopUpPars* cPUP = new closePopUpPars;
		cPUP->pPUP = pars;
		GameObjClick *px = new GameObjClick(260, 200, pars->media->images.at(1), testClosePopUp,
			(void*)(cPUP));

		objs.push_back(p);
		objs.push_back(p0);
		objs.push_back(px);
		clicks.push_back(p);
		clicks.push_back(p0);
		clicks.push_back(px);

		cPUP->cr = createPopup(clicks, objs, pars->objects, pars->cc, SDL_Rect{ 200, 200, 120, 120 });
		printf("Called testPopPopUp\n");
		pars->poppedUp = true;
	}
}
void testPopPopUp0(void *cntxt) {
	popPopUpPars *pars = (popPopUpPars*)cntxt;
	if (!pars->poppedUp) {
		vector<GameObject*> objs;
		vector<IsClickable*> clicks;

		GameObjClick *p = new GameObjClick(260, 260, pars->media->images.at(0), btnHello,
			(void*)(new btnHelloParameter{ "p!" }));

		GameObjClick *p0 = new GameObjClick(260, 260, pars->media->images.at(1), btnHello,
			(void*)(new btnHelloParameter{ "p0!" }));

		closePopUpPars* cPUP = new closePopUpPars;
		cPUP->pPUP = pars;
		GameObjClick *px = new GameObjClick(320, 260, pars->media->images.at(1), testClosePopUp,
			(void*)(cPUP));

		objs.push_back(p);
		objs.push_back(p0);
		objs.push_back(px);
		clicks.push_back(p);
		clicks.push_back(p0);
		clicks.push_back(px);

		cPUP->cr = createPopup(clicks, objs, pars->objects, pars->cc, SDL_Rect{260, 260, 120, 120 });
		printf("Called testPopPopUp0\n");
		pars->poppedUp = true;
	}
}

//Specifically needed so it deletes necessary buttons
void closeCharRoomPopup(void *cntxt) {
	closePopUpPars *pars = (closePopUpPars*)cntxt;

	pars->pPUP->poppedUp = false;
	ClickReciept *cr = pars->cr;

	//Delete necessary GameObejcts and IsClickable objects.
	//NOTE: This deletes the button which we were called from
	for (GameObject* obj : *cr->renderObjs) {
		delete obj;
	}
	closePopup(cr);
	printf("Hello %s!\n", "Closed a popup");
}

void closeRoomPopup(void *cntxt) {
	closePopUpPars *pars = (closePopUpPars*)cntxt;

	pars->pPUP->poppedUp = false;
	ClickReciept *cr = pars->cr;

	//Delete necessary GameObejcts and IsClickable objects.
	//NOTE: This deletes the button which we were called from
	delete cr->renderObjs->at(0);
	delete cr->renderObjs->back();
	/*int i = 0;
	for (GameObject* obj : *cr->renderObjs) {
		printf("num: %d, p: %p\n", i, obj);
		delete obj;
		i++;
	}*/
	closePopup(cr);
	printf("Hello %s!\n", "Closed a popup");
}

struct SendCharPars {
	CharacterObject* character;
	Room *room;
};
//Adds task for character to go to specific place.
void sendChar(void *cntxt) {
	SendCharPars *pars = (SendCharPars*)cntxt;
	pars->character->addTask(new Task(pars->room, nullptr, nullptr, 16, 64, "GOTO", AIASSIGNED));
	printf("Ordering %s to go to (%d, %d)\n", pars->character->name, pars->room->x, pars->room->y);
}
//Creates a popup to send characters
void sendCharPopup(void *cntxt) {
	roomPopupPars *pars = (roomPopupPars*)cntxt;
	if (!pars->poppedUp) {
		vector<GameObject*> objs;
		vector<IsClickable*> clicks;
		int x = 320;
		int y = 200;

		GameObjClick *p = new GameObjClick(x, y, pars->media->images.at(0), btnHello,
			(void*)(new btnHelloParameter{ "p!" }));

		objs.push_back(p);
		clicks.push_back(p);

		int yLength = 0;
		for (CharacterObject* cobj : *pars->characters) {
			GameObjClick* goc = new GameObjClick(x, y, pars->media->images.at(1), sendChar,
				(void*)(new SendCharPars{cobj, pars->room}));
			goc->x = x;
			goc->y = y + 60 * yLength;
			goc->area.x = x;
			goc->area.y = goc->y;
			clicks.push_back(goc);
			objs.push_back(goc);
			yLength++;
		}
		yLength = yLength < 2 ? 2 : yLength;

		//Close button need to be pushed last, so it is deleted last.
		closePopUpPars* cPUP = new closePopUpPars;
		cPUP->pPUP = pars;
		GameObjClick *px = new GameObjClick(x + 60, y, pars->media->images.at(1), closeCharRoomPopup,
			(void*)(cPUP));
		clicks.push_back(px);
		objs.push_back(px);

		cPUP->cr = createPopup(clicks, objs, pars->objects, pars->cc, SDL_Rect{ x, y, 120, 60 * yLength });
		printf("Called sendCharPopup\n");
		pars->poppedUp = true;
		pars->close = cPUP;
	}
}
void roomPopup(void *cntxt) {
	roomPopupPars *pars = (roomPopupPars*)cntxt;
	if (!pars->poppedUp) {
		vector<GameObject*> objs;
		vector<IsClickable*> clicks;
		int yLength = 0;
		int x = 320;
		int y = 200;

		GameObjClick *p = new GameObjClick(x, y, pars->media->images.at(0), btnHello,
			(void*)(new btnHelloParameter{ "p!" }));

		closePopUpPars* cPUP = new closePopUpPars;
		cPUP->pPUP = pars;
		GameObjClick *px = new GameObjClick(x+60, y, pars->media->images.at(1), closeRoomPopup,
			(void*)(cPUP));

		roomPopupPars* rPup = new roomPopupPars(popPopUpPars{ pars->cc, false, pars->objects, pars->media }, pars->characters);
		rPup->room = pars->room;
		GameObjClick* sendBtn = new GameObjClick(x, y, pars->media->images.at(1), sendCharPopup,
			(void*)(rPup));
		yLength++;

		objs.push_back(p);
		clicks.push_back(p);

		objs.push_back(sendBtn);
		clicks.push_back(sendBtn);
		
		for (GameObjClick* goc : pars->room->buttons) {
			goc->x = x;
			goc->y = y + 60*yLength;
			goc->area.x = x;
			goc->area.y = goc->y;

			//GameObjClick* copy = new GameObjClick{ *goc };
			//printf("And this2 %p\n", copy);
			clicks.push_back(goc);
			objs.push_back(goc);
			yLength++;
		}
		yLength = yLength < 2 ? 2 : yLength;

		//Close button need to be pushed last, so it is deleted last.
		clicks.push_back(px);
		objs.push_back(px);
		cPUP->cr = createPopup(clicks, objs, pars->objects, pars->cc, SDL_Rect{ x, y, 120, 60 * yLength });
		printf("Called roomPopup\n");
		pars->poppedUp = true;
		pars->close = cPUP;
	}
}
void doorClick(void *cntxt) {
	DoorClickPars *pars = (DoorClickPars*)cntxt;
	if (pars->mouse->buttons[0].isReleased) {
		pars->door->IsOpen = !pars->door->IsOpen;
		if (pars->door->IsOpen) pars->door->IsLocked = false;
	} else if(pars->mouse->buttons[2].isReleased) {
		pars->door->IsLocked = !pars->door->IsLocked;
	}

	if (pars->door->IsLocked) {
		pars->door->IsOpen = false;
		pars->door->image = pars->locked;
	} else if (pars->door->IsOpen) {
		pars->door->image = pars->open;
	} else {
		pars->door->image = pars->closed;
	}
}

void stressCharacter(void *cntxt) {
	StressCharacterPars *pars = (StressCharacterPars*)cntxt;
	pars->currChar->stress += 20;
	printf("Stressing %s, their stress is now at %d\n",
		pars->currChar->name, pars->currChar->stress);
}

/*
	###########################
	### Relationship events ###
	###########################
*/

/*
Changes the stress level
Just checks so it doesn't go below zero
and if a character is paranoid it increases.
*/
void changeStress(int amnt, CharacterObject* cobj) {
	int newStress = cobj->stress + amnt +
			((cobj->traitFlags & PARANOID) != 0) * abs(amnt >> 1);

	cobj->stress = newStress > -1 ? newStress : 0;
}

void falloutEffect(void *cntxt) {
	FalloutEffectPars *pars = (FalloutEffectPars*)cntxt;

	printf("%s had a fallout with %s, so they are on bad terms now\n",
					pars->currChar->name, pars->otherChar->name);

	if(pars->relatonships.getEdgeValue(pars->currChar, pars->otherChar) == Dating){
		printf("%s and %s have broken up\n", pars->currChar->name,
								pars->otherChar->name);
		pars->currChar->dating = false;
		pars->otherChar->dating = false;
	}

	pars->relatonships.updateEdge(pars->currChar, pars->otherChar, On_Bad_Terms);
	pars->relatonships.updateEdge(pars->otherChar, pars->currChar, On_Bad_Terms);

	changeStress(20, pars->currChar);
	printf("%s's stress just increased, it is now %d\n", pars->currChar->name,
								pars->currChar->stress);

	changeStress(20, pars->otherChar);
	printf("%s's stress just increased, it is now %d\n", pars->otherChar->name,
								pars->otherChar->stress);
}

void cheatingEffect(void *cntxt) {
	CheatingEffectPars *pars = (CheatingEffectPars*)cntxt;

	uniform_int_distribution<int> distribution(1, 100);
	printf("%s is cheating with %s, that is bad\n", pars->currChar->name,
								pars->cobj->name);

	int roll = distribution(pars->dre);
	if (roll < 50) {
		changeStress(20, pars->currChar);
		printf("%s's stress just increased, it is now %d\n",
					pars->currChar->name, pars->currChar->stress);
	}

	roll = distribution(pars->dre);
	if (roll < 50) {
		changeStress(20, pars->cobj);
		printf("%s's stress just increased, it is now %d\n", pars->cobj->name,
								pars->cobj->stress);
	}
}

void confessionEffect(void *cntxt) {
	ConfessionEffectPars *pars = (ConfessionEffectPars*)cntxt;
	printf("%s just confessed to %s, they are now dating\n", pars->currChar->name,
								pars->cobj->name);

	pars->currChar->dating = true;
	pars->cobj->dating = true;

	changeStress(-20, pars->currChar);
	printf("%s's stress just decreased, it is now %d\n", pars->currChar->name,
								pars->currChar->stress);

	changeStress(-20, pars->cobj);
	printf("%s's stress just decreased, it is now %d\n", pars->cobj->name,
								pars->cobj->stress);

	pars->relatonships.updateEdge(pars->currChar, pars->cobj, Dating);
	pars->relatonships.updateEdge(pars->cobj, pars->currChar, Dating);
}

/*
	TODO: If no one shows up, the birthday child gets more stressed.
*/
void birthdayEffect(void *cntxt) {
	BirthdayEffectPars *pars = (BirthdayEffectPars*)cntxt;
	printf("%s is throwing a birthday,\n everyone there has their\
 stress is lowered.\n", pars->currChar->name);

	GameObject* location = whichRoom(&pars->cc->rooms, pars->currChar);
	int i = 0;
	for (CharacterObject* character : pars->characters) {
		if (character == pars->currChar) {
			changeStress(-10, character);
			printf("%s's stress: %d\n", character->name, character->stress);
		}
		else if (location == whichRoom(&pars->cc->rooms, character)) {
			changeStress(-10, character);
			printf("%s's stress: %d\n", character->name, character->stress);
			i++;
		}
		else {
			pars->tasksToDelete[i]->flag =
					pars->tasksToDelete[i]->flag & ~WAITINGFOR;
			pars->tasksToDelete[i]->waitingFor = nullptr;
			i++;
		}
	}
}

void cuddleEffect(void *cntxt) {
	CuddleEffectPars *pars = (CuddleEffectPars*)cntxt;
	printf("%s is cuddling with %s, it is hella great\n",
						pars->currChar->name, pars->cobj->name);

	changeStress(-20, pars->currChar);
	printf("%s's stress just decreased, it is now %d\n",
					pars->currChar->name, pars->currChar->stress);

	changeStress(-20, pars->cobj);
	printf("%s's stress just decreased, it is now %d\n",
						pars->cobj->name, pars->cobj->stress);
}

void supportEffect(void *cntxt) {
	SupportEffectPars *pars = (SupportEffectPars*)cntxt;

	if (pars->relatonships.getEdgeValue(pars->currChar, pars->otherChar) != Dating) {
		pars->relatonships.updateEdge(pars->currChar, pars->otherChar, Friends);
		pars->relatonships.updateEdge(pars->otherChar, pars->currChar, Friends);
	}

	printf("%s is being supprotive and supporting %s,\nso %s is less stressed and\
 this makes them more positive to each other\n",

	pars->currChar->name, pars->otherChar->name, pars->otherChar->name);

	changeStress(-20, pars->otherChar);
	printf("%s's stress just decreased, it is now %d\n",
					pars->otherChar->name, pars->otherChar->stress);
}
