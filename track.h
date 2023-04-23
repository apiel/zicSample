#ifndef _TRACK_H_
#define _TRACK_H_

#include "audioFile.h"
#include "def.h"
#include "distortion.h"
#include "filter.h"
#include "fs.h"

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

enum {
    STEP_PLAYING,
    STEP_EVERY_PAIR,
    STEP_EVERY_FOURTH,
    STEP_EVERY_SIXTH,
    STEP_EVERY_EIGHTH,
    STEP_EVERY_IMPAIR,
    STEP_1_PERCENT,
    STEP_2_PERCENT,
    STEP_5_PERCENT,
    STEP_10_PERCENT,
    STEP_20_PERCENT,
    STEP_30_PERCENT,
    STEP_40_PERCENT,
    STEP_50_PERCENT,
    STEP_60_PERCENT,
    STEP_70_PERCENT,
    STEP_80_PERCENT,
    STEP_90_PERCENT,
    STEP_95_PERCENT,
    STEP_98_PERCENT,
    STEP_99_PERCENT,
    STEP_COUNT,
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

    bool conditionMet(uint8_t loopCounter)
    {
        switch (condition) {
        case STEP_EVERY_PAIR:
            return loopCounter % 2 == 0;

        case STEP_EVERY_FOURTH:
            return loopCounter % 4 == 0;

        case STEP_EVERY_SIXTH:
            return loopCounter % 6 == 0;

        case STEP_EVERY_EIGHTH:
            return loopCounter % 8 == 0;

        case STEP_EVERY_IMPAIR:
            return loopCounter % 2 == 1;

        case STEP_1_PERCENT:
            return (rand() % 100) == 0;

        case STEP_2_PERCENT:
            return (rand() % 100) < 2;

        case STEP_5_PERCENT:
            return (rand() % 100) < 5;

        case STEP_10_PERCENT:
            return (rand() % 100) < 10;

        case STEP_20_PERCENT:
            return (rand() % 100) < 20;

        case STEP_30_PERCENT:
            return (rand() % 100) < 30;

        case STEP_40_PERCENT:
            return (rand() % 100) < 40;

        case STEP_50_PERCENT:
            return (rand() % 100) < 50;

        case STEP_60_PERCENT:
            return (rand() % 100) < 60;

        case STEP_70_PERCENT:
            return (rand() % 100) < 70;

        case STEP_80_PERCENT:
            return (rand() % 100) < 80;

        case STEP_90_PERCENT:
            return (rand() % 100) < 90;

        case STEP_95_PERCENT:
            return (rand() % 100) < 95;

        case STEP_98_PERCENT:
            return (rand() % 100) < 98;

        case STEP_99_PERCENT:
            return (rand() % 100) < 99;
        }
        return true;
    }
};

class Track {
protected:
    char filepath[strlen(APP_DATA_FOLDER) + 1 + APP_TRACK_NAME];

    void openAudioFileName()
    {
        char filepath[strlen(APP_SAMPLES_FOLDER) + 1 + strlen(audioFileName)];
        sprintf(filepath, "%s/%s", APP_SAMPLES_FOLDER, audioFileName);
        audioFile.open(filepath);
    }

public:
    Filter filter;
    Distortion distortion;
    AudioFile audioFile;
    uint8_t stepCounter = 0;
    uint8_t loopCounter = 0;

    char name[APP_TRACK_NAME];
    char audioFileName[APP_SAMPLE_NAME];

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
        if (stepCounter == 0) {
            if (active != nextState) {
                ret = true;
                active = nextState;
            }
            loopCounter++;
        }
        if (active) {
            Step* step = &steps[stepCounter];
            if (step->enabled && step->conditionMet(loopCounter)) {
                audioFile.restart();
                activeStep = step;
            }
        }
        return ret;
    }

    float sampleModulation(float buf, float mixDivider)
    {
        return distortion.sample(filter.sample(buf)) * mixDivider * activeStep->velocity * volume;
    }

    Track& setAudioFileName(char* name)
    {
        strncpy(audioFileName, name, APP_SAMPLE_NAME);
        openAudioFileName();
        return *this;
    }

    Track& setNextAudioFileName(int8_t direction = 0)
    {
        // TODO if direction > 1 then should jump to next letter
        direction = range(direction, -1, 1);
        nextFile(audioFileName, APP_SAMPLES_FOLDER, audioFileName, direction);
        openAudioFileName();
        return *this;
    }

    char* getFilePath()
    {
        sprintf(filepath, "%s/%s", APP_DATA_FOLDER, name);
        return filepath;
    }

    Track& load()
    {
        // SDL_Log("Track::load %s\n", name);
        if (name[0] != '-') {
            size_t sz;
            void* loaded = SDL_LoadFile(getFilePath(), &sz);
            if (!loaded) {
                APP_LOG("Error: could not load file %s\n", getFilePath());
                return *this;
            }
            // APP_LOG("Track data (%lu):\n%s\n\n", sz, (char*)loaded);

            char* rest = (char*)loaded;
            setAudioFileName(strtok_r(rest, "\n", &rest));
            char* line = strtok_r(rest, "\n", &rest);
            volume = atof(strtok(line, " "));
            filter.set(atoi(strtok(NULL, " ")));
            filter.setResonance(atof(strtok(NULL, " ")));

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

    Track& save()
    {
        // SDL_Log("Track::save %s\n", name);
        if (name[0] != '-') {
            FILE* file = fopen(getFilePath(), "w");
            if (!file) {
                APP_LOG("Error: could not open file %s\n", getFilePath());
                return *this;
            }
            fprintf(file, "%s\n", audioFileName);
            fprintf(file, "%.2f %i %.2f\n\n", volume, filter.value, filter.resonance);
            for (uint8_t i = 0; i < APP_TRACK_STEPS; i++) {
                Step& step = steps[i];
                fprintf(file, "%i %.2f %i\n", step.enabled, step.velocity, step.condition);
            }
            fclose(file);
        }
        return *this;
    }
};

#endif