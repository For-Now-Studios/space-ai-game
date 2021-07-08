#include "structs.h"
#include "engine.h"
#include "globals.h"

/*
	Render the specified image or game object to the screen
*/
void render(WindowStruct *window, Image *image, int x, int y){
	SDL_Rect dst = {x, y, image->width, image->height};

	SDL_RenderCopyEx(window->render, image->image, NULL, &dst, 0, NULL,
									SDL_FLIP_NONE);
}
void render(WindowStruct *window, Image *image, int x, int y, Camera *cam){
	SDL_Rect obj = {x, y, image->width, image->height};

	SDL_Rect dst = translateToCamera(cam, &obj);

	SDL_RenderCopyEx(window->render, image->image, NULL, &dst, 0, NULL,
									SDL_FLIP_NONE);
}
void render(WindowStruct *window, GameObject *obj){
	SDL_Rect dst = {obj->x, obj->y, obj->image->width, obj->image->height};

	SDL_RenderCopyEx(window->render, obj->image->image, NULL, &dst,
								0, NULL, SDL_FLIP_NONE);
}
void render(WindowStruct *window, GameObject *obj, Camera *cam){
	SDL_Rect rect = {obj->x, obj->y, obj->image->width, obj->image->height};

	SDL_Rect dst = translateToCamera(cam, &rect);

	SDL_RenderCopyEx(window->render, obj->image->image, NULL, &dst,
								0, NULL, SDL_FLIP_NONE);
}
