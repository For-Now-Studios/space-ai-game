#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

struct WindowStruct {
	SDL_Window *window;
	SDL_Texture *screenTexture;
	SDL_Renderer *render;
};

struct Image {
	SDL_Texture *image;
	int width;
	int height;
};

struct GameObject {
	//Game Coordinates
	int x;
	int y;

	//Images
	Image *image;
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
	int width;
	int height;
};

struct Point {
	int x;
	int y;
};
