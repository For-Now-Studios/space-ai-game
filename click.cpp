#include<algorithm>

#include "structs.h"
#include "engine.h"
#include "globals.h"
#include "click.h"

void buildClickAreas(CurrentClick *cc, vector<IsClickable*> clickable) {
	cc->Characters.push_back(clickable.at(1));
	cc->Characters.push_back(clickable.at(4));
	cc->numChars[0] = 1;
	cc->numChars[1] = 1;

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
	S0->clicks.push_back(clickable.at(3));
	S0->clicks.push_back(clickable.at(1));

	ClickArea *S1 = new ClickArea;
	S1->area = SDL_Rect{ SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT };
	S1->clicks.push_back(clickable.at(4));

	cc->Game.push_back(S0);
	cc->Game.push_back(S1);
}

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

void addCharacters(CurrentClick *cc, IsClickable* character) {
	for (int i = 0; i < cc->Game.size(); i++) {
		if (SDL_HasIntersection(&cc->Game.at(i)->area, &character->area)) {
			cc->Game.at(i)->clicks.push_back(character);
			cc->numChars[i]++;
		}
	}
}

ClickReciept* createPopup(const vector<IsClickable*>& clicks, const vector<GameObject*>& objs, vector<GameObject*>* currentObjects, CurrentClick *cc, const SDL_Rect& area) {
	ClickReciept* cr = new ClickReciept;
	cr->renderObjs = new vector<GameObject*>(objs);
	cc->toRender.push_back(cr->renderObjs);

	cr->ca = new ClickAreaPopup;
	cr->ca->area = area;
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
			delete (*cr->cc->Popup.erase(cr->cc->Popup.begin() + i));
		}
	}
	for (int i = 0; i < cr->cc->toRender.size(); i++)
	{
		if (cr->cc->toRender.at(i) == cr->renderObjs) {
			delete (*cr->cc->toRender.erase(cr->cc->toRender.begin() + i));
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
		}
	}
	cc->Popup.push_back(ca);
	for (int i = 0; i < cc->toRender.size(); i++)
	{
		if (cc->toRender.at(i) == ca->cr->renderObjs) {
			vector<GameObject*>* temp = cc->toRender.at(i);
			cc->toRender.at(i) = cc->toRender.back();
			cc->toRender.back() = temp;
		}
	}
}

IsClickable* checkCord(CurrentClick *cc, int x, int y, Camera* cam) {
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
