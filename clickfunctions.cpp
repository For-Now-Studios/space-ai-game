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
	for (GameObject* obj : *pars->cr->renderObjs) {
		delete obj;
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
		GameObjClick *px = new GameObjClick(x+60, y, pars->media->images.at(1), testClosePopUp,
			(void*)(cPUP));
		objs.push_back(p);
		objs.push_back(px);
		clicks.push_back(p);
		clicks.push_back(px);
		
		int yLength = 0;
		for (GameObjClick* goc : pars->buttons) {
			goc->x = x;
			goc->y = y + 60*yLength;
			goc->area.x = x;
			goc->area.y = goc->y;
			clicks.push_back(goc);
			objs.push_back(goc);
			yLength++;
		}
		yLength = yLength < 2 ? 2 : yLength;
		printf("yLength: %d\n", yLength);

		cPUP->cr = createPopup(clicks, objs, pars->objects, pars->cc, SDL_Rect{ x, y, 120, 60 * yLength });
		printf("Called roomPopup\n");
		pars->poppedUp = true;
	}
}