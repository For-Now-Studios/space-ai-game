#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<string.h>
#include "engine.h"
#include "structs.h"
#include "globals.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/*
	Test funtion and struct parameter.
*/
struct btnHelloParameter { const char* name; };
void btnHello(void *cntxt)
{
	btnHelloParameter *parameters = (btnHelloParameter*)cntxt;
	printf("Hello %s!\n", parameters->name);
}

/*
	Builds the clickable areas.
*/
void buildClickAreas(CurrentClick *cc, vector<IsClickable*> clickable) {
	cc->Characters.push_back(clickable.at(1));
	cc->numChars[0] = 1;
	cc->numChars[1] = 0;

	//UI
	ClickArea *UI = new ClickArea;
	UI->area = SDL_Rect{0, 420, SCREEN_WIDTH, 60};
	UI->clicks.push_back(clickable.at(2));

	cc->UI.push_back(UI);

	//Popup
	//nothign in the start

	//Game
	ClickArea *S0 = new ClickArea;
	S0->area = SDL_Rect{0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT};
	S0->clicks.push_back(clickable.at(0));
	S0->clicks.push_back(clickable.at(1));

	ClickArea *S1 = new ClickArea;
	S1->area = SDL_Rect{SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT};
	
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
	for (int i = first; i < last+1; i++)
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
	for(int i = 0; i < cc->Game.size(); i++) {
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

		closePopUpPars* cPUP = new closePopUpPars{0,0,0,pars->cc,pars};
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

/*
	TODO: Maybe make this function load data from a level file?
	
	NOTE: Currently just a collection of used game objects collected in a singular
	space for ease of use
*/
bool loadLevel(vector<GameObject *>* objects, vector<IsClickable *>* clickable,
							Media* media, const char *path, CurrentClick* cc){
	GameObject *obj = new GameObject;
	obj->image = media->images.at(0);
	obj->x = 0;
	obj->y = 0;

	GameObject *obj2 = new GameObject;
	obj2->image = media->images.at(0);
	obj2->x = 120;
	obj2->y = 0;

	/*	So we can go through all buttons later on.*/
	GameObjClick *c0 = new GameObjClick(0, 200, media->images.at(1), btnHello,
		(void*)(new btnHelloParameter{ "c0!" }));
	
	GameObjClick *button = new GameObjClick(0, 0, media->images.at(0), popPopUp,
		(void*)(new popPopUpPars{cc, false, objects, media}));

	GameObjClick *ui0 = new GameObjClick(0, 420, media->images.at(1), btnHello, //blaze it
		(void*)(new btnHelloParameter{ "ui0!" }));

	clickable->push_back(button);
	clickable->push_back(c0);
	clickable->push_back(ui0);

	objects->push_back(obj);
	objects->push_back(obj2);
	objects->push_back(button);
	objects->push_back(c0);
	objects->push_back(ui0);
	

	return true;
}

/*
	Initialize the things needed for the engine to run
*/
bool init(WindowStruct *window) {
	//Initialize the SDL library and check if it didn't work, otherwise continue
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
		printf("SDL couldn't be initialized. Error: %s\n", SDL_GetError());

		return false;
	}

	//Create the program window
	window->window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Couldn't create a window. Error: %s\n", SDL_GetError());

		return false;
	}
	//Initialize the renderer
	window->render = SDL_CreateRenderer(window->window, -1,
							SDL_RENDERER_ACCELERATED);
	if (window->render == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());

		return false;
	}

	//Set the background colour of the window
	SDL_SetRenderDrawColor(window->render, 102, 102, 102, 255);

	//Initialize Sound Drivers
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_Mixer could not initialize! SDL_Mixer Error: %s\n",
									Mix_GetError());

		return false;
	}

	//Initialize Fonts
	if (TTF_Init() == -1) {
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
									TTF_GetError());

		return false;
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n",
									IMG_GetError());

		return false;
	}
	
	return true;
}

void close(WindowStruct *window, Media& media, vector<GameObject*>& objects, CurrentClick* cc){
	for (Image* img : media.images)
	{
		img->~Image();
	}

	for(Music* m : media.music){
		m->~Music();
	}

	for(Sound* s : media.sounds){
		s->~Sound();
	}

	for(Font* f : media.fonts){
		f->~Font();
	}

	cleanClickAreas(cc);

	for (GameObject* obj : objects) {
		delete obj;
	}

	SDL_DestroyRenderer(window->render);
	SDL_DestroyWindow(window->window);

	Mix_CloseAudio();

	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}

int main(int argc, char *argv[]){
	WindowStruct window;

	if(!init(&window)){
		exit(1);
	}

	MouseStruct mouse;
	mouse.x = 0;
	mouse.y = 0;
	mouse.scrollRight = 0;
	mouse.scrollUp = 0;
	mouse.buttons[0] = MouseStruct::Button{ false, false, false };
	mouse.buttons[1] = MouseStruct::Button{ false, false, false };
	mouse.buttons[2] = MouseStruct::Button{ false, false, false };

	Media media;
	bool running = loadMedia(&media, "manifest", window.render);

	CurrentClick currClick;
	vector<IsClickable*> clickable;
	vector<GameObject*> objects;

	int channel; //MUSIC TEST
	if(running){
		if(loadLevel(&objects, &clickable, &media, "", &currClick)){
			printf("Game object done!\n");

			// MUSIC TEST
			Mix_Volume(-1, masterVolume);
			Mix_VolumeMusic(masterVolume);
			switchMusic(media.music.at(0), -1, 0, 60000);
			channel = playSound(media.sounds.at(0), -1, 2.0f, 1000);
		
			media.images.push_back(new Image(media.fonts.at(0),
						"Hello Jacob!", {0,0,0}, window.render));
			
			//Clickable areas
			buildClickAreas(&currClick, clickable);
		}
	}
	//Pause the music & sound
	pauseAll();

	Camera cam;
	cam.x = 0;
	cam.y = 0;
	cam.zoomLevel = 1.0f;
	cam.wndX = 0;
	cam.wndY = 0;

	// Timing
	unsigned int targetFrequency = 60;
	Uint32 targetTime = 1000 / targetFrequency;
	Uint32 startTime = SDL_GetTicks();

	printf("Initialization done\n");

	int speed = 5; //For testing characters

	// The Loop
	int ticks = 0;
	while(running){
		SDL_RenderClear(window.render);

		//Need to make these zero or they will get stuck.
		for(MouseStruct::Button& button : mouse.buttons) button.isReleased = 0;
		mouse.scrollRight = 0;
		mouse.scrollUp = 0;

		// Fading out of continous sound test 
		/*if(ticks == 180){
			pauseAllSound();
		}
		if(ticks == 240){
			resumeAllSound();
		}
		else if(ticks == 360){
			stopSound(channel, 1000);
		}
		else if(ticks == 640){
			switchMusic(media.music.at(1), 0, 1000, 1000);
		}
		else if(ticks == 820){
			pauseMusic();
		}
		else if(ticks == 940){
			resumeMusic();
			playSound(media.sounds.at(0));
		}
		else if(ticks == 1000){
			pauseAll();
		}
		else if(ticks == 1060){
			resumeAllSound();
		}
		else if(ticks == 1180){
			pauseAll();
		}
		else if(ticks == 1300){
			resumeAll();
		}
		ticks++;*/

		//Start handling events.
		SDL_Event event;
		while(SDL_PollEvent(&event) != 0){
			switch(event.type){
				case SDL_QUIT:
					running = false;
					break;

				case SDL_MOUSEMOTION:
					mouse.x = event.motion.x;
					mouse.y = event.motion.y;
					break;
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
					mouse.x = event.button.x;
					mouse.y = event.button.y;
					/* In SDL, left button = 1, middle = 2, and
					   right = 3. If we subtract this by one we can
					   use our array setup
					*/
					mouse.buttons[event.button.button - 1]
						.doubleClick = event.button.clicks - 1;
					mouse.buttons[event.button.button - 1]
						.isPressed = event.button.state == 
									SDL_PRESSED;
					mouse.buttons[event.button.button - 1]
						.isReleased = event.button.state == 
									SDL_RELEASED;
					break;
				case SDL_MOUSEWHEEL:
					mouse.scrollRight = event.wheel.x;
					mouse.scrollUp = event.wheel.y;
					break;
				default:
					break;
			}
		}

		//More advanced simple button test
		if (mouse.buttons[0].isReleased) {
			IsClickable* clicked = checkCord(&currClick, mouse.x, mouse.y);
			if (clicked != nullptr) {
				clicked->function(clicked->data);
			}
		}

		// Testing moving characters
		objects.at(3)->moveBy(speed, 0);
		if (objects.at(3)->x > SCREEN_WIDTH - 60 || objects.at(3)->x < 1) {
			speed *= -1;
		}
		updateClickAreas(&currClick);
    //Prints the amount of characters for each part of the screen
		//printf("S0: %d, S1: %d\n", currClick.numChars[0], currClick.numChars[1]);

		SDL_Rect temp = {mouse.x, mouse.y, 1, 1};
		SDL_Rect trans = translateToGame(&cam, &temp);
		objects.at(0)->x = trans.x - objects.at(0)->image->width / 2;
		objects.at(0)->y = trans.y -  objects.at(0)->image->height / 2;

		//render(&window, media.images.at(0), 0, 120);
		//render(&window, media.images.at(0), 120, 120, &cam);
		render(&window, media.images.at(2), 120, 300);
		
		for(GameObject* obj : objects) {
			render(&window, obj, &cam);
		}

		// If we were too quick, wait until it is time
		Uint32 time = SDL_GetTicks() - startTime;
		if(time < targetTime){
			SDL_Delay(targetTime - time);
		}

		startTime = SDL_GetTicks();		

		SDL_RenderPresent(window.render);
	}

	close(&window, media, objects, &currClick);

	return 0;
}
