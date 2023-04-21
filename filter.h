#ifndef _FILTER_H_
#define _FILTER_H_

#include "def.h"

#include <math.h>

// https://www.martin-finke.de/articles/audio-plugins-013-filter/
// https://www.musicdsp.org/en/latest/Filters/29-resonant-filter.html

class Filter {
protected:
    // cutoff cannot be 1.0 else div by zero range(_cutoff, 0.01, 0.99);
    float cutoff = 0.99;
    float feedback;
    float buf0 = 0;
    float buf1 = 0;

    void calculateVar()
    {
        calculateVar(cutoff, resonance);
    }

    void calculateVar(float _cutoff, float _resonance)
    {
        if (_cutoff == 1.0) { // Is this right??
            feedback = _resonance + _resonance;
        } else {
            feedback = _resonance + _resonance / (1.0 - _cutoff);
        }
    }

    float next(float inputValue, float _cutoff)
    {
        if (mode == FILTER_MODE_OFF || inputValue == 0) {
            return inputValue;
        }

        buf0 += _cutoff * (inputValue - buf0 + feedback * (buf0 - buf1));
        buf1 += _cutoff * (buf0 - buf1);
        switch (mode) {
        case FILTER_MODE_LOWPASS_12:
            return buf1;
        case FILTER_MODE_HIGHPASS_12:
            return inputValue - buf0;
        default:
            return inputValue;
        }
    }

public:
    enum FilterMode {
        FILTER_MODE_OFF,
        FILTER_MODE_LOWPASS_12,
        FILTER_MODE_HIGHPASS_12,
        FILTER_MODE_COUNT,
    };

    int16_t frequency = 0;
    float resonance = 0.0;
    uint8_t mode = FILTER_MODE_OFF;

    Filter()
    {
        // calculateVar();
        setFrequency(frequency);
    };

    float next(float inputValue)
    {
        return next(inputValue, cutoff);
    }

    float next(float inputValue, float modCutoff, float modResonance)
    {
        float _cutoff = cutoff + ((1.0 - cutoff) * modCutoff);
        float _resonance = resonance + ((1.0 - resonance) * modResonance);
        calculateVar(_cutoff, _resonance);

        return next(inputValue, _cutoff);
    }

    Filter& setFrequency(int16_t freq)
    {
        frequency = range(freq, -7950, 8000);
        int16_t _frequency = frequency;
        if (frequency == 0) {
            mode = FILTER_MODE_OFF;
        } else if (frequency > 0) {
            mode = FILTER_MODE_HIGHPASS_12;
        } else {
            _frequency = _frequency + 8000;
            mode = FILTER_MODE_LOWPASS_12;
        }
        cutoff = 2.0 * sin(M_PI * _frequency / SAMPLE_RATE);
        calculateVar();

        return *this;
    }

    Filter& setResonance(float _res)
    {
        resonance = range(_res, 0.00, 1.00);
        calculateVar();

        return *this;
    };

    Filter& setFilterMode(int8_t value)
    {
        mode = range(value, 0, FILTER_MODE_COUNT);
        return *this;
    }

    const char* getName()
    {
        switch (mode) {
        case FILTER_MODE_LOWPASS_12:
            return "LPF 12dB";
        case FILTER_MODE_HIGHPASS_12:
            return "HPF 12dB";
        default:
            return "OFF";
        }
    }
};

#endif
