#include "structs.h"
#include "globals.h"

int masterVolume = MIX_MAX_VOLUME / 2; //The baseline volume

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
