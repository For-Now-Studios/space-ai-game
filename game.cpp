#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<string.h>
#include "engine.h"
#include "structs.h"
#include "globals.h"
#include "click.h"
#include "clickfunctions.h"
#include "graph.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

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
	const char *path, CurrentClick* cc, Labels* labels, MouseStruct* mouse,
	Graph<GameObject *, int> *pG){

	/* ROOMS */
	// The bridge
	roomPopupPars* rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *cockpit = new Room(0, 0, media->images.at(5), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = cockpit;

	// Bridge hall
	rPup = new roomPopupPars(popPopUpPars{ cc, false, objects, media});
	Room *bridgeHall = new Room(128, 0, media->images.at(6), roomPopup,
								(void *)(rPup), HALL);
	rPup->room = bridgeHall;

	// Bedroom 1
	rPup = new roomPopupPars(popPopUpPars{ cc, false, objects, media});
	Room *bRoom1 = new Room(256, 0, media->images.at(5), roomPopup, (void *)(rPup),
										BEDROOM);
	rPup->room = bRoom1;

	// Bedroom 2
	rPup = new roomPopupPars(popPopUpPars{ cc, false, objects, media});
	Room *bRoom2 = new Room(384, 0, media->images.at(5), roomPopup, (void *)(rPup),
										BEDROOM);
	rPup->room = bRoom2;

	// Bedroom 3
	rPup = new roomPopupPars(popPopUpPars{ cc, false, objects, media});
	Room *bRoom3 = new Room(256, 64, media->images.at(5), roomPopup, (void *)(rPup),
										BEDROOM);
	rPup->room = bRoom3;

	// Bedroom 4
	rPup = new roomPopupPars(popPopUpPars{ cc, false, objects, media});
	Room *bRoom4 = new Room(384, 64, media->images.at(5), roomPopup, (void *)(rPup),
										BEDROOM);
	rPup->room = bRoom4;

	// Kitchen Hall
	rPup = new roomPopupPars(popPopUpPars{ cc, false, objects, media});
	Room *kitchenHall = new Room(128, 64, media->images.at(6), roomPopup,
								(void *)(rPup), HALL);
	rPup->room = kitchenHall;

	// Kitchen
	rPup = new roomPopupPars(popPopUpPars{ cc, false, objects, media});
	Room *kitchen = new Room(0, 64, media->images.at(5), roomPopup,
								(void *)(rPup), KITCHEN);
	rPup->room = kitchen;

	/* CHARACTERS */
	// Paul
	CharacterObject *paul = new CharacterObject(320, 0,
		media->images.at(7), btnHello, (void *)(new btnHelloParameter{"Paul"}),
		"Paul", intersex, labels->genders->at(0), labels->romance->at(0),
							labels->sexuality->at(0), pilot);

	// Paulette
	CharacterObject *paulette = new CharacterObject(400, 0,
		media->images.at(7), btnHello,
		(void *)(new btnHelloParameter{"Paulette"}), "Paulette", intersex,
					labels->genders->at(3), labels->romance->at(1),
							labels->sexuality->at(1), engineer);
	
	// Paulus
	CharacterObject *paulus = new CharacterObject(310, 64,
		media->images.at(7), btnHello,
		(void *)(new btnHelloParameter{"Paulus"}), "Paulus", female,
		labels->genders->at(2), labels->romance->at(2),
							labels->sexuality->at(2), doctor);

	// Paulob
	CharacterObject *paulob = new CharacterObject(420, 64,
		media->images.at(7), btnHello,
		(void *)(new btnHelloParameter{"Paulob"}), "Paulob", male,
		labels->genders->at(2), labels->romance->at(3),
							labels->sexuality->at(3), captain);
  
  //Add tasks for paulette:
	paulette->addTask(new Task{ 120,120,btnHello,(void*)(new btnHelloParameter{"Start!"}),1,0,10,"Start",AIASSIGNED });
	paulette->addTask(new Task{ 240,120,btnHello,(void*)(new btnHelloParameter{"Place0!"}),2,0,10,"Place0",AGAINSTFRIENDS });
	paulette->addTask(new Task{ 240,240,btnHello,(void*)(new btnHelloParameter{"Place1!"}),3,0,10,"Place1",FORLOVE });
	paulette->addTask(new Task{ 120,240,btnHello,(void*)(new btnHelloParameter{"Place2!"}),4,0,10,"Place2",FORENEMIES });

	/* Doors */
	// Bridge Door
	DoorClickPars* DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *bridgeDoor = new Door(128, 0, media->images.at(3), doorClick, DCP);
	DCP->door = bridgeDoor;
	cockpit->doors.push_back(bridgeDoor);
	bridgeHall->doors.push_back(bridgeDoor);
	pG->addNode(bridgeDoor);
	strcpy(bridgeDoor->n, "BridgeDoor");
	//TestMap (all doors teleport to self)
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *t = new Door(129, 0, media->images.at(3), doorClick, DCP);
	DCP->door = t;
	pG->addNode(t);
	pG->addEdge(bridgeDoor, t, 1);
	pG->addEdge(t, bridgeDoor, 1);
	strcpy(t->n, "BridgeDoorTemp (right)");

	// Bridige Hall hatchet
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *hallHatch = new Door(172, 54, media->images.at(9), doorClick, DCP);
	DCP->door = hallHatch;
	bridgeHall->doors.push_back(hallHatch);
	kitchenHall->doors.push_back(hallHatch);
	pG->addNode(hallHatch);
	pG->addEdge(t, hallHatch, 1);
	pG->addEdge(hallHatch, t, 1);
	strcpy(hallHatch->n, "hallHatch");
	
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(11);
	DCP->closed = media->images.at(11);
	DCP->locked = media->images.at(8);
	Door *bot = new Door(172, 64, media->images.at(11), doorClick, DCP);
	DCP->door = bot;
	hallHatch->bottom = bot;
	bot->bottom = hallHatch;
	pG->addNode(bot);
	pG->addEdge(bot, hallHatch, 1);
	pG->addEdge(hallHatch, bot, 1);
	strcpy(bot->n, "Bottom");

	// Bedroom 1 Door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *bRoom1Door = new Door(256, 0, media->images.at(3), doorClick, DCP);
	DCP->door = bRoom1Door;
	bridgeHall->doors.push_back(bRoom1Door);
	bRoom1->doors.push_back(bRoom1Door);
	pG->addNode(bRoom1Door);
	pG->addEdge(bRoom1Door, hallHatch, 1);
	pG->addEdge(hallHatch, bRoom1Door, 1);
	pG->addEdge(bRoom1Door, t, 1);
	pG->addEdge(t, bRoom1Door, 1);
	strcpy(bRoom1->n, "Bedroom1");
	//TestMap (all doors teleport to self)
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	t = new Door(257, 0, media->images.at(3), doorClick, DCP);
	DCP->door = t;
	pG->addNode(t);
	pG->addEdge(bRoom1Door, t, 1);
	pG->addEdge(t, bRoom1Door, 1);
	strcpy(t->n, "Bedroom1Temp (right)");

	// Bedroom 2 Door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *bRoom2Door = new Door(384, 0, media->images.at(3), doorClick, DCP);
	DCP->door = bRoom2Door;
	bRoom1->doors.push_back(bRoom2Door);
	bRoom2->doors.push_back(bRoom2Door);
	pG->addNode(bRoom2Door);
	pG->addEdge(bRoom2Door, t, 1);
	pG->addEdge(t, bRoom2Door, 1);
	strcpy(bRoom2->n, "Bedroom2");
	//TestMap (all doors teleport to self)
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	t = new Door(385, 0, media->images.at(3), doorClick, DCP);
	DCP->door = t;
	pG->addNode(t);
	pG->addEdge(bRoom2Door, t, 1);
	pG->addEdge(t, bRoom2Door, 1);
	strcpy(t->n, "Bedroom2Temp (right)");

	// Bedroom 3 Door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *bRoom3Door = new Door(256, 64, media->images.at(3), doorClick, DCP);
	DCP->door = bRoom3Door;
	kitchenHall->doors.push_back(bRoom3Door);
	bRoom3->doors.push_back(bRoom3Door);
	pG->addNode(bRoom3Door);
	pG->addEdge(bRoom3Door, bot, 1);
	pG->addEdge(bot, bRoom3Door, 1);
	strcpy(bRoom3->n, "Bedroom3");
	//TestMap (all doors teleport to self)
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	t = new Door(257, 64, media->images.at(3), doorClick, DCP);
	DCP->door = t;
	pG->addNode(t);
	pG->addEdge(bRoom3Door, t, 1);
	pG->addEdge(t, bRoom3Door, 1);
	strcpy(t->n, "Bedroom3Temp (right)");

	// Bedroom 4 Door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *bRoom4Door = new Door(384, 64, media->images.at(3), doorClick, DCP);
	DCP->door = bRoom4Door;
	bRoom3->doors.push_back(bRoom4Door);
	bRoom4->doors.push_back(bRoom4Door);
	pG->addNode(bRoom4Door);
	pG->addEdge(bRoom4Door, t, 1);
	pG->addEdge(t, bRoom4Door, 1);
	strcpy(bRoom4->n, "Bedroom4");
	//TestMap (all doors teleport to self)
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	t = new Door(385, 64, media->images.at(3), doorClick, DCP);
	DCP->door = t;
	pG->addNode(t);
	pG->addEdge(bRoom4Door, t, 1);
	pG->addEdge(t, bRoom4Door, 1);
	strcpy(t->n, "Bedroom4Temp (right)");

	// Kitchen Door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *kitchenDoor = new Door(128, 64, media->images.at(3), doorClick, DCP);
	DCP->door = kitchenDoor;
	kitchen->doors.push_back(kitchenDoor);
	kitchenHall->doors.push_back(kitchenDoor);
	pG->addNode(kitchenDoor);
	pG->addEdge(kitchenDoor, bot, 1);
	pG->addEdge(bot, kitchenDoor, 1);
	pG->addEdge(kitchenDoor, bRoom3Door, 1);
	pG->addEdge(bRoom3Door, kitchenDoor, 1);
	strcpy(kitchenDoor->n, "KitchenDoor (right)");
	//TestMap (all doors teleport to self)
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	t = new Door(120, 64, media->images.at(3), doorClick, DCP);
	DCP->door = t;
	pG->addNode(t);
	pG->addEdge(kitchenDoor, t, 1);
	pG->addEdge(t, kitchenDoor, 1);
	strcpy(t->n, "KitchenDoorTemp (left)");

	// Add all clickable elements to the click system
	buildClickAreas(cc,
		{paul, paulette, paulus, paulob},
		{cockpit, bridgeHall, bRoom1, bRoom2, bRoom3, bRoom4, kitchenHall,
			kitchen},
		{},
		{},
		{bridgeDoor, bRoom1Door, bRoom2Door, bRoom3Door, bRoom4Door, kitchenDoor,
			hallHatch}
	);

	/* Add all objects to the objects list */
	// Rooms
	objects->push_back(cockpit);
	objects->push_back(bridgeHall);
	objects->push_back(bRoom1);
	objects->push_back(bRoom2);
	objects->push_back(bRoom3);
	objects->push_back(bRoom4);
	objects->push_back(kitchenHall);
	objects->push_back(kitchen);
	//Ladders
	objects->push_back(bot);
	// Characters
	objects->push_back(paul);
	objects->push_back(paulette);
	objects->push_back(paulus);
	objects->push_back(paulob);
	// Doors
	objects->push_back(bridgeDoor);
	objects->push_back(hallHatch);
	objects->push_back(bRoom1Door);
	objects->push_back(bRoom2Door);
	objects->push_back(bRoom3Door);
	objects->push_back(bRoom4Door);
	objects->push_back(kitchenDoor);

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
	
	Labels labels;
	Graph<CharacterObject *, Relation> *relGraph = nullptr;
	Graph<GameObject *, int> *pathGraph = new Graph<GameObject *, int>();
	vector<CharacterObject *> characters;

	if(running){
		labels.genders = loadGender("gender.jpeg");
		labels.romance = loadAffectionTrait("romance.jpeg");
		labels.sexuality = loadAffectionTrait("sexuality.jpeg");

		if(loadLevel(&objects, &media, "", &currClick, &labels, &mouse,
									pathGraph)){
			printf("Game object done!\n");

			for(int i = 0; i < currClick.Characters.size(); i++){
				characters.push_back((CharacterObject *)
							currClick.Characters.at(i));
			}

			relGraph = initRelations(&characters);
			//pathGraph = initPathfinding(&currClick.rooms);
		}
	}

	Camera cam;
	cam.x = 0;
	cam.y = 0;
	cam.zoomLevel = 0.25f;
	cam.wndX = 0;
	cam.wndY = 0;

	// Timing
	unsigned int targetFrequency = 60;
	Uint32 targetTime = 1000 / targetFrequency;
	Uint32 startTime = SDL_GetTicks();

	printf("Initialization done\n");

	// The Loop
	int ticks = 0;

	//TEST CODE TODO: REMOVE
	bool key[6];
	for(bool k : key) k = false;

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
					mouse.relX = event.motion.x-mouse.x;
					mouse.relY = event.motion.y-mouse.y;
					mouse.x = event.motion.x;
					mouse.y = event.motion.y;
					//mouse.relX = event.motion.xrel;
					//mouse.relY = event.motion.yrel;
					break;
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
					mouse.relX = event.motion.x-mouse.x;
					mouse.relY = event.motion.y-mouse.y;
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

				//TEMPORARY. TODO: REMOVE
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_w:
							key[0] = true;
							break;
						case SDLK_a:
							key[1] = true;
							break;
						case SDLK_s:
							key[2] = true;
							break;
						case SDLK_d:
							key[3] = true;
							break;
						case SDLK_UP:
							key[4] = true;
							break;
						case SDLK_DOWN:
							key[5] = true;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym){
						case SDLK_w:
							key[0] = false;
							break;
						case SDLK_a:
							key[1] = false;
							break;
						case SDLK_s:
							key[2] = false;
							break;
						case SDLK_d:
							key[3] = false;
							break;
						case SDLK_UP:
							key[4] = false;
							break;
						case SDLK_DOWN:
							key[5] = false;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}

		//Update the movment of a character
		for(CharacterObject *c : characters){
			updateMovement(c, &currClick.rooms, pathGraph);
		}

		//TEST!!! TODO: REMOVE!
		if(characters.at(0)->goal == nullptr){
			if(whichRoom(&currClick.rooms, characters.at(0)) !=
								currClick.rooms.at(7)){
				characters.at(0)->goal = currClick.rooms.at(7);
			}
			else{
				characters.at(0)->goal = currClick.rooms.at(2);
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

		if(key[0]) cam.y -= 20;
		if(key[1]) cam.x -= 20;
		if(key[2]) cam.y += 20;
		if(key[3]) cam.x += 20;
		if(key[4]) cam.zoomLevel += 0.01;
		if(key[5]) cam.zoomLevel -= 0.01;
    
		updateClickAreas(&currClick);
		
		//Render
		render(&window, media.images.at(12), 0, 0, &cam); //Render background
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
