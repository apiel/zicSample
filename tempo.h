#ifndef _TEMPO_H_
#define _TEMPO_H_

#include "def.h"

// 4 𝆺𝅥𝅯𝆺𝅥𝅯𝆺𝅥𝅯𝆺𝅥𝅯 step per beat
#ifndef STEP_PER_BEAT
#define STEP_PER_BEAT 4
#endif

class Tempo {
protected:
    const float SAMPLE_PER_MS = SAMPLE_RATE / 1000.0f; // 44.1
    const uint16_t SAMPLE_COUNT_INCREMENT = APP_AUDIO_CHUNK / APP_CHANNELS;

    unsigned long previous = 0;
    uint16_t tempo;
    uint16_t sampleCount = 0;
    unsigned long time;

    static Tempo* instance;

    Tempo()
    {
        set(120);
    }

public:
    uint16_t bpm;

    static Tempo& get()
    {
        if (!instance) {
            instance = new Tempo();
        }
        return *instance;
    }

    /**
     * @brief Set tempo in beat per minutes (BPM)
     *
     * @param _bpm
     */
    void set(uint16_t _bpm)
    {
        bpm = range(_bpm, 10, 250);
        tempo = 60000.0f / (float)(bpm * STEP_PER_BEAT);
        // printf("Tempo (%d -> %d): %d\n", _bpm, bpm, tempo);
    }

    /**
     * @brief Get tempo in beat per minutes (BPM)
     *
     * @return uint16_t
     */
    uint16_t getBpm()
    {
        return bpm;
    }

    /**
     * @brief Tempo base on given time
     *
     * @param now
     * @return true
     * @return false
     */
    bool next(unsigned long now)
    {
        if ((now - previous) >= tempo) {
            previous = now;
            return true;
        }
        return false;
    }

    /**
     * @brief Tempo base on sample rate
     *
     * @return true
     * @return false
     */
    bool next()
    {
        sampleCount += SAMPLE_COUNT_INCREMENT;
        if (sampleCount > SAMPLE_PER_MS) {
            time += sampleCount / (SAMPLE_PER_MS);
            sampleCount = 0;
            return next(time);
        }
        return false;
    }
};

Tempo* Tempo::instance = NULL;

#endif