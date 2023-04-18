#ifndef _AUDIO_FILE_H_
#define _AUDIO_FILE_H_

#include <sndfile.h>

#include "def.h"

// 10 seconds
#define AUDIO_BUFFER_SIZE SAMPLE_RATE * 10

class AudioFile {
public:
    SF_INFO sfinfo;
    SNDFILE* file = NULL;

    float buffer[AUDIO_BUFFER_SIZE];

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

        sf_read_float(file, buffer, AUDIO_BUFFER_SIZE);

        return file;
    }

    int samplePos = 0;

    void sample(float* buf, int len)
    {
        // sf_read_float(file, buf, len);

        for (int i = 0; i < len; i++) {
            if (samplePos < sfinfo.frames) {
                buf[i] = buffer[samplePos];
                samplePos++;
            } else {
                buf[i] = 0;
            }
        }
    }

    void restart()
    {
        // sf_seek(file, 0, SEEK_SET);

        samplePos = 0;
    }
};

#endif
