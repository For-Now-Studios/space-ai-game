#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include<fstream>
#include<random>
#include "structs.h"
#include "graph.h"

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

// Pathfinding.cpp
Graph<Room *, int> *initPathfinding(vector<Room *> *v);
Room *whichRoom(vector<Room *> *v, GameObject *obj);
vector<GameObject *> *findPathTo(Graph<GameObject *, int> *g, GameObject *from,
								GameObject *to);
void updateMovement(CharacterObject *object, vector<Room *> *rooms,
							Graph<GameObject *, int> *g);

// Relationships.cpp
Graph<CharacterObject *, Relation> *initRelations(vector<CharacterObject *> *chars);
bool validRomance(CharacterObject *a, CharacterObject *b);
bool validSex(CharacterObject *a, CharacterObject *b);

void fallout(vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine);
void cheating(vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine);
void confession(vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine);
void birthday(vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine);
void cuddles(vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine);
void support(vector<CharacterObject*>& characters, CharacterObject* currChar, Graph<CharacterObject*, Relation>& relatonships, default_random_engine);