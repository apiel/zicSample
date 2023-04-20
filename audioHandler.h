#ifndef _AUDIO_HANDLER_H_
#define _AUDIO_HANDLER_H_

#include "data.h"
#include "def.h"
#include "state.h"
#include "tempo.h"

class AudioHandler {
protected:
    Data& data = Data::get();

    float volume = 1.0f;
    float mixDivider = 1.0f / APP_TRACKS;

    AudioHandler() {
        setVolume(volume);
    }

public:
    Tempo& tempo = Tempo::get();

    uint8_t stepCounter = 0;

    static AudioHandler* instance;

    static AudioHandler& get()
    {
        if (!instance) {
            instance = new AudioHandler();
        }
        return *instance;
    }

    void sample(float* buf, int len)
    {
        if (tempo.next(SDL_GetTicks()))
        // if (tempo.next(SDL_GetTicks64()))
        // if (tempo.next())
        {
            stepCounter = (stepCounter + 1) % APP_TRACK_STEPS;
            for (uint8_t i = 0; i < APP_TRACKS; i++) {
                if (data.tracks[i].next(stepCounter)) {
                    // needRender = true;
                }
            }
            needToRenderProgressBar = true;
        }

        for (int j = 0; j < len; j++) {
            buf[j] = 0.0f;
        }

        float* buffer = new float[len];
        for (uint8_t i = 0; i < APP_TRACKS; i++) {
            Track& track = data.tracks[i];
            if (track.active) {
                track.audioFile.sample(buffer, len);
                for (int j = 0; j < len; j++) {
                    buf[j] += track.filter.next(buffer[j]) * mixDivider * track.activeStep->velocity * track.volume;
                }
            }
        }
        delete[] buffer;
    }

    void setVolume(float _volume)
    {
        volume = range(_volume, 0.0, APP_MAX_VOLUME);
        mixDivider = 1.0f / APP_TRACKS * volume;
    }

    float getVolume()
    {
        return volume;
    }
};

AudioHandler* AudioHandler::instance = NULL;

#endif