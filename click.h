#pragma once

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

struct ClickReciept {
	int index;
	int objs[2];
	CurrentClick *cc;
	vector<GameObject*>* objects;
};

/*
	Builds the clickable areas.
*/
void buildClickAreas(CurrentClick*, vector<IsClickable*>);
/*
	Cleans the memory of the clickable areas.
*/
void cleanClickAreas(CurrentClick*);
/*
	Adds a popup to the game.
	Returns the index.
*/
int addPopup(CurrentClick*, ClickArea*);
/*
	Adds a character and technically anything moving.
*/
void addCharacters(CurrentClick*, IsClickable*);
/*
	Updates all moving clickable things.
*/
void updateClickAreas(CurrentClick*);
/*
	Check if specific area, translated to camera coordinates, in current click if coordinates x & y is within anything clickable.
*/
IsClickable* checkArea(CurrentClick*, int, int, ClickArea*, Camera*);
/*
	Check if specific area in current click if coordinates x & y is within anything clickable.
*/
IsClickable* checkArea(CurrentClick*, int, int, ClickArea*);
/*
	Checks specified coordinates on current clickable area.
	It checks for one item only and check in this order
	1: UI
	2: Popup
	3: Game
	TODO: Have focused popups be on top and test it
*/
IsClickable* checkCord(CurrentClick*, int, int, Camera*);
/*
	Closes specified popup and deletes the objects from the game.
	Send in the reciept for the specific popup to close it.
	It will delete the Clickarea on the heap.
	You will, however, need to yourself delete the GameObjects and IsClickable-objects that was used for the popup.
*/
void closePopup(ClickReciept*);
/*
	Creates specified popup and adds it to the current clickable area.
	1: Vector of IsClickable that will be added to the popup area so they are clickable.
	2: Vector of GameObjects that will be added to the current objects so they are rendered.
	3: The vector of current objects being rendered.
	4: The current clickable area.
	5: The rectangle for the clickable area that represents the popup.
*/
ClickReciept* createPopup(const vector<IsClickable*>&, const vector<GameObject*>&, vector<GameObject*>*, CurrentClick*, const SDL_Rect&);
