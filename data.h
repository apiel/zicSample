#ifndef _DATA_H_
#define _DATA_H_

#include "def.h"
#include "tempo.h"
#include "track.h"

class Data {
protected:
    Tempo& tempo = Tempo::get();

    Data() { }

public:
    Track tracks[APP_TRACKS];

    static Data* instance;

    static Data& get()
    {
        if (!instance) {
            instance = new Data();
        }
        return *instance;
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

    void save()
    {
        FILE* file = fopen(APP_DATA_MAIN, "w");
        if (!file) {
            APP_LOG("Error: could not open file %s\n", APP_DATA_MAIN);
            return;
        }
        APP_LOG("Save master data...\n");
        for (uint8_t i = 0; i < APP_TRACKS; i++) {
            fprintf(file, "%s\n", tracks[i].name);
        }

        // volume  bpm resonance
        fprintf(file, "\n%f %d %f\n", 1.0f, tempo.getBpm(), 0.0f);

        fclose(file);
    }

    void saveAll()
    {
        APP_LOG("Save all data...\n");
        for (uint8_t i = 0; i < APP_TRACKS; i++) {
            tracks[i].save();
        }
        save();
    }
};

Data* Data::instance = NULL;

#endif