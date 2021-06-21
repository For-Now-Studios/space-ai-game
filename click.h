#pragma once

//Struct that can be added to make an object clickable.
struct IsClickable {
	SDL_Rect area; //Area that is clickable
	void(*function)(void*); //The function being called when clicked
	void* data = nullptr; //The data sent into the function

	// Destructor to delete the data, as it needs to be put on the heap (I think in most cases)
	virtual ~IsClickable() {
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

/*
	A clickable area with a vector containing things that are clickable.
*/
struct ClickArea {
	SDL_Rect area;
	std::vector<IsClickable*> clicks;
};

/*
	Struct for the object that builds up the clickable objects on the screen.
*/
struct CurrentClick {
	//Number of characters per each divide up part of the game.
	int numChars[2];

	//All clickable characters, though it could be anything moving.
	std::vector<IsClickable*> Characters;
	std::vector<ClickArea*> UI;
	std::vector<ClickArea*> Popup;
	std::vector<ClickArea*> Game;
};

void buildClickAreas(CurrentClick*, vector<IsClickable*>);
void cleanClickAreas(CurrentClick*);
int addPopup(CurrentClick*, ClickArea*);
void closePopup(CurrentClick*, int, vector<GameObject *>*, int, int);
void addCharacters(CurrentClick*, IsClickable*);
void updateClickAreas(CurrentClick*);
IsClickable* checkArea(CurrentClick*, int, int, ClickArea*, Camera*);
IsClickable* checkArea(CurrentClick*, int, int, ClickArea*);
IsClickable* checkCord(CurrentClick*, int, int, Camera*);