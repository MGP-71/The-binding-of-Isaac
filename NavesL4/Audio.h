#pragma once
#include <iostream>
#include <string> 
using namespace std;

#include <SDL.h>;
#include <SDL_mixer.h> // libreria de audio

class Audio
{
public:
	Audio(string filename, bool loop);
	~Audio(); //destructor
	void play();
	bool loop;
	Mix_Music* mix; // Lib mixer
	SDL_AudioSpec wavSpec; // Lib SDL Standard
	Uint32 wavLength;
	Uint8* wavBuffer;
	SDL_AudioDeviceID deviceId;
};


