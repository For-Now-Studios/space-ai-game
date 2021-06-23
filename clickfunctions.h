#pragma once

/*
	Test funtion and struct parameter.
*/
struct btnHelloParameter { const char* name; };
void btnHello(void*);

/*
	Test popup
*/
struct popPopUpPars {
	CurrentClick *cc;
	bool poppedUp;
	vector<GameObject *>* objects;
	Media* media;
};
struct closePopUpPars {
	popPopUpPars *pPUP;
	ClickReciept *cr;
};

struct roomPopupPars {
	vector<GameObjClick*> buttons;
	CurrentClick *cc;
	bool poppedUp;
	vector<GameObject *>* objects;
};

void testClosePopUp(void*);
void testPopPopUp(void*);
void testPopPopUp0(void*);