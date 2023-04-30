#ifndef _AUDIO_PREVIEW_H_
#define _AUDIO_PREVIEW_H_

#include "def.h"
#include "audioFile.h"

class AudioPreview {
protected:
    AudioFile audioFile;

    static AudioPreview* instance;

    AudioPreview() {
    }

public:
    static AudioPreview& get()
    {
        if (!instance) {
            instance = new AudioPreview();
        }
        return *instance;
    }

    void samples(float* buf, int len)
    {
    }

    void play(char* path)
    {
        audioFile.open(path);
    }
};

AudioPreview* AudioPreview::instance = NULL;

#endif