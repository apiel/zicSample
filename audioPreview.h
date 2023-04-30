#ifndef _AUDIO_PREVIEW_H_
#define _AUDIO_PREVIEW_H_

#include "def.h"
#include "audioFile.h"

class AudioPreview {
protected:
    AudioFile audioFile;
    bool isPlaying = false;

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

    bool samples(float* buf, int len)
    {
        if (!isPlaying) {
            return false;
        }
        if (!audioFile.samples(buf, len)) {
            APP_LOG("AudioPreview::samples stop\n");
            stop();
        }
        return isPlaying;
    }

    void play(char* path)
    {
        audioFile.open(path, SEEK_SET);
        isPlaying = true;
    }

    void stop()
    {
        audioFile.close();
        isPlaying = false;
    }
};

AudioPreview* AudioPreview::instance = NULL;

#endif