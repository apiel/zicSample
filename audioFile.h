#ifndef _AUDIO_FILE_H_
#define _AUDIO_FILE_H_

#include <sndfile.h>

#include "def.h"

// Should we load sample in memory?
// How long can we buffer? 10sec?
// If not, we will read it from disk every time we need it.
#define AUDIO_BUFFER_SECONDS 0 // 10

#if AUDIO_BUFFER_SECONDS > 0
// 10 seconds
#define LOAD_SAMPLE_IN_MEMORY 1
#define AUDIO_BUFFER_SIZE SAMPLE_RATE* AUDIO_BUFFER_SECONDS
#endif

class AudioFile {
protected:
#if LOAD_SAMPLE_IN_MEMORY
    int samplePos = 0;
    float buffer[AUDIO_BUFFER_SIZE];
#endif

    bool isOpen = false;

public:
    SF_INFO sfinfo;
    SNDFILE* file = NULL;

    AudioFile()
    {
        memset(&sfinfo, 0, sizeof(sfinfo));
    }

    ~AudioFile()
    {
        close();
    }

    void close()
    {
        isOpen = false;
        if (file) {
            sf_close(file);
        }
    }

    void* open(const char* filename)
    {
        close();

        if (!(file = sf_open(filename, SFM_READ, &sfinfo))) {
            printf("Error: could not open file %s\n", filename);
            return NULL;
        }
        printf("Audio file %s sampleCount %ld sampleRate %d\n", filename, (long)sfinfo.frames, sfinfo.samplerate);
        isOpen = true;

        sf_seek(file, 0, SEEK_END); // Move to the end to avoid unwanted play on first sound toggle

#if LOAD_SAMPLE_IN_MEMORY
        sf_read_float(file, buffer, AUDIO_BUFFER_SIZE);
#endif

        return file;
    }

    void samples(float* buf, int len)
    {
#if LOAD_SAMPLE_IN_MEMORY
        for (int i = 0; i < len; i++) {
            if (samplePos < sfinfo.frames) {
                buf[i] = buffer[samplePos];
                samplePos++;
            } else {
                buf[i] = 0;
            }
        }
#else
        if (isOpen) {
            sf_read_float(file, buf, len);
        }
#endif
    }

    void restart()
    {
#if LOAD_SAMPLE_IN_MEMORY
        samplePos = 0;
#else
        sf_seek(file, 0, SEEK_SET);
#endif
    }
};

#endif
