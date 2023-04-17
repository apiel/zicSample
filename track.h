#ifndef _TRACK_H_
#define _TRACK_H_

#include "def.h"

class Track {
public:
    char name[APP_TRACK_NAME];

    void load(char * _name)
    {
        SDL_Log("Track::load %s\n", _name);
    }
};

#endif