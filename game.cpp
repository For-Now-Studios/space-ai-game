#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<deque>
#include<fstream>
#include<string.h>
#include "structs.h"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int masterVolume = MIX_MAX_VOLUME / 2; //The volume level everything else is scaled too

/*
	Test funtion and struct parameter.
*/
struct btnHelloParameter { const char* name; };
void btnHello(void *cntxt)
{
	btnHelloParameter *parameters = (btnHelloParameter*)cntxt;
	printf("Hello %s!\n", parameters->name);
}

void buildClickAreas(CurrentClick *cc, vector<IsClickable*> clickable)
{

}

void addPopup(CurrentClick *cc)
{

}

void updateClickAreas(CurrentClick *cc)
{

}

IsClickable* checkCord(CurrentClick *cc, int x, int y, MouseStruct mouse)
{
	//bool found = false;
	for (vector<ClickArea>::reverse_iterator it = cc->UI.rbegin(); it != cc->UI.rend(); ++it) {
		if (it->area.x < mouse.x && mouse.x < it->area.x + it->area.w &&
			it->area.y < mouse.y && mouse.y < it->area.y + it->area.h) {
			for (vector<IsClickable*>::reverse_iterator jt = it->clicks.rbegin(); jt != it->clicks.rend(); ++jt) {
				IsClickable* ic = *jt;
				if (ic->area.x < mouse.x && mouse.x < ic->area.x + ic->area.w &&
					ic->area.y < mouse.y && mouse.y < ic->area.y + ic->area.h) {
					return ic;
					break;
				}
			}
			break;
		}
	}

	for (vector<ClickArea>::reverse_iterator it = cc->Popup.rbegin(); it != cc->Popup.rend(); ++it) {
		if (it->area.x < mouse.x && mouse.x < it->area.x + it->area.w &&
			it->area.y < mouse.y && mouse.y < it->area.y + it->area.h) {
			for (vector<IsClickable*>::reverse_iterator jt = it->clicks.rbegin(); jt != it->clicks.rend(); ++jt) {
				IsClickable* ic = *jt;
				if (ic->area.x < mouse.x && mouse.x < ic->area.x + ic->area.w &&
					ic->area.y < mouse.y && mouse.y < ic->area.y + ic->area.h) {
					return ic;
					break;
				}
			}
			break;
		}
	}

	// TODO: Make the coordinates game coordinates.
	for (vector<ClickArea>::reverse_iterator it = cc->Game.rbegin(); it != cc->Game.rend(); ++it) {
		if (it->area.x < mouse.x && mouse.x < it->area.x + it->area.w &&
			it->area.y < mouse.y && mouse.y < it->area.y + it->area.h) {
			for (vector<IsClickable*>::reverse_iterator jt = it->clicks.rbegin(); jt != it->clicks.rend(); ++jt) {
				IsClickable* ic = *jt;
				if (ic->area.x < mouse.x && mouse.x < ic->area.x + ic->area.w &&
					ic->area.y < mouse.y && mouse.y < ic->area.y + ic->area.h) {
					return ic;
					break;
				}
			}
			break;
		}
	}
	return nullptr;
}

/*
	TODO: Maybe make this function load data from a level file?
	
	NOTE: Currently just a collection of used game objects collected in a singular
	space for ease of use
*/
bool loadLevel(vector<GameObject *>* objects, vector<IsClickable *>* clickable,
							Media* media, const char *path){
	GameObject *obj = new GameObject;
	obj->image = media->images.at(0);
	obj->x = 0;
	obj->y = 0;

	GameObject *obj2 = new GameObject;
	obj2->image = media->images.at(0);
	obj2->x = 120;
	obj2->y = 0;

	/*	So we can go through all buttons alter on.
		Also the destuctors are all called when the "button" object gets
		destroyed, which is at the final return statement.*/
	GameObjClick *button = new GameObjClick;
	button->x = 0;
	button->y = 0;
	button->image = media->images.at(0);
	button->area = SDL_Rect{ 0,0,media->images.at(0)->width,
							media->images.at(0)->height};

	button->function = btnHello;
	button->data = (void*)(new btnHelloParameter{ "Alexander, Tim & Jacob!" });

	clickable->push_back(button);

	objects->push_back(obj);
	objects->push_back(obj2);
	objects->push_back(button);
	

	return true;
}

/*
	Read the next line of text from the specified open file stream.
	At max maxLength text can be read, but the actual size of the line is stored
	in len.

	If the line was properly read, or if the EOF was encountered, the
	function returns true. Otherwise it will return false and report that an error
	had occured
*/
bool fileReadLine(fstream *fs, char *line, int *len, int maxLength){
	fs->getline(line, maxLength);
	*len = fs->gcount();
	if(fs->eof()){
		return true;
	}
	else if(fs->fail()){
		printf("Error reading in text with legth %d!\n", len);
		return false;
	}

	return true;
}

/*
	Takes the given string and splits it around the delimiter " ".
	Returns an array of len substrings, where each substring can at most be
	maxLength long.

	NOTE: String will be truncated to only containe the first substring, and
	the array of substrings (consisting of all substrings) will have to be
	deallocated at some point
*/
char **splitString(char *string, int &len, int maxLength){
	len = 0;

	vector<char *> temp;
	temp.push_back(strtok(string, " "));
	while(temp.at(len) != NULL){
		len += 1;
		temp.push_back(strtok(NULL, " "));
	}
	
	char **result = new char *[len];
	for(int i = 0; i < len; i++){
		result[i] = new char[maxLength];
		strncpy(result[i], temp.at(i), maxLength);
	}
	
	return result;
}

/*
	Initializes the specified media object by loading all the media which filepath
	is specified in the specified manifest file. Returns true if all media was
	loaded correctly, or false if an error was encountered
*/
bool loadMedia(Media *media, const char *path, SDL_Renderer *render){
	const int MAX_FILE_NAME_LENGTH = 1024;

	bool result = true;

	fstream fs;
	fs.open(path, fstream::in);

	char text[MAX_FILE_NAME_LENGTH];
	int mode = 0;
	int len;
	if(!fileReadLine(&fs, text, &len, MAX_FILE_NAME_LENGTH)){
		printf("Error reading file paths from file %s!\n", path);
		result = false;
		len = 0;
	}
	while(len > 0){
		if(mode == 0){
			if(strcmp(text, "IMAGES:") == 0){} //Skip the first line
			else if(strcmp(text, "MUSIC:") == 0) mode++;
			else{
				char **parts = splitString(text, len, len);
				if(len == 3){
					int w = atoi(parts[1]);
					int h = atoi(parts[2]);
					media->images.push_back(new Image(parts[0], w, h,
										render));
				}
				else if(len == 1){
					media->images.push_back(new Image(text, render));
				}
				else{
					printf("Image file %s was badly formated!\n",
										 text);
					result = false;
					break;
				}

				//Cleanup
				for(int i = 0; i < len; i++){
					delete parts[i];
				}				
				delete parts;
			}
		}
		else if(mode == 1){
			if(strcmp(text, "SOUNDS:") == 0) mode++;
			else media->music.push_back(new Music(text));
		}
		else if(mode == 2){
			if(strcmp(text, "FONTS:") == 0) mode++;
			else media->sounds.push_back(new Sound(text));
		}
		else if(mode == 3){
			char **parts = splitString(text, len, len);
			
			if(len != 2){
				printf("Font file %s was badly formated!\n", text);
				result = false;
				break;
			}

			int size = atoi(parts[1]);
			media->fonts.push_back(new Font(text, size));

			//Cleanup
			for(int i = 0; i < len; i++){
				delete parts[i];
			}				
			delete parts;
		}

		if(!fileReadLine(&fs, text, &len, MAX_FILE_NAME_LENGTH)){
			printf("Error reading file paths from file %s!\n", path);
			result = false;
			break;
		}
	}

	fs.close();

	return result;
}

/*
	Resume the current background music
*/
void resumeMusic(){
	Mix_ResumeMusic();
}

/*
	Pause the current background music
*/
void pauseMusic(){
	Mix_PauseMusic();
}

/*
	Stops the current background music, letting it fade out for the specified time
*/
void stopMusic(int fadeTime){
	if(fadeTime > 0) Mix_FadeOutMusic(fadeTime);
	else Mix_HaltMusic();
}

/*
	Switch the current background music to the specified music, letting the old music
	fade out and the new music fade in for the specified time. The new music will
	loop for the specified number of loops, or indefinatley if loops = -1
*/
void switchMusic(Music *music, int loops, int fadeInTime, int fadeOutTime){
	if(fadeOutTime > 0) Mix_FadeOutMusic(fadeOutTime);
	else Mix_HaltMusic();

	if(fadeInTime > 0) Mix_FadeInMusic(music->music, loops, fadeInTime);
	else Mix_PlayMusic(music->music, loops);
}
void switchMusic(Music *music, int loops){
	switchMusic(music, loops, 0, 0);
}
void switchMusic(Music *music){
	switchMusic(music, -1, 0, 0);
}

/*
	Resume all active sound effects
*/
void resumeAllSound(){
	Mix_Resume(-1);
}

/*
	Pause all active sound effects
*/
void pauseAllSound(){
	Mix_Pause(-1);
}

/*
	Resume a given sound
*/
void resumeSound(int channel){
	Mix_Resume(channel);
}

/*
	Pause a given sound
*/
void pauseSound(int channel){
	Mix_Pause(channel);
}

/*
	Resume both sound and music
*/
void resumeAll(){
	resumeAllSound();
	resumeMusic();
}

/*
	Pause both sound and music
*/
void pauseAll(){
	pauseAllSound();
	pauseMusic();
}

/*
	Stops the sound playing on the specified channel.
	If a fade time is specified, the sound will fade out for the specified time
*/
void stopSound(int channel, int fadeTime){
	if(fadeTime > 0){
		Mix_FadeOutChannel(channel, fadeTime);
	}
	else{
		Mix_HaltChannel(channel);
	}
}

/*
	Play a sound with the specified volume, fade in time and loops, returns the used
	channel for the sound
*/
int playSound(Sound *sound, int loops, double volume, int fadeTime){
	int channel;
	if(fadeTime > 0){
		channel = Mix_FadeInChannel(-1, sound->sound, loops, fadeTime);
	}
	else{
		channel = Mix_PlayChannel(-1, sound->sound, loops);
	}

	Mix_Volume(channel, masterVolume * volume);

	return channel;
}
int playSound(Sound *sound, int loops, double volume){
	return playSound(sound, loops, volume, 0);
}
int playSound(Sound *sound, int loops){
	return playSound(sound, loops, 1.0f, 0);
}
int playSound(Sound *sound){
	return playSound(sound, 0, 1.0f, 0);
}

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
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
									TTF_GetError());

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

void close(WindowStruct *window, Media& media, vector<GameObject*>& objects){
	for (Image* img : media.images)
	{
		img->~Image();
	}

	for(Music* m : media.music){
		m->~Music();
	}

	for(Sound* s : media.sounds){
		s->~Sound();
	}

	for(Font* f : media.fonts){
		f->~Font();
	}

	for (GameObject* obj : objects) {
		delete obj;
	}

	SDL_DestroyRenderer(window->render);
	SDL_DestroyWindow(window->window);

	Mix_CloseAudio();

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

	MouseStruct mouse;
	mouse.x = 0;
	mouse.y = 0;
	mouse.scrollRight = 0;
	mouse.scrollUp = 0;
	mouse.buttons[0] = MouseStruct::Button{ false, false, false };
	mouse.buttons[1] = MouseStruct::Button{ false, false, false };
	mouse.buttons[2] = MouseStruct::Button{ false, false, false };

	Media media;
	bool running = loadMedia(&media, "manifest", window.render);

	vector<IsClickable*> clickable;
	vector<GameObject*> objects;

	int channel; //MUSIC TEST
	if(running){
		if(loadLevel(&objects, &clickable, &media, "")){
			printf("Game object done!\n");

			// MUSIC TEST
			Mix_Volume(-1, masterVolume);
			Mix_VolumeMusic(masterVolume);
			switchMusic(media.music.at(0), -1, 0, 60000);
			channel = playSound(media.sounds.at(0), -1, 2.0f, 1000);
		
			media.images.push_back(new Image(media.fonts.at(0),
						"Hello Jacob!", {0,0,0}, window.render));
		}
	}

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
	int ticks = 0;
	while(running){
		SDL_RenderClear(window.render);

		//Need to make these zero or they will get stuck.
		for(MouseStruct::Button& button : mouse.buttons) button.isReleased = 0;
		mouse.scrollRight = 0;
		mouse.scrollUp = 0;

		// Fading out of continous sound test 
		if(ticks == 180){
			pauseAllSound();
		}
		if(ticks == 240){
			resumeAllSound();
		}
		else if(ticks == 360){
			stopSound(channel, 1000);
		}
		else if(ticks == 640){
			switchMusic(media.music.at(1), 0, 1000, 1000);
		}
		else if(ticks == 820){
			pauseMusic();
		}
		else if(ticks == 940){
			resumeMusic();
			playSound(media.sounds.at(0));
		}
		else if(ticks == 1000){
			pauseAll();
		}
		else if(ticks == 1060){
			resumeAllSound();
		}
		else if(ticks == 1180){
			pauseAll();
		}
		else if(ticks == 1300){
			resumeAll();
		}
		ticks++;

		//Start handling events.
		SDL_Event event;
		while(SDL_PollEvent(&event) != 0){
			switch(event.type){
				case SDL_QUIT:
					running = false;
					break;

				case SDL_MOUSEMOTION:
					mouse.x = event.motion.x;
					mouse.y = event.motion.y;
					break;
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
					mouse.x = event.button.x;
					mouse.y = event.button.y;
					/* In SDL, left button = 1, middle = 2, and
					   right = 3. If we subtract this by one we can
					   use our array setup
					*/
					mouse.buttons[event.button.button - 1]
						.doubleClick = event.button.clicks - 1;
					mouse.buttons[event.button.button - 1]
						.isPressed = event.button.state == 
									SDL_PRESSED;
					mouse.buttons[event.button.button - 1]
						.isReleased = event.button.state == 
									SDL_RELEASED;
					break;
				case SDL_MOUSEWHEEL:
					mouse.scrollRight = event.wheel.x;
					mouse.scrollUp = event.wheel.y;
					break;
				default:
					break;
			}
		}

		//Simple button test
		if (mouse.buttons[0].isReleased)
		{
			for (const IsClickable* c : clickable) {
				if (c->area.x < mouse.x && mouse.x < c->area.x + c->area.w &&
					c->area.y < mouse.y && mouse.y < c->area.y + c->area.h) {
					c->function(c->data);
				}
			}
		}

		objects.at(0)->x = mouse.x - objects.at(0)->image->width / 2;
		objects.at(0)->y = mouse.y -  objects.at(0)->image->height / 2;

		render(&window, media.images.at(0), 0, 120);
		render(&window, media.images.at(0), 120, 120, &cam);
		render(&window, media.images.at(1), 120, 300);
		
		for(GameObject* obj : objects){
			render(&window, obj, &cam);
		}

		// If we were too quick, wait until it is time
		Uint32 time = SDL_GetTicks() - startTime;
		if(time < targetTime){
			SDL_Delay(targetTime - time);
		}

		startTime = SDL_GetTicks();		

		SDL_RenderPresent(window.render);
	}

	close(&window, media, objects);

	return 0;
}
