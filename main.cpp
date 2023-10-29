#include "sdlStream.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

Uint8* siBuff;
Uint8* soBuff;
void audioRecordingCallback(void* userdata, Uint8* stream, int len )
{
	//Copy audio from stream
	std::memcpy(&siBuff[ gBufferBytePosition ], stream, len);

	//Move along buffer
	gBufferBytePosition += len;
}

void audioPlaybackCallback(void* userdata, Uint8* stream, int len )
{
	//Copy audio to stream
	std::memcpy(stream, &soBuff[ gBufferBytePosition ], len);

	//Move along buffer
	gBufferBytePosition += len;
}

int main()
{
	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return 1;
	}

	// let user choose which audio devices to use
	int inputId{}, outputId{};
	int audioDeviceCount = SDL_GetNumAudioDevices(SDL_TRUE);
	if(audioDeviceCount < 1)
	{
		printf( "Unable to get audio capture device! SDL Error: %s\n", SDL_GetError() );
		return 0;
	}
	for(int i = 0; i < audioDeviceCount; ++i)
	{
		//Get capture device name
		const char* deviceName = SDL_GetAudioDeviceName(i, SDL_TRUE);
		printf("%d - %s\n", i, deviceName);
	}
	printf("Choose input audio device: \n");
	scanf("%d\n", &inputId);

	for(int i = 0; i < audioDeviceCount; ++i)
	{
		//Get playboy device name
		const char* deviceName = SDL_GetAudioDeviceName(i, SDL_FALSE);
		printf("%d - %s\n", i, deviceName);
	}
	printf("Choose output audio device: \n");
	scanf("%d\n", &outputId);

	return 0;
}
