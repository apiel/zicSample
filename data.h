#ifndef _DATA_H_
#define _DATA_H_

#include "def.h"
#include "track.h"

class Data {
private:
    Data() { }

public:
    Track tracks[APP_TRACKS];

    static Data* instance;

    static Data* getInstance()
    {
        if (!instance) {
            instance = new Data();
        }
        return instance;
    }

    void load()
    {
        size_t sz;
        void* loaded = SDL_LoadFile(APP_DATA_MAIN, &sz);
        if (!loaded) {
            APP_LOG("Error: could not load file %s\n", APP_DATA_MAIN);
            return;
        }
        // APP_LOG("DATASIZE: %lu\n\n%s\n\n", sz, (char*)loaded);

        char* rest = (char*)loaded;
        char* line;
        for (uint8_t i = 0; (line = strtok_r(rest, "\n", &rest)) && i < APP_TRACKS; i++) {
            tracks[i].setName(line).load();
        }

        SDL_free(loaded);
    }

    // Track& getTrack(uint8_t index)
    // {
    //     return tracks[index % APP_TRACKS];
    // }
};

Data* Data::instance = NULL;

#endif