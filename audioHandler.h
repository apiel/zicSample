#ifndef _AUDIO_HANDLER_H_
#define _AUDIO_HANDLER_H_

#include "audioPreview.h"
#include "data.h"
#include "def.h"
#include "filter.h"
#include "state.h"
#include "tempo.h"

class AudioHandler {
protected:
    Data& data = Data::get();
    AudioPreview& audioPreview = AudioPreview::get();

    float volume = 1.0f;
    float mixDivider = 1.0f / APP_TRACKS;

    unsigned long lastAxisUpdate = 0;

    static AudioHandler* instance;

    AudioHandler()
    {
        setVolume(volume);
        filter.setResonance(0.90f);
    }

    float getAXisValue(int axis)
    {
        // Round because else we never get 0.0f values...
        return roundf(SDL_JoystickGetAxis(ui.joystick, axis) * 0.00305175f) * 0.01f; // 0.00305175f = 1 / 32768.0f * 100.0f
    }

public:
    Tempo& tempo = Tempo::get();
    Filter filter;

    uint8_t stepCounter = 0;

    static AudioHandler& get()
    {
        if (!instance) {
            instance = new AudioHandler();
        }
        return *instance;
    }

    void samples(float* buf, int len)
    {
        unsigned long now = SDL_GetTicks();
        if (tempo.next(now))
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

        if (now - lastAxisUpdate > 50) {
            lastAxisUpdate = now;
            // float valueX1 = getAXisValue(UI_PAD_X1);
            // float valueY1 = getAXisValue(UI_PAD_Y1);
            // filter.setResonance(0.95f * valueY1 * valueX1);

            float valueX2 = getAXisValue(UI_PAD_X2);
            float valueY2 = getAXisValue(UI_PAD_Y2);
            // filter.set(7250 * valueY2 * valueX2);
            // filter.set(7000 * valueY2 * fabs(valueX2));
            filter.set(7000 * valueY2 * ((valueX2 + 1) * 0.5f));
        }

        if (!audioPreview.samples(buf, len)) {
            for (int j = 0; j < len; j++) {
                buf[j] = 0.0f;
            }

            float* buffer = new float[len];
            for (uint8_t i = 0; i < APP_TRACKS; i++) {
                Track& track = data.tracks[i];
                if (track.active) {
                    track.audioFile.samples(buffer, len);
                    for (int j = 0; j < len; j++) {
                        // buf[j] += track.filter.sample(buffer[j]) * mixDivider * track.activeStep->velocity * track.volume;
                        buf[j] += track.sampleModulation(buffer[j], mixDivider);
                    }
                }
            }
            delete[] buffer;

            for (int j = 0; j < len; j++) {
                buf[j] = filter.sample(buf[j]);
            }
        }
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