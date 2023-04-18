#ifndef _AUDIO_HANDLER_H_
#define _AUDIO_HANDLER_H_

#include "def.h"
#include "tempo.h"

class AudioHandler {
protected:
    Tempo& tempo = Tempo::get();

    AudioHandler() { }

public:
    static AudioHandler* instance;

    static AudioHandler& get()
    {
        if (!instance) {
            instance = new AudioHandler();
        }
        return *instance;
    }

    void sample(float* buf, int len)
    {
        if (tempo.next(SDL_GetTicks()))
        // if (tempo.next(SDL_GetTicks64()))
        // if (tempo.next())
        {
            // tracks->next();
            // if (menuView.getView()->renderOn(EVENT_VIEW_ON_TEMPO)) {
            //     render();
            // }
        }
        // tracks->sample(buf, len);
    }
};

AudioHandler* AudioHandler::instance = NULL;

#endif