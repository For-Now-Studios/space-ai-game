#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include<fstream>
#include "structs.h"

using namespace std;

// Sound.cpp
void resumeMusic();
void pauseMusic();
void stopMusic(int fadeTime);
void switchMusic(Music *music, int loops, int fadeInTime, int fadeOutTime);
void switchMusic(Music *music, int loops);
void switchMusic(Music *music);
void resumeAllSound();
void pauseAllSound();
void resumeSound(int channel);
void pauseSound(int channel);
void resumeAll();
void pauseAll();
void stopSound(int channel, int fadeTime);
int playSound(Sound *sound, int loops, double volume, int fadeTime);
int playSound(Sound *sound, int loops, double volume);
int playSound(Sound *sound, int loops);
int playSound(Sound *sound);

// Graphics.cpp
void render(WindowStruct *window, Image *image, int x, int y);
void render(WindowStruct *window, Image *image, int x, int y, Camera *cam);
void render(WindowStruct *window, GameObject *obj, Camera *cam);

// Camera.cpp
SDL_Rect translateToGame(const Camera *camera, const SDL_Rect *obj);
SDL_Rect translateToCamera(const Camera *camera, const SDL_Rect *obj);

// IO.cpp
bool fileReadLine(fstream *fs, char *line, int *len, int maxLength);
char **splitString(char *string, int &len, int maxLength);
bool loadMedia(Media *media, const char *path, SDL_Renderer *render);
vector<char *> *loadGender(const char *path);
vector<affectionTrait *> *loadAffectionTrait(const char *path);
