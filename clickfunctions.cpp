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
	for (int i = pars->cr->objs[0]; i < pars->cr->objs[1] + 1; i++)
	{
		delete pars->cr->objects->at(i);
	}
	closePopup(pars->cr);
	printf("Hello %s!\n", "Closed a popup");
	pars->pPUP->poppedUp = false;
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