#include "structs.h"
#include "globals.h"

/*
	Translates the given rect from camera coordinates to game coordinates
*/
SDL_Rect translateToGame(const Camera *camera, const SDL_Rect *obj){
	SDL_Rect rect = {camera->x + (int)((obj->x - camera->wndX) / camera->zoomLevel),
			 camera->y + (int)((obj->y - camera->wndY) / camera->zoomLevel),
			 (int)(obj->w / camera->zoomLevel),
			 (int)(obj->h / camera->zoomLevel)};

	return rect;
}

/*
	Translates the given rect from game coordinates to camera coordinates
*/
SDL_Rect translateToCamera(const Camera *camera, const SDL_Rect *obj){
	SDL_Rect rect = {camera->wndX + (int)((obj->x - camera->x) * camera->zoomLevel),
			 camera->wndY + (int)((obj->y - camera->y) * camera->zoomLevel),
			 (int)(obj->w * camera->zoomLevel),
			 (int)(obj->h * camera->zoomLevel)};

	return rect;
}
