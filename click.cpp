#include "structs.h"
#include "engine.h"
#include "globals.h"
#include "click.h"

/*
	Builds the clickable areas.
*/
void buildClickAreas(CurrentClick *cc, vector<IsClickable*> clickable) {
	cc->Characters.push_back(clickable.at(1));
	cc->numChars[0] = 1;
	cc->numChars[1] = 0;

	//UI
	ClickArea *UI = new ClickArea;
	UI->area = SDL_Rect{ 0, 420, SCREEN_WIDTH, 60 };
	UI->clicks.push_back(clickable.at(2));

	cc->UI.push_back(UI);

	//Popup
	//nothign in the start

	//Game
	ClickArea *S0 = new ClickArea;
	S0->area = SDL_Rect{ 0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT };
	S0->clicks.push_back(clickable.at(0));
	S0->clicks.push_back(clickable.at(1));

	ClickArea *S1 = new ClickArea;
	S1->area = SDL_Rect{ SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT };

	cc->Game.push_back(S0);
	cc->Game.push_back(S1);
}

/*
	Cleans the memory of the clickable areas.
*/
void cleanClickAreas(CurrentClick *cc) {
	for (ClickArea* ca : cc->UI) {
		delete ca;
	}
	for (ClickArea* ca : cc->Popup) {
		delete ca;
	}
	for (ClickArea* ca : cc->Game) {
		delete ca;
	}
}

/*
	Adds a popup to the game.
	Returns the index.
*/
int addPopup(CurrentClick *cc, ClickArea* area) {
	cc->Popup.push_back(area);
	return cc->Popup.size() - 1;
}

/*
	Closes specified popup and deletes the objects from the game.
*/
void closePopup(CurrentClick *cc, int index, vector<GameObject *>* objects, int first, int last) {
	delete (*cc->Popup.erase(cc->Popup.begin() + index));
	vector<GameObject *>::iterator buttons = objects->erase(objects->begin() + first, objects->begin() + last + 1);
	for (int i = first; i < last + 1; i++)
	{
		delete (*buttons);
		buttons++;
	}
}

/*
	Adds a character and technically anything moving.
*/
void addCharacters(CurrentClick *cc, IsClickable* character) {
	for (int i = 0; i < cc->Game.size(); i++) {
		if (SDL_HasIntersection(&cc->Game.at(i)->area, &character->area)) {
			cc->Game.at(i)->clicks.push_back(character);
			cc->numChars[i]++;
		}
	}
}

/*
	Updates all moving clickable things.
*/
void updateClickAreas(CurrentClick *cc) {
	for (int i = 0; i < cc->Game.size(); i++) {
		for (int j = 0; j < cc->numChars[i]; j++) {
			cc->Game.at(i)->clicks.pop_back();
		}
		cc->numChars[i] = 0;
	}
	for (IsClickable* character : cc->Characters) {
		addCharacters(cc, character);
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
	Checks specified coordinates on current clickable area.
	It checks for one item only and check in this order
	1: UI
	2: Popup
	3: Game
	TODO: Have focused popups be on top and test it
*/
IsClickable* checkCord(CurrentClick *cc, int x, int y, Camera* cam) {
	for (vector<ClickArea*>::reverse_iterator it = cc->UI.rbegin(); it != cc->UI.rend(); ++it) {
		ClickArea* ca = *it;
		if (ca->area.x < x && x < ca->area.x + ca->area.w &&
			ca->area.y < y && y < ca->area.y + ca->area.h) {
			return checkArea(cc, x, y, ca);
		}
	}

	for (vector<ClickArea*>::reverse_iterator it = cc->Popup.rbegin(); it != cc->Popup.rend(); ++it) {
		ClickArea* ca = *it;
		if (ca->area.x < x && x < ca->area.x + ca->area.w &&
			ca->area.y < y && y < ca->area.y + ca->area.h) {
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