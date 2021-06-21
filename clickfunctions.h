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
	int index;
	int first;
	int last;
	CurrentClick *cc;
	popPopUpPars *pPUP;
	vector<GameObject *>* objects;
};
void closePopUp(void*);
void popPopUp(void*);