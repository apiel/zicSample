#ifndef _FILTER_H_
#define _FILTER_H_

#include "def.h"

#include <math.h>

// https://www.martin-finke.de/articles/audio-plugins-013-filter/
// https://www.musicdsp.org/en/latest/Filters/29-resonant-filter.html

class Filter {
protected:
    enum FilterMode {
        FILTER_MODE_OFF,
        FILTER_MODE_LOWPASS_12,
        FILTER_MODE_HIGHPASS_12,
        FILTER_MODE_COUNT,
    };

    // cutoff cannot be 1.0 else div by zero range(_cutoff, 0.01, 0.99);
    float cutoff = 0.99;
    float feedback;
    float buf0 = 0;
    float buf1 = 0;

    float joystickCutoffMod = 0.0;
    float joystickResonanceMod = 0.0;

    void calculateVar()
    {
        calculateVar(cutoff, resonance);
    }

    void calculateVar(float _cutoff, float _resonance)
    {
        if (_resonance == 0.0f) {
            feedback = 0.0f;
            return;
        }

        // cutoff cannot be 1.0 (should we ensure this?)
        feedback = _resonance + _resonance / (1.0 - _cutoff); // would it make sense to make a lookup table for `1.0 / (1.0 - _cutoff);` ?
    }

    float sample(float inputValue, float _cutoff)
    {
        if (mode == FILTER_MODE_OFF || inputValue == 0) {
            return inputValue;
        }

        buf0 += _cutoff * (inputValue - buf0 + feedback * (buf0 - buf1));
        buf1 += _cutoff * (buf0 - buf1);

        if (mode == FILTER_MODE_LOWPASS_12) {
            return buf1;
        }
        return inputValue - buf0;
    }

public:
    int16_t value = 0;
    float resonance = 0.0;
    uint8_t mode = FILTER_MODE_OFF;

    Filter()
    {
        set(value);
    };

    float sample(float inputValue)
    {
        return sample(inputValue, cutoff);
    }

    // float sample(float inputValue, float modCutoff, float modResonance)
    // {
    //     // could be optimized and apply only if modCutoff or modResonance != 0
    //     float _cutoff = cutoff + ((1.0 - cutoff) * modCutoff); // I am not sure this make sense!!
    //     float _resonance = resonance + ((1.0 - resonance) * modResonance);

    //     // optimized if reso = 0 then feedback = 0, no need to calculate...
    //     calculateVar(_cutoff, _resonance);

    //     return sample(inputValue, _cutoff);
    // }

    Filter& set(int16_t val)
    {
        // Frequency should be under 7350Hz else cutoff is = 1 and then no sound
        // We dont want to have LPF/HPF under 50Hz

        value = range(val, -7250, 7250);
        int16_t frequency = value;
        if (value == 0) {
            mode = FILTER_MODE_OFF;
        } else if (value > 0) {
            mode = FILTER_MODE_HIGHPASS_12;
            frequency = frequency + 50;
        } else {
            mode = FILTER_MODE_LOWPASS_12;
            frequency = 7350 - (frequency * -1 + 40);
        }

        cutoff = 2.0 * sin(M_PI * frequency / SAMPLE_RATE); // lookup table?? js: Array.from(Array(7350).keys()).map(frequency => 2.0 * Math.sin(3.141592653589793238 * frequency / 44100));

        // printf("Filter: %d -> %d cutoff %f\n", value, frequency, cutoff);

        calculateVar();

        return *this;
    }

    int16_t getFrequency()
    {
        if (mode == FILTER_MODE_OFF) {
            return 0;
        }
        return (int16_t)(asin(cutoff / 2.0) * SAMPLE_RATE / M_PI);
    }

    float getPctValue()
    {
        if (mode == FILTER_MODE_LOWPASS_12) {
            return 100 * (float)value / -7250.0;
        }
        if (mode == FILTER_MODE_HIGHPASS_12) {
            return 100 * (float)value / 7250.0;
        }
        return 0.0f;
    }

    Filter& setResonance(float _res)
    {
        resonance = range(_res, 0.00, 0.99);
        calculateVar();

        return *this;
    };

    const char* getName()
    {
        switch (mode) {
        case FILTER_MODE_LOWPASS_12:
            return "LPF ";
        case FILTER_MODE_HIGHPASS_12:
            return "HPF ";
        default:
            return "Filter";
        }
    }
};

#endif
