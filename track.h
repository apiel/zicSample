#ifndef _TRACK_H_
#define _TRACK_H_

#include "audioFile.h"
#include "def.h"

const char* STEP_CONDITIONS[] = {
    "---",
    "Pair",
    "4th",
    "6th",
    "8th",
    "Impair",
    "1%",
    "2%",
    "5%",
    "10%",
    "20%",
    "30%",
    "40%",
    "50%",
    "60%",
    "70%",
    "80%",
    "90%",
    "95%",
    "98%",
    "99%",
};

uint8_t STEP_CONDITIONS_COUNT = sizeof(STEP_CONDITIONS) / sizeof(STEP_CONDITIONS[0]);

class Step {
public:
    bool enabled = false;
    float velocity = 0;
    uint8_t condition = 0;

    Step& toggle()
    {
        enabled = !enabled;
        return *this;
    }

    Step& setVelocity(float _velocity)
    {
        velocity = range(_velocity, 0.0f, 1.0f);
        return *this;
    }

    Step& setCondition(int8_t _condition)
    {
        condition = range(_condition, 0, STEP_CONDITIONS_COUNT - 1);
        return *this;
    }
};

class Track {
public:
    AudioFile audioFile;
    uint8_t stepCounter = 0;

    char name[APP_TRACK_NAME];
    char sample[APP_SAMPLE_NAME];

    Step steps[APP_TRACK_STEPS];
    Step* activeStep = &steps[0];

    bool active = false;
    bool nextState = false;

    float volume = 1.0f;

    Track& setVolume(float _volume)
    {
        volume = range(_volume, 0.0f, 1.0f);
        return *this;
    }

    Track& setName(char* _name)
    {
        strncpy(name, _name, APP_TRACK_NAME);
        return *this;
    }

    Track& setNextState(bool _nextState)
    {
        nextState = _nextState;
        return *this;
    }

    Track& toggleNextState()
    {
        nextState = !nextState;
        return *this;
    }

    Track& toggle()
    {
        active = !active;
        nextState = active;
        return *this;
    }

    bool next(uint8_t _stepCounter)
    {
        bool ret = false;
        stepCounter = _stepCounter;
        if (stepCounter == 0 && active != nextState) {
            ret = true;
            active = nextState;
        }
        if (active) {
            Step* step = &steps[stepCounter];
            if (step->enabled) {
                audioFile.restart();
                activeStep = step;
            }
        }
        return ret;
    }

    Track& setSample(char* _sample)
    {
        strncpy(sample, _sample, APP_SAMPLE_NAME);
        char filepath[strlen(APP_SAMPLES_FOLDER) + 1 + strlen(sample)];
        sprintf(filepath, "%s/%s", APP_SAMPLES_FOLDER, sample);
        audioFile.open(filepath);
        // SDL_Log("Audio file %s sampleCount %ld\n", filepath, (long)audioFile.sfinfo.frames);
        return *this;
    }

    Track& load()
    {
        // SDL_Log("Track::load %s\n", name);
        if (name[0] != '-') {
            size_t sz;
            char filepath[strlen(APP_DATA_FOLDER) + 1 + strlen(name)];
            sprintf(filepath, "%s/%s", APP_DATA_FOLDER, name);
            void* loaded = SDL_LoadFile(filepath, &sz);
            if (!loaded) {
                APP_LOG("Error: could not load file %s\n", filepath);
                return *this;
            }
            // APP_LOG("Track data (%lu):\n%s\n\n", sz, (char*)loaded);

            char* rest = (char*)loaded;
            setSample(strtok_r(rest, "\n", &rest));
            char* line = strtok_r(rest, "\n", &rest);
            // TODO parse track params
            for (uint8_t i = 0; (line = strtok_r(rest, "\n", &rest)) && i < APP_TRACK_STEPS; i++) {
                Step& step = steps[i];
                // step.enabled = atoi(strtok(line, " "));
                step.enabled = strtok(line, " ")[0] == '1';
                step.velocity = atof(strtok(NULL, " "));
                step.condition = atoi(strtok(NULL, " "));
                // SDL_Log("- enable: %s velocity: %f, condition %i\n", step.enabled ? "on" : "off", step.velocity, step.condition);
            }
            SDL_free(loaded);
        }
        return *this;
    }
};

#endif