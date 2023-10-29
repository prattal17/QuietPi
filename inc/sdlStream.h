#ifndef SDL_STREAM
#define SDL_STREAM
#include <SDL2/SDL.h>
#include <mutex>
#include <queue>

class sdlStream
{
    static const int bufferDepth = 100;

    public:
        sdlStream() = default;
        ~sdlStream()
        {
	        SDL_Quit();
        }

        void configure( SDL_AudioDeviceID inputId,
                        SDL_AudioDeviceID outputId, 
                        int sampleRate = 44100, 
                        Uint8 ch = 2, 
                        Uint16 samples = 4096, 
                        SDL_AudioCallback inputCallback,
                        SDL_AudioCallback outputCallback)
        {
            // configure the audio specifications
            SDL_zero(desiredSpec);
            desiredSpec.freq = sampleRate;
            desiredSpec.format = AUDIO_S16;
            desiredSpec.channels = ch;
            desiredSpec.samples = samples;

            // output spec same as input spec
            outputSpec = desiredSpec;

            // configure the respective callbacks
            // these callbacks will need to send data to our receiver functions still
            desiredSpec.callback = inputCallback;
            outputSpec.callback = outputCallback;

            SDL_OpenAudioDevice(
                SDL_GetAudioDeviceName(inputId, SDL_TRUE),  // device name
                SDL_TRUE,                                   // is capture
                &desiredSpec,                               // desired audio format
                &inputSpec,                                 // audio format SDL decides to give you
                SDL_AUDIO_ALLOW_FORMAT_CHANGE);             // Is SDL allowed to change the format?
                
            SDL_OpenAudioDevice(
                SDL_GetAudioDeviceName(outputId, SDL_FALSE),// device name
                SDL_FALSE,                                  // is capture
                &desiredSpec,                               // desired audio format
                &outputSpec,                                // audio format SDL decides to give you
                SDL_AUDIO_ALLOW_FORMAT_CHANGE);             // Is SDL allowed to change the format?

        }

        void run()
        {
            // all the pointers and such should be configured before calling this
            // unpause each stream
            for (auto id : {inputId, outputId})
            {
                SDL_PauseAudioDevice(id, SDL_FALSE);
                SDL_LockAudioDevice(id);
            }
        }

        void stop()
        {
            for (auto id : {inputId, outputId})
            {
                SDL_PauseAudioDevice(id, SDL_TRUE);
                SDL_LockAudioDevice(id);
            }
        }

    private:
        SDL_AudioDeviceID inputId{}, outputId{};
        SDL_AudioSpec desiredSpec, inputSpec, outputSpec;
        std::mutex inBuffLock, outBuffLock;
        std::queue<std::array<Uint8, bufferDepth>> iframes, oframes;
};

#endif