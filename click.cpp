#include<algorithm>

#include "structs.h"
#include "engine.h"
#include "globals.h"
#include "click.h"
#include "clickfunctions.h"

/*
	Adds a SINGLE IsClickable to a list of areas.
	NOTICE: It is only a single clickable object.
*/
void addClickableToAreas(initializer_list<ClickArea*> areas, IsClickable* ic) {
	for (ClickArea* area : areas) {
		if (SDL_HasIntersection(&area->area, &ic->area)) {
			area->clicks.push_back(ic);
		}
	}
}

/*
	Adds IsClickable:s to a list of areas.
	NOTICE: It is a list of clickables.
*/
void addClickablesToAreas(initializer_list<ClickArea*> areas, initializer_list<IsClickable*> clickables) {
	for (IsClickable* ic : clickables) {
		addClickableToAreas(areas, ic);
	}	
}

/*
	Adds a character and technically anything moving.
	Currently only used inside of the buildClickable areas.
	However, this version could probably add a character in the middle of game running or whenever,
	as long it is after we bult the clickable area.
	**BUT** This is not tested yet, so it is *theoretically* possible.
	Also I removed from the header so it can be accessed outside this area of the code,
	just to avoid confusion.
	But right now please don't use this function to add new characters.
	Instead, put it in the correct initializer_list in loadLevel.
*/
void addCharacter(CurrentClick *cc, IsClickable* character) {
	for (int i = 0; i < cc->Game.size(); i++) {
		if (SDL_HasIntersection(&cc->Game.at(i)->area, &character->area)) {
			cc->Game.at(i)->clicks.push_back(character);
			cc->numChars[i]++;
		}
	}
}

void buildClickAreas(CurrentClick *cc, initializer_list<IsClickable*> characters, initializer_list<Room*> rooms,
	initializer_list<IsClickable*> UIElems, initializer_list<IsClickable*> popupElems, initializer_list<IsClickable*> gameElems) {

	//## UI ##
	initializer_list<ClickArea*> UIAreas = {
		new ClickArea(SDL_Rect{ 0, 420, SCREEN_WIDTH, 60 })
	};
	addClickablesToAreas(UIAreas, UIElems);
	for (ClickArea* ca : UIAreas) {
		cc->UI.push_back(ca);
	}

	//## Popup ##
		//nothing in the start

	//## Game ##
		//The number of game parts in the game part of the heirchy of clickable things.
	const int numGameParts = 2;
	cc->numChars = new int[numGameParts];
	for (int i = 0; i < numGameParts; i++)
	{
		cc->numChars[i] = 0;
	}
		//The game part areas
	initializer_list<ClickArea*> gameAreas = {
		new ClickArea(SDL_Rect{ 0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT }),
		new ClickArea(SDL_Rect{ SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT })
	};
		//Add them rooms to rooms vector and the correct part of the game.
	for (Room* room : rooms) {
		cc->rooms.push_back(room);
		addClickableToAreas(gameAreas, (IsClickable*)room);
	}
		//Add the usual elements
	addClickablesToAreas(gameAreas, gameElems);
		//Add the areas
	for (ClickArea* ca : gameAreas) {
		cc->Game.push_back(ca);
	}
		//Add them characters to characters vector and game.
	for (IsClickable* character : characters) {
		cc->Characters.push_back(character);
		addCharacter(cc, character);
	}
}

void cleanClickAreas(CurrentClick *cc) {
	for (Room* r : cc->rooms) {
		if (((roomPopupPars*)r->data)->poppedUp) {
			closeRoomPopup(((roomPopupPars*)r->data)->close);
		}
	}
	for (ClickArea* ca : cc->UI) {
		delete ca;
	}
	for (ClickArea* ca : cc->Popup) {
		delete ca;
	}
	for (ClickArea* ca : cc->Game) {
		delete ca;
	}
	for (vector<GameObject*>* vec : cc->toRender)
	{
		for (GameObject* obj : *vec) {
			delete obj;
		}
	}
	delete cc->numChars;
}

ClickReciept* createPopup(const vector<IsClickable*>& clicks, const vector<GameObject*>& objs, vector<GameObject*>* currentObjects, CurrentClick *cc, const SDL_Rect& area) {
	ClickReciept* cr = new ClickReciept;
	cr->renderObjs = new vector<GameObject*>(objs);
	cc->toRender.push_back(cr->renderObjs);

	cr->ca = new ClickAreaPopup(area);
	cr->ca->cr = cr;
	for (IsClickable* cl : clicks) {
		cr->ca->clicks.push_back(cl);
	}
	cc->Popup.push_back(cr->ca);
	printf("Hello %s!\n", "Made a popup");

	cr->objects = currentObjects;
	cr->cc = cc;
	return cr;
}

void closePopup(ClickReciept* cr) {
	for (int i = 0; i < cr->cc->Popup.size(); i++)
	{
		if (cr->cc->Popup.at(i) == cr->ca) {
			delete cr->ca;
			cr->cc->Popup.erase(cr->cc->Popup.begin() + i);
		}
	}
	for (int i = 0; i < cr->cc->toRender.size(); i++)
	{
		if (cr->cc->toRender.at(i) == cr->renderObjs) {
			delete cr->renderObjs;
			cr->cc->toRender.erase(cr->cc->toRender.begin() + i);
		}
	}

	delete cr;
}

void updateClickAreas(CurrentClick *cc) {
	for (int i = 0; i < cc->Game.size(); i++) {
		for (int j = 0; j < cc->numChars[i]; j++) {
			cc->Game.at(i)->clicks.pop_back();
		}
		cc->numChars[i] = 0;
	}
	for (IsClickable* character : cc->Characters) {
		addCharacter(cc, character);
	}
}

IsClickable* checkArea(CurrentClick *cc, int x, int y, ClickArea* ca, Camera *cam) {
	for (vector<IsClickable*>::reverse_iterator jt = ca->clicks.rbegin(); jt != ca->clicks.rend(); ++jt) {
		IsClickable* ic = *jt;
		SDL_Rect rec = translateToCamera(cam, &ic->area);
		if (rec.x < x && x < rec.x + rec.w &&
			rec.y < y && y < rec.y + rec.h) {
			return ic;
		}
	}
	return nullptr;
}
IsClickable* checkArea(CurrentClick *cc, int x, int y, ClickArea* ca) {
	for (vector<IsClickable*>::reverse_iterator jt = ca->clicks.rbegin(); jt != ca->clicks.rend(); ++jt) {
		IsClickable* ic = *jt;
		if (ic->area.x < x && x < ic->area.x + ic->area.w &&
			ic->area.y < y && y < ic->area.y + ic->area.h) {
			return ic;
		}
	}
	return nullptr;
}
/*
	Focuses the specific ClickAreaPopup.
	It needs to search throught the other popups for the right one.
	But only thourgh the popups, so it only increases in time when there is A LOT of popups
	and you either close or press one that isn't already on top.
*/
void focusPopup(CurrentClick *cc, ClickAreaPopup* ca) {
	for (int i = 0; i < cc->Popup.size(); i++)
	{
		if (cc->Popup.at(i) == ca) {
			cc->Popup.erase(cc->Popup.begin() + i);
			break;
		}
	}
	cc->Popup.push_back(ca);
	for (int i = 0; i < cc->toRender.size(); i++)
	{
		if (cc->toRender.at(i) == ca->cr->renderObjs) {
			vector<GameObject*>* temp = cc->toRender.at(i);
			cc->toRender.at(i) = cc->toRender.back();
			cc->toRender.back() = temp;
			break;
		}
	}
}

void movePopup(CurrentClick *cc, ClickAreaPopup* ca, MouseStruct& mouse) {
	ca->area.x += mouse.relX;
	ca->area.y += mouse.relY;

	for (IsClickable* ic : ca->clicks)
	{
		ic->area.x += mouse.relX;
		ic->area.y += mouse.relY;
	}

	for (int i = 0; i < cc->toRender.size(); i++)
	{
		if (cc->toRender.at(i) == ca->cr->renderObjs) {
			vector<GameObject*> temp = *(cc->toRender.at(i));
			for (GameObject* obj : temp)
			{
				obj->x += mouse.relX;
				obj->y += mouse.relY;
			}
			break;
		}
	}
}

IsClickable* checkCord(CurrentClick *cc, MouseStruct& mouse, Camera* cam) {
	int x = mouse.x;
	int y = mouse.y;

	for (vector<ClickArea*>::reverse_iterator it = cc->UI.rbegin(); it != cc->UI.rend(); ++it) {
		ClickArea* ca = *it;
		if (ca->area.x < x && x < ca->area.x + ca->area.w &&
			ca->area.y < y && y < ca->area.y + ca->area.h) {
			return checkArea(cc, x, y, ca);
		}
	}


	for (vector<ClickAreaPopup*>::reverse_iterator it = cc->Popup.rbegin(); it != cc->Popup.rend(); ++it) {
		ClickAreaPopup* ca = *it;
		if (ca->area.x < x && x < ca->area.x + ca->area.w &&
			ca->area.y < y && y < ca->area.y + ca->area.h) {
			if (cc->Popup.back() != ca) {
				focusPopup(cc, ca);
			}
			if (mouse.buttons[0].isPressed)
			{
				if (cc->currentlySelected == ca)
				{
					movePopup(cc, ca, mouse);
				}
				cc->currentlySelected = ca;
			}
			return checkArea(cc, x, y, ca);
		}
	}

	for (vector<ClickArea*>::reverse_iterator it = cc->Game.rbegin(); it != cc->Game.rend(); ++it) {
		ClickArea* ca = *it;
		SDL_Rect rec = translateToCamera(cam, &ca->area);
		if (rec.x < x && x < rec.x + rec.w &&
			rec.y < y && y < rec.y + rec.h) {
			return checkArea(cc, x, y, ca, cam);
		}
	}

	return nullptr;
}
