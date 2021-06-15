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

	Image(const char *path, int w, int h, SDL_Renderer *render) : width{ w }, height{ h } {
		SDL_Surface *temp = IMG_Load(path);
		if (temp == NULL) printf("bruh!\n");
		image = SDL_CreateTextureFromSurface(render, temp);
		if (image == NULL) printf("omegalul\n");
		SDL_FreeSurface(temp);
		printf("%s loaded!\n", path);
	}

	~Image() {
		SDL_DestroyTexture(image);
		printf("File deleted!\n");
	}
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
};
