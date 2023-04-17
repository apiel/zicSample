#ifndef _AUDIO_FILE_H_
#define _AUDIO_FILE_H_

#include <sndfile.h>

#include "def.h"

class AudioFile {
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
        printf("Audio file %s sampleCount %ld\n", filename, (long)sfinfo.frames);

        return file;
    }
};

#endif
