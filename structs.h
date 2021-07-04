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
};

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

struct Task {
	// Location
	int x;
	int y;

	// Effect
	void(*function)(void*); //The function that does its effect
	void* data = nullptr; //The data sent into the function
	
	int priority; //How important is this job?
	int actualPrio; //How ACTUALLY important is this?
	int waitTime;

	const char* name;

	int flag;

	~Task() {
		delete data;
		data = nullptr;
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

struct CharacterObject : GameObjClick{
	int stress;
	int loyalty;
	const char *name;
	Sex sex;
	char *gender;
	affectionTrait *romance;
	affectionTrait *sexuality;
	Role role;
	std::list<Task*> tasks;

	//Pathfinding
	std::vector<GameObject *> *path;
	GameObject *goal;
	GameObject *target;
	int xDist;
	int yDist;
	int speed;

	CharacterObject(int xPos, int yPos, Image *img, void(*func)(void*), void* d,
		const char *n, Sex s, char *g, affectionTrait *r, affectionTrait *se, Role roly) :
						 GameObjClick(xPos, yPos, img, func, d){
		setParam(n, s, g, r, se, roly);
	}
	CharacterObject(int xPos, int yPos, Image *img, SDL_Rect ar, void(*func)(void*),
	void* d, const char *n, Sex s, char *g, affectionTrait *r, affectionTrait *se, Role roly) :
					 GameObjClick(xPos, yPos, img, ar, func, d){
		setParam(n, s, g, r, se, roly);
	}
	
	void setParam(const char *n, Sex s, char *g, affectionTrait *r,
						affectionTrait *se, Role roly){
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
		speed = 1;

		role = roly;
	}


	void makeActualPriority(int &val, int flags)
	{
		val *= flags & AIASSIGNED ? loyalty / 100 : 1;
		val += ((flags & EMERGENCY) != 0) * 100
			+ ((flags & (FORFRIENDS | FORHATE | FORLOVE | AGAINSTENEMY)) != 0) * 69
			+ ((flags & (FORENEMIES | AGAINSTLOVE | AGAINSTFRIENDS)) != 0) * -96;
	}
	struct compGreater {
		bool operator()(const Task* l, const Task* r) {
			return l->actualPrio < r->actualPrio;
		}
	};
	

	void addTask(Task* task) {
		task->actualPrio = task->priority;
		makeActualPriority(task->actualPrio, task->flag);
		tasks.push_back(task);
		tasks.sort(compGreater());
	}

	//Currently calculates actual priority each time it compares
	void rethinkOrder() {
		for (Task* t : tasks) {
			t->actualPrio = t->priority;
			makeActualPriority(t->actualPrio, t->flag);
		}
		tasks.sort(compGreater());
	}

	~CharacterObject() {
		for(Task* t : tasks) {
			delete t;
		}
		printf("Deleted a task, for there is no need to do anything when you are heading to the void!\n");
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
