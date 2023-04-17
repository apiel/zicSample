#ifndef _TRACK_H_
#define _TRACK_H_

#include "def.h"

class Step {
public:
    bool enabled = false;
    float velocity = 0;
    uint8_t condition = 0;
};

class Track {
public:
    char name[APP_TRACK_NAME];
    char sample[APP_SAMPLE_NAME];

    Step steps[APP_TRACK_STEPS];

    Track& setName(char* _name)
    {
        strncpy(name, _name, APP_TRACK_NAME);
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
            char* line = strtok_r(rest, "\n", &rest);
            strncpy(sample, line, APP_SAMPLE_NAME);
            line = strtok_r(rest, "\n", &rest);
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