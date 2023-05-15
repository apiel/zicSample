#ifndef _MASTER_H_
#define _MASTER_H_

#include "def.h"
#include "filter.h"

class Master {
protected:
    Master()
    {
        setVolume(volume);
        filter.setResonance(0.80f);
    }

public:
    Filter filter;

    float volume = 1.0f;
    float mixDivider = 1.0f / APP_TRACKS;

    static Master* instance;

    static Master& get()
    {
        if (!instance) {
            instance = new Master();
        }
        return *instance;
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

Master* Master::instance = NULL;

#endif