#ifndef _TRACK_H_
#define _TRACK_H_

#include "def.h"

class Track {
public:
    char name[APP_TRACK_NAME - 1];

    void load(char* _name)
    {
        SDL_Log("Track::load %s\n", _name);
        strncpy(name, _name, APP_TRACK_NAME);
        if (_name[0] != '-') {
            size_t sz;
            char filepath[strlen(APP_DATA_FOLDER) + 1 + strlen(_name)];
            sprintf(filepath, "%s/%s", APP_DATA_FOLDER, _name);
            void* loaded = SDL_LoadFile(filepath, &sz);
            if (!loaded) {
                APP_LOG("Error: could not load file %s\n", filepath);
                return;
            }
            APP_LOG("Track data (%lu):\n%s\n\n", sz, (char*)loaded);
            // TODO: parse track data
            SDL_free(loaded);
        }
    }
};

#endif