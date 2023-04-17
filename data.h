#ifndef _DATA_H_
#define _DATA_H_

#include "def.h"

class Data {
private:
    Data() { }

public:
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
        APP_LOG("DATASIZE: %lu\n\n%s\n\n", sz, (char*)loaded);

        SDL_free(loaded);
    }
};

Data* Data::instance = NULL;

#endif