#ifndef _AUDIO_HANDLER_H_
#define _AUDIO_HANDLER_H_

#include "audioPreview.h"
#include "data.h"
#include "def.h"
#include "master.h"
#include "state.h"
#include "tempo.h"

class AudioHandler {
protected:
    Data& data = Data::get();
    AudioPreview& audioPreview = AudioPreview::get();
    Master& master = Master::get();

    unsigned long lastAxisUpdate = 0;
    float lastX2 = 0.0f;
    float lastY2 = 0.0f;

    static AudioHandler* instance;

    AudioHandler() { }

    float getAXisValue(int axis)
    {
        // Round because else we never get 0.0f values...
        return roundf(SDL_JoystickGetAxis(ui.joystick, axis) * 0.00305175f) * 0.01f; // 0.00305175f = 1 / 32768.0f * 100.0f
    }

public:
    Tempo& tempo = Tempo::get();

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
            if (valueX2 != lastX2 || valueY2 != lastY2) {
                lastX2 = valueX2;
                lastY2 = valueY2;
                // filter.set(7250 * valueY2 * valueX2);
                // filter.set(7000 * valueY2 * fabs(valueX2));
                // filter.set(7000 * valueY2 * ((valueX2 + 1) * 0.5f));
                master.filter.set(7000 * (valueY2 * 0.5f + valueY2 * (1 - fabs(valueX2)) * 0.5f));
            }
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
                        buf[j] += track.sampleModulation(buffer[j], master.mixDivider);
                    }
                }
            }
            delete[] buffer;

            for (int j = 0; j < len; j++) {
                buf[j] = master.filter.sample(buf[j]);
            }
        }
    }
};

AudioHandler* AudioHandler::instance = NULL;

#endif