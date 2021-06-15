#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

struct WindowStruct {
	SDL_Window *window;
	SDL_Texture *screenTexture;
	SDL_Renderer *render;
};

struct GameObject {
	//Game Coordinates
	int x;
	int y;

	//TODO: Replace with the correct image wraper
	SDL_Texture *image;
};
