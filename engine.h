#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include "structs.h"

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

// Camera.cpp
SDL_Rect translateToGame(const Camera *camera, const SDL_Rect *obj);
SDL_Rect translateToCamera(const Camera *camera, const SDL_Rect *obj);
