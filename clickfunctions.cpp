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

void closePopUp(void *cntxt) {
	closePopUpPars *pars = (closePopUpPars*)cntxt;
	closePopup(pars->cc, pars->index, pars->objects, pars->first, pars->last);
	printf("Hello %s!\n", "Closed a popup");
	pars->pPUP->poppedUp = false;
}

void popPopUp(void *cntxt) {
	popPopUpPars *pars = (popPopUpPars*)cntxt;
	if (!pars->poppedUp) {
		GameObjClick *p = new GameObjClick(200, 200, pars->media->images.at(0), btnHello,
			(void*)(new btnHelloParameter{ "p!" }));

		GameObjClick *p0 = new GameObjClick(200, 200, pars->media->images.at(1), btnHello,
			(void*)(new btnHelloParameter{ "p0!" }));

		closePopUpPars* cPUP = new closePopUpPars{ 0,0,0,pars->cc,pars };
		GameObjClick *px = new GameObjClick(260, 200, pars->media->images.at(1), closePopUp,
			(void*)(cPUP));

		pars->objects->push_back(p);
		cPUP->first = pars->objects->size() - 1;
		pars->objects->push_back(p0);
		pars->objects->push_back(px);
		cPUP->last = pars->objects->size() - 1;

		cPUP->objects = pars->objects;

		ClickArea* P = new ClickArea;
		P->area = SDL_Rect{ 200, 200, 120, 120 };
		P->clicks.push_back(p);
		P->clicks.push_back(p0);
		P->clicks.push_back(px);
		cPUP->index = addPopup(pars->cc, P);
		printf("Hello %s!\n", "Made a popup");
		pars->poppedUp = true;
	}
}