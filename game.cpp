#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include<stdlib.h>
#include<stdio.h>
#include "structures.h"

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

void render(WindowStruct *window, SDL_Texture *image){
	SDL_Rect dst = {100, 100, 1200, 1200};

	int error = SDL_RenderCopyEx(window->render, image, NULL, &dst, 0, NULL,
								SDL_FLIP_NONE);
	if(error != 0) printf("Render fault!\n");
}

void render(WindowStruct *window, GameObject *obj, Camera *cam){
	SDL_Rect dst = translateToCamera(cam, obj);

	int error = SDL_RenderCopyEx(window->render, obj->image->image, NULL, &dst, 0, NULL,
								SDL_FLIP_NONE);
	if(error != 0) printf("Render fault!\n");
}

void close(WindowStruct *window){
	SDL_DestroyRenderer(window->render);
	SDL_DestroyWindow(window->window);

	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}

int main(){
	WindowStruct window;

	if(!init(&window)){
		exit(1);
	}

	SDL_Surface *temp = IMG_Load("hal9000.png");
	if(temp == NULL) printf("bruh!\n");
	SDL_Texture *image = SDL_CreateTextureFromSurface(window.render, temp);
	if(image == NULL) printf("omegalul\n");
	SDL_FreeSurface(temp);

	printf("Images loaded!\n");

	GameObject obj;
	obj.image = new Image;
	obj.image->image = image;
	obj.image->width = 120;
	obj.image->height = 120;
	obj.x = 0;
	obj.y = 0;
	GameObject obj2;
	obj2.image = new Image;
	obj2.image->image = image;
	obj2.image->width = 120;
	obj2.image->height = 120;
	obj2.x = 120;
	obj2.y = 0;

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
	while(true){
		SDL_RenderClear(window.render);

		SDL_Event event;
		while(SDL_PollEvent(&event) != 0){
			switch(event.type){
				case SDL_QUIT:
					exit(0);
					break;
				default:
					break;
			}
		}

		render(&window, &obj, &cam);
		render(&window, &obj2, &cam);

		// If we were too quick, wait until it is time
		Uint32 time = SDL_GetTicks() - startTime;
		if(time < targetTime){
			SDL_Delay(targetTime - time);
		}

		startTime = SDL_GetTicks();		

		SDL_RenderPresent(window.render);
	}

	SDL_DestroyTexture(image);

	return 0;
}
