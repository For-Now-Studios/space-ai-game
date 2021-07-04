#pragma once
#include "click.h"

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

struct roomPopupPars : popPopUpPars {
	roomPopupPars(popPopUpPars pPUP) : popPopUpPars{pPUP} {}
	Room *room;
	closePopUpPars *close;
};

struct DoorClickPars {
	Door *door;
	MouseStruct *mouse;
	Image *open;
	Image *closed;
	Image *locked;
};

void testClosePopUp(void*);
void testPopPopUp(void*);
void testPopPopUp0(void*);
void roomPopup(void*);
void closeRoomPopup(void*);
/*
 The function called when you click a door
 If you left click it will open/close door
 Right click lock/unlock door
 A locked door is closed and can't be opened
 Opening a locked door (as the AI) will unlock it and open it.
 Has *pretty* graphics to show this
*/
void doorClick(void*);
