#pragma once
#include<vector>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include<list>

struct Room;

struct WindowStruct {
	SDL_Window *window;
	SDL_Texture *screenTexture;
	SDL_Renderer *render;
};

struct Font {
	TTF_Font *font;

	Font(const char *path, int size){
		font = TTF_OpenFont(path, size);

		if(font == NULL) printf("Could not open font at %s\n", path);
	}

	~Font() {
		TTF_CloseFont(font);
		printf("Font has been freed!\n");
	}
};

struct Image {
	SDL_Texture *image;
	int width;
	int height;

	Image(const char *path, SDL_Renderer *render){
		SDL_Surface *temp = IMG_Load(path);
		if (temp == NULL) printf("Failed to load image at %s\n", path);

		width = temp->w;
		height = temp->h;

		image = SDL_CreateTextureFromSurface(render, temp);
		if (image == NULL) printf("Failed to convert %s to a texture!\n", path);

		SDL_FreeSurface(temp);

		printf("Image %s has been loaded!\n", path);
	}
	Image(const char *path, int w, int h, SDL_Renderer *render) : width{ w },
									height{ h } {
		SDL_Surface *temp = IMG_Load(path);
		if (temp == NULL) printf("Failed to load image at %s\n", path);

		image = SDL_CreateTextureFromSurface(render, temp);
		if (image == NULL) printf("Failed to convert %s to a texture!\n", path);

		SDL_FreeSurface(temp);

		printf("Image %s has been loaded!\n", path);
	}

	Image(Font *font, const char *text, SDL_Color colour, SDL_Renderer *render){
		if(TTF_SizeUTF8(font->font, text, &width, &height) == -1){
			printf("Could not render the string '%s'\n", text);
		}

		SDL_Surface *temp = TTF_RenderUTF8_Blended(font->font, text, colour);
		if (temp == NULL) printf("Failed to turn '%s' to an image\n", text);

		image = SDL_CreateTextureFromSurface(render, temp);
		if (image == NULL) printf("Failed to convert '%s' to an image!\n", text);

		SDL_FreeSurface(temp);

		printf("Text %s has been rendered!\n", text);
	}

	~Image() {
		SDL_DestroyTexture(image);
		printf("Image has been freed!\n");
	}
};

struct Music {
	Mix_Music *music;

	Music(const char *path){
		music = Mix_LoadMUS(path);

		if(music == NULL) printf("Could not load music at %s\n", path);
	}

	~Music(){
		Mix_FreeMusic(music);
		printf("Music has been freed!\n");
	}

};

struct Sound {
	Mix_Chunk *sound;

	Sound(const char *path){
		sound = Mix_LoadWAV(path);
		if(sound == NULL) printf("Could not load sound at %s\n", path);
	}

	~Sound(){
		Mix_FreeChunk(sound);
		printf("Sound has been freed!\n");
	}
};

struct GameObject {
	//Game Coordinates
	int x;
	int y;
	char n[256];

	//Images
	Image *image;

	virtual ~GameObject() {

	}
	
	// Function to move the object, implemented mostly because so children can make their own move functions and be run.
	virtual void moveBy(int xDir, int yDir) { 
		x += xDir;
		y += yDir;
	}
	virtual void moveTo(int a, int b) {
		x = a;
		y = b;
	}
};

/*
	An object contain all information about the mouse.
*/
struct MouseStruct {
	//Screen coordinates.
	int x;
	int y;

	int relX;
	int relY;

	int scrollUp; //1 scrolling up, -1 down, 0 no scrolling
	int scrollRight; //1 scrolling right, -1 left, 0 no scrolling

	//Nested button, for each button on the mouse.
	struct Button {
		bool isPressed;
		bool isReleased;
		bool doubleClick; //If the button is doubleclicked, it is usually 1.
	};

	Button buttons[3]; //0 left, 1 middle, 2 right button
};

/*
	A object used for translating game coordinates to window coordinates;
	A camera has a pair of in game coordinates, a width and height, and a set of
	window coordinates. It can then be used translate the game coordinates of game
	entities to coordinates to be displayed on the screen
*/
struct Camera {
	//Game Coordinates
	int x;
	int y;

	//Zoom
	double zoomLevel;

	//Window Coordinates
	int wndX;
	int wndY;
};

struct Media{
	std::vector<Image*> images;
	std::vector<Sound*> sounds;
	std::vector<Music*> music;
	std::vector<Font*> fonts;
};

//Struct that can be added to make an object clickable.
struct IsClickable {
	SDL_Rect area; //Area that is clickable
	void(*function)(void*); //The function being called when clicked
	void* data = nullptr; //The data sent into the function

	// Destructor to delete the data, as it needs to be put on the heap (I think in most cases)
	~IsClickable() {
		delete data;
		data = nullptr;
		printf("Data for a clickable thing has been freed!\n");
	}
};

/*
	A clickable version of GameObject
*/
struct GameObjClick : GameObject, IsClickable {
	GameObjClick(int xPos, int yPos, Image *img, void(*func)(void*), void* d) {
		x = xPos;
		y = yPos;
		image = img;

		area = SDL_Rect{ x, y, image->width, image->height };
		function = func;
		data = d;
	}
	GameObjClick(int xPos, int yPos, Image *img, SDL_Rect ar, void(*func)(void*), void* d) {
		x = xPos;
		y = yPos;
		image = img;

		area = ar;
		function = func;
		data = d;
	}

	void moveBy(int xDir, int yDir) {
		x += xDir;
		y += yDir;
		area.x += xDir;
		area.y += yDir;
	}

	void moveTo(int a, int b){
		x = a;
		y = b;
		area.x = x;
		area.y = y;
	}
};

struct CharacterObject;

#define AIASSIGNED (1 << 0)
#define SELFCARE (1 << 1)
#define EMERGENCY (1 << 2)
#define LIFEORDEATH (1 << 3) 
#define FORFRIENDS (1 << 4)
#define FORLOVE (1 << 5)
#define FORENEMIES (1 << 6) 
#define FORHATE (1 << 7)
#define AGAINSTENEMY (1 << 8)
#define AGAINSTLOVE (1 << 9)
#define AGAINSTFRIENDS (1 << 10)
#define WAITINGFOR (1 << 11)

struct Task {
	// Location
	GameObject *location;

	// Effect
	void(*function)(void*); //The function that does its effect
	void* data = nullptr; //The data sent into the function
	
	int priority; //How important is this job?
	int actualPrio; //How ACTUALLY important is this?
	int waitTime; //Until the effect triggers when in the same room
	int tolerance; //Unti the task is abandoned (< 0 means infinite)

	Task *sisterTask = nullptr; //Pointer to a related task for another character

	CharacterObject* waitingFor;

	const char* name;
	Image *icon;
	Image *waitingIcon;

	int flag;

	Task(GameObject* loc, void(*func)(void*), void* d, int prio, int wait,
								const char* n, int f) : 
			location{loc}, function{func}, data{d}, priority{prio},
			actualPrio{0}, waitTime{wait}, waitingFor{nullptr}, name{ n }, 
			flag{ f }{
		icon = nullptr;
		waitingIcon = nullptr;
		tolerance = 60*10;
	}
	Task(GameObject* loc, void(*func)(void*), void* d, int prio, int wait,
					const char* n, int f, Image *i, Image *wi) :
			location{loc}, function{func}, data{d}, priority{prio},
			actualPrio{0}, waitTime{wait}, waitingFor{nullptr}, name{ n }, 
			flag{ f }, icon{i}, waitingIcon{wi}{
		tolerance = 60*10;
	 }
	
	Task(GameObject* loc, void(*func)(void*), void* d, int prio, int wait,
					const char* n, int f, CharacterObject* cobj) :
			location{ loc }, function{ func }, data{ d }, priority{ prio },
			actualPrio{ 0 }, waitTime{ wait }, waitingFor{ cobj }, name{ n },
									flag{ f } {
		icon = nullptr;
		tolerance = 60*10;
	}

	Task(GameObject* loc, void(*func)(void*), void* d, int prio, int wait,
		const char* n, int f, CharacterObject* cobj, Image *i, Image *wi) :
			location{ loc }, function{ func }, data{ d }, priority{ prio },
			actualPrio{ 0 }, waitTime{ wait }, waitingFor{ cobj }, name{ n },
						flag{ f }, icon{i}, waitingIcon(wi) {
		tolerance = 60*10;
	}

	~Task() {
		data = nullptr;
		waitingIcon = nullptr;
		printf("Data for a task has been freed!\n");
	}
};

struct affectionTrait{
	char *name;
	char **genders;
	int n;
};

enum Sex{
	female,
	male,
	intersex
};

enum Role {
	captain,
	pilot,
	doctor,
	engineer,
	biologist,
	generalist
};

struct relationEventChances {
	//The default chances for each event, just arbitary numbers
	//int falloutChance = 10; //Both of you count to ten before you do anything irrational.
	int falloutChance = 10; //Both of you count to ten before you do anything irrational.
	int confessionChance = 16; // 14+2 valentines day.
	int cheatingChance = 69; //Nice, gotta get that lay
	int birthdayChance = 12; //The year I stopped having birthday parties.
	int cuddleChance = 33; //Spooning
	int supportChance = 29; //0+8+1+2+3+4+9+2+0+0
	int noChance = 999; //1/1000 chance for each character.
};

//The traits a character have, see doc
#define BIGOT (1 << 0)
#define CARING (1 << 1)
#define PARANOID (1 << 2)
#define TRUSTING (1 << 3)
#define SENSATIVE (1 << 4)
#define LIER (1 << 5) //Increases the chances for birthdays and cheating

struct CharacterObject : GameObjClick{
	//When we delete em
	int objsIndex;

	int stress;
	int loyalty;

	const char *name;
	Image *nameImage;

	Sex sex;
	char *gender;
	affectionTrait *romance;
	affectionTrait *sexuality;

	Role role;

	//Tasks
	std::list<Task*> tasks;
	Task* currentTask = nullptr;
	Image *taskIcon;

	int actionTime; //Indicates if an action is being done (-1 is none, 0 means done)

	int traitFlags; //The traits they have.

	relationEventChances rec; //The chance *this* characters has for each event.
	bool dating = false; //If they are currently dating anyone.

	//Pathfinding
	std::vector<GameObject *> *path;
	GameObject *goal;
	GameObject *target;
	int xDist;
	int yDist;
	int speed;

	CharacterObject(int xPos, int yPos, Image *img, void(*func)(void*), void* d,
				const char *n, Sex s, char *g, affectionTrait *r,
					affectionTrait *se, Role roly, int traits,
					Font *f, SDL_Renderer *render) :
						 GameObjClick(xPos, yPos, img, func, d){

		setParam(n, s, g, r, se, roly, traits, f, render);
	}
	CharacterObject(int xPos, int yPos, Image *img, SDL_Rect ar, void(*func)(void*),
				void* d, const char *n, Sex s, char *g,
				affectionTrait *r, affectionTrait *se, Role roly,
				int traits, Font *f, SDL_Renderer *render) :
					 GameObjClick(xPos, yPos, img, ar, func, d){

		setParam(n, s, g, r, se, roly, traits, f, render);
	}
	
	void setParam(const char *n, Sex s, char *g, affectionTrait *r,
					affectionTrait *se, Role roly, int traits,
						Font *f, SDL_Renderer *render){
		name = n;
		sex = s;
		gender = g;
		romance =  r;
		sexuality = se;

		stress = 0;
		loyalty = 100;
    
    		path = nullptr;
		goal = nullptr;
		target = nullptr;
		xDist = 0;
		yDist = 0;
		speed = 4;

		role = roly;
		traitFlags = traits;
		taskIcon = nullptr;

		nameImage = new Image(f, name, {0,0,0}, render);	

		actionTime = -1;
	}


	void makeActualPriority(int &val, int flags) {
		val *= flags & AIASSIGNED ? loyalty / 100 : 1;
		val += ((flags & EMERGENCY) != 0) * 100
			+ ((flags & (FORFRIENDS | FORHATE | FORLOVE | AGAINSTENEMY)) != 0) * 69
			+ ((flags & (FORENEMIES | AGAINSTLOVE | AGAINSTFRIENDS)) != 0) * -96;
	}
	struct compGreater {
		Task* last = nullptr;
		bool operator()(const Task* l, const Task* r) {
			if (l == last) return l->actualPrio + 0000 < r->actualPrio;
			if (r == last) return l->actualPrio < r->actualPrio + 0000;
			return l->actualPrio < r->actualPrio;
		}
	};

	void changeCurrentTask() {
		if (!tasks.empty()) {
			if (currentTask != tasks.back()) {
				printf("Change of main tasks (%s) for %s\n",
								tasks.back()->name,
										name);
				//printf("(%d, %d)\n", x, y);
				Task* task = tasks.back();
				//printf("\t(%d, %d)\n", task->location->x, task->location->y);
				delete path;
				path = nullptr;
				target = nullptr;
				goal = task->location;
				currentTask = tasks.back();
				taskIcon = currentTask->icon;
			}
		} else {
			goal = nullptr;
			delete path;
			path = nullptr;
			currentTask = nullptr;
			taskIcon = nullptr;
			target = nullptr;
		}
	}
	
	void removeTask() {
		printf("\t%s tries to delete %p\n", name, tasks.back());
		delete tasks.back();
		tasks.pop_back();
		printf("\t%s tries to change task\n", name);
		changeCurrentTask();
		printf("\t%s is done with tasks\n", name);
	}

	void removeTask(Task* toDelete) {
		printf("\t%s tries to remove %p\n", name, toDelete);
		tasks.remove(toDelete);
		printf("\t%s tries to delete %p\n", name, toDelete);
		delete toDelete;
		printf("\t%s tries to change task\n", name);
		changeCurrentTask();
		printf("\t%s is done with tasks\n", name);
	}

	void addTask(Task* task) {
		task->actualPrio = task->priority;
		makeActualPriority(task->actualPrio, task->flag);

		tasks.push_back(task);
		compGreater cg{ currentTask };
		tasks.sort(cg);

		changeCurrentTask();
	}

	void rethinkOrder() {
		for (Task* t : tasks) {
			t->actualPrio = t->priority;
			makeActualPriority(t->actualPrio, t->flag);
		}

		tasks.sort(compGreater());
		changeCurrentTask();
	}

	~CharacterObject() {
		for(Task* t : tasks) {
			delete t;
		}
		//printf("Deleted a task, for there is no need to do anything when you are heading to the void!\n");
		delete nameImage;
	}

};

/*
	Door struct for doors
*/
struct Door : GameObjClick {
	bool IsLocked = false;
	bool IsOpen = false;
	GameObject *bottom; //TODO: Temp fix for ladders, could be revised
	std::vector<Door *> doors;

	Door(int xPos, int yPos, Image *img, void(*func)(void*), void* d)
		: GameObjClick(xPos, yPos, img, func, d) {
			bottom = nullptr;
		}
};

#define STORAGE (1 << 0)
#define MEDBAY (1 << 1)
#define BEDROOM (1 << 2)
#define HALL (1 << 3)
#define PRIVATE (1 << 4)
#define TOILET (1 << 5)
#define KITCHEN (1 << 6)
#define AICORE (1 << 7)
#define BRIDGE (1 << 8)
#define CLEARLYFATAL (1 << 9)
#define CREWQUARTERS (1 << 10)
#define LUNCHROOM (1 << 11)
#define RECREATIONALCENTER (1 << 12)
struct Room : GameObjClick {
	int flag;
	const char* name;
	std::vector<GameObjClick*> buttons;
	std::vector<Door *> doors;

	Room(int xPos, int yPos, Image *img, void(*func)(void*), void* d, int f)
		: flag{ f }, GameObjClick(xPos, yPos, img, func, d) {
	}

	Room(int xPos, int yPos, Image *img, SDL_Rect ar, void(*func)(void*), void* d, int f)
		: flag{ f }, GameObjClick(xPos, yPos, img, ar, func, d) {

	}
};

enum Relation{
	Neutral,
	On_Bad_Terms,
	Friends,
	Dating
};
