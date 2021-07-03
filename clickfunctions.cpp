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

void roomPopup(void *cntxt) {
	roomPopupPars *pars = (roomPopupPars*)cntxt;
	if (!pars->poppedUp) {
		vector<GameObject*> objs;
		vector<IsClickable*> clicks;
		int x = 320;
		int y = 200;
		GameObjClick *p = new GameObjClick(x, y, pars->media->images.at(0), btnHello,
			(void*)(new btnHelloParameter{ "p!" }));

		closePopUpPars* cPUP = new closePopUpPars;
		cPUP->pPUP = pars;
		GameObjClick *px = new GameObjClick(x+60, y, pars->media->images.at(1), closeRoomPopup,
			(void*)(cPUP));
		objs.push_back(p);
		clicks.push_back(p);
		
		int yLength = 0;
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