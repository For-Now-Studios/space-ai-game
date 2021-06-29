#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<string.h>
#include "engine.h"
#include "structs.h"
#include "globals.h"
#include "click.h"
#include "clickfunctions.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

struct Labels {
	vector<char *> *genders = nullptr;
	vector<affectionTrait *> *romance = nullptr;
	vector<affectionTrait *> *sexuality = nullptr;
};

/*
	TODO: Maybe make this function load data from a level file?
	TODO: Divide this function up to different functions that each return a vector/list_intializer,
	that we finally put into the CreateClickableArea function and into the objects vector.
	
	NOTE: Currently just a collection of used game objects collected in a singular
	space for ease of use
*/
bool loadLevel(vector<GameObject *>* objects, Media* media,
	const char *path, CurrentClick* cc, Labels* labels, MouseStruct* mouse){
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
	
	GameObjClick *button = new GameObjClick(0, 0, media->images.at(0), testPopPopUp,
		(void*)(new popPopUpPars{cc, false, objects, media}));

	GameObjClick *ui0 = new GameObjClick(0, 420, media->images.at(1), btnHello, //blaze it
		(void*)(new btnHelloParameter{ "ui0!" }));

	GameObjClick *button0 = new GameObjClick(0, 120, media->images.at(0), testPopPopUp0,
		(void*)(new popPopUpPars{cc, false, objects, media}));

	roomPopupPars* rPUP = new roomPopupPars(popPopUpPars{ cc, false, objects, media });
	Room *roomTest = new Room(120, 120, media->images.at(0), roomPopup,
		(void*)(rPUP),
		TOILET | AICORE | CLEARLYFATAL
	);
	rPUP->room = roomTest;
	roomTest->buttons.push_back(new GameObjClick(0, 0, media->images.at(1), btnHello,
		(void*)(new btnHelloParameter{ "room option 0!" })));
	roomTest->buttons.push_back(new GameObjClick(0, 0, media->images.at(1), btnHello,
		(void*)(new btnHelloParameter{ "room option 1!" })));
	roomTest->buttons.push_back(new GameObjClick(0, 0, media->images.at(1), btnHello,
		(void*)(new btnHelloParameter{ "room option 2!" })));
	roomTest->buttons.push_back(new GameObjClick(0, 0, media->images.at(1), btnHello,
		(void*)(new btnHelloParameter{ "room option 3!" })));
	roomTest->buttons.push_back(new GameObjClick(0, 0, media->images.at(1), btnHello,
		(void*)(new btnHelloParameter{ "room option 4!" })));

	CharacterObject *paul = new CharacterObject(420, 300,
		media->images.at(1), btnHello,
		(void *)(new btnHelloParameter{ "Come on mr tally man, tally my\
			banana!" }), "Paul", intersex, labels->genders->at(0), labels->romance->at(0),
		labels->sexuality->at(0));

	DoorClickPars* DCP = new DoorClickPars;
	Door *doorTest = new Door(600, 300, media->images.at(2), doorClick, DCP);
	DCP->door = doorTest;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);

	//Build clickable areas
	buildClickAreas(cc, { c0, paul }, { roomTest }, { ui0 }, {}, {button, button0, doorTest});

	objects->push_back(obj);
	objects->push_back(obj2);
	objects->push_back(button);
	objects->push_back(c0);
	objects->push_back(ui0);
	objects->push_back(button0);
	objects->push_back(paul);
	objects->push_back(roomTest);
	objects->push_back(doorTest);

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

void close(WindowStruct *window, Media& media, vector<GameObject*>& objects,
	CurrentClick* cc, Labels *labels){
	for(Image* img : media.images)
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

	for(GameObject* obj : objects) {
		delete obj;
	}

	for(char *t : *labels->genders){
		delete t;
	}
	printf("Freed the genders!\n");
	
	for(affectionTrait *t : *labels->romance){
		delete t->name;
		for(int i = 0; i < t->n; i++){
			delete t->genders[i];
		}
		delete t->genders;

		delete t;
	}
	printf("Freed the romances!\n");

	for(affectionTrait *t : *labels->sexuality){
		delete t->name;
		for(int i = 0; i < t->n; i++){
			delete t->genders[i];
		}
		delete t->genders;

		delete t;
	}
	printf("Freed the sexuality!\n");

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
	vector<GameObject*> objects;
	
	vector<char *> *genders = nullptr;
	vector<affectionTrait *> *romance = nullptr;
	vector<affectionTrait *> *sexuality = nullptr;
	Labels labels;

	int channel; //MUSIC TEST
	if(running){
		genders = loadGender("gender.jpeg");
		romance = loadAffectionTrait("romance.jpeg");
		sexuality = loadAffectionTrait("sexuality.jpeg");
		labels.genders = genders;
		labels.romance = romance;
		labels.sexuality = sexuality;

		if(loadLevel(&objects, &media, "", &currClick, &labels, &mouse)){
			printf("Game object done!\n");

			// MUSIC TEST
			Mix_Volume(-1, masterVolume);
			Mix_VolumeMusic(masterVolume);
			switchMusic(media.music.at(0), -1, 0, 60000);
			channel = playSound(media.sounds.at(0), -1, 2.0f, 1000);
		
			media.images.push_back(new Image(media.fonts.at(0),
						"Hello Jacob!", {0,0,0}, window.render));
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

	printf("\nFeatures the genders of:\n");
	for(char *t : *genders){
		printf("%s\n", t);
	}

	printf("With a slice of:\n");
	for(affectionTrait *t : *romance){
		printf("%s\n", t->name);

		for(int i = 0; i < t->n; i++){
			printf("\t%s\n", t->genders[i]);
		}
	}

	printf("And some casual:\n");
	for(affectionTrait *t : *sexuality){
		printf("%s\n", t->name);

		for(int i = 0; i < t->n; i++){
			printf("\t%s\n", t->genders[i]);
		}
	}


	// The Loop
	int ticks = 0;
	while(running){
		SDL_RenderClear(window.render);

		//Need to make these zero or they will get stuck.
		for(MouseStruct::Button& button : mouse.buttons) button.isReleased = 0;
		mouse.scrollRight = 0;
		mouse.scrollUp = 0;
		mouse.relX = 0;
		mouse.relY = 0;

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
					mouse.relX = event.motion.xrel;
					mouse.relY = event.motion.yrel;
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
		if (mouse.buttons[0].isPressed) {
			checkCord(&currClick, mouse, &cam);
		}
		if (mouse.buttons[0].isReleased || mouse.buttons[2].isReleased) {
			IsClickable* clicked = checkCord(&currClick, mouse, &cam);
			if (clicked != nullptr) {
				clicked->function(clicked->data);
			}
			currClick.currentlySelected = nullptr;
		}

		// Testing moving characters
		objects.at(3)->moveBy(speed, 0);
		objects.at(6)->moveBy(speed, speed/3);
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
		render(&window, media.images.at(1), 120, 300);
		
		for(GameObject* obj : objects) {
			render(&window, obj, &cam);
		}
		for (vector<GameObject*>* vec : currClick.toRender)
		{
			for (GameObject* obj : *vec) {
				render(&window, obj, &cam);
			}
		}

		// If we were too quick, wait until it is time
		Uint32 time = SDL_GetTicks() - startTime;
		if(time < targetTime){
			SDL_Delay(targetTime - time);
		}

		startTime = SDL_GetTicks();		

		SDL_RenderPresent(window.render);
	}

	close(&window, media, objects, &currClick, &labels);

	return 0;
}
