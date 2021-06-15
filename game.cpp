#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include "structs.h"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/*
	Translates the coordinates of a GameObject to coordinates relative to the Camera
*/
SDL_Rect translateToCamera(Camera *camera, GameObject *obj){
	SDL_Rect rect = {camera->wndX + (int)((obj->x - camera->x) * camera->zoomLevel),
			 camera->wndY + (int)((obj->y - camera->y) * camera->zoomLevel),
			 (int)(obj->image->width * camera->zoomLevel),
			 (int)(obj->image->height * camera->zoomLevel)};

	return rect;
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
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());

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

/*
	Render the specified image or game object to the screen
*/
void render(WindowStruct *window, Image *image, int x, int y){
	SDL_Rect dst = {x, y, image->width, image->height};

	SDL_RenderCopyEx(window->render, image->image, NULL, &dst, 0, NULL,
									SDL_FLIP_NONE);
}
void render(WindowStruct *window, Image *image, int x, int y, Camera *cam){
	GameObject obj;
	obj.x = x;
	obj.y = y;
	obj.image = image;

	SDL_Rect dst = translateToCamera(cam, &obj);

	SDL_RenderCopyEx(window->render, image->image, NULL, &dst, 0, NULL,
									SDL_FLIP_NONE);
}
void render(WindowStruct *window, GameObject *obj, Camera *cam){
	SDL_Rect dst = translateToCamera(cam, obj);

	SDL_RenderCopyEx(window->render, obj->image->image, NULL, &dst,
								0, NULL, SDL_FLIP_NONE);
}

void close(WindowStruct *window, vector<Image*>& images){
	for (int i = 0; i < images.size(); i++)
	{
		images[i]->~Image();
	}

	SDL_DestroyRenderer(window->render);
	SDL_DestroyWindow(window->window);

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

	Image *img = new Image("hal9000.png", 120, 120, window.render);

	vector<Image*> images;
	images.push_back(img);

	vector<GameObject> objects;

	GameObject obj;
	obj.image = img;
	obj.x = 0;
	obj.y = 0;
	GameObject obj2;
	obj2.image = img;
	obj2.x = 120;
	obj2.y = 0;
	objects.push_back(obj);
	objects.push_back(obj2);

	printf("Game object done!\n");

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

	// The Loop
	bool running = true;
	while(running){
		SDL_RenderClear(window.render);

		SDL_Event event;
		while(SDL_PollEvent(&event) != 0){
			switch(event.type){
				case SDL_QUIT:
					running = false;
					break;
				default:
					break;
			}
		}

		render(&window, img, 0, 120);
		render(&window, img, 120, 120, &cam);
		
		for(GameObject obj : objects){
			render(&window, &obj, &cam);
		}

		// If we were too quick, wait until it is time
		Uint32 time = SDL_GetTicks() - startTime;
		if(time < targetTime){
			SDL_Delay(targetTime - time);
		}

		startTime = SDL_GetTicks();		

		SDL_RenderPresent(window.render);
	}

	close(&window, images);

	return 0;
}
