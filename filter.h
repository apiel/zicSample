#ifndef _FILTER_H_
#define _FILTER_H_

#include "def.h"

#include <math.h>

// https://www.martin-finke.de/articles/audio-plugins-013-filter/
// https://www.musicdsp.org/en/latest/Filters/29-resonant-filter.html

class Filter {
protected:
    float buf0 = 0;
    float buf1 = 0;
    float buf2 = 0;
    float buf3 = 0;

    float Q1;
    float feedback;
    void calculateVar()
    {
        calculateVar(cutoff, resonance);
    }

    void calculateVar(float _cutoff, float _resonance)
    {
        if (_cutoff == 1.0) {
            feedback = _resonance + _resonance;
        } else {
            feedback = _resonance + _resonance / (1.0 - _cutoff);
        }

        // Q1 is for the second filtering method
        Q1 = 1 / (_resonance * _resonance * 1000 + 0.7); // 1000 value set randomly, might need to find better value?!
    }

    float nextResonantFilter(float inputValue, float _cutoff)
    {
        // https://www.musicdsp.org/en/latest/Filters/29-resonant-filter.html
        // https://www.martin-finke.de/articles/audio-plugins-013-filter/
        buf0 += _cutoff * (inputValue - buf0 + feedback * (buf0 - buf1));
        buf1 += _cutoff * (buf0 - buf1);
        buf2 += _cutoff * (buf1 - buf2);
        buf3 += _cutoff * (buf2 - buf3);
        switch (mode) {
        case FILTER_MODE_LOWPASS_12:
            return buf1;
        case FILTER_MODE_LOWPASS_24:
            return buf3;
        case FILTER_MODE_HIGHPASS_12:
            return inputValue - buf0;
        case FILTER_MODE_HIGHPASS_24:
            return inputValue - buf3;
        case FILTER_MODE_BANDPASS_12:
            return buf0 - buf1;
        case FILTER_MODE_BANDPASS_24:
            return buf0 - buf3;
        default:
            return inputValue;
        }
    }

    float nextStateVariableFilter(float inputValue, float _cutoff)
    {
        // https://www.musicdsp.org/en/latest/Filters/142-state-variable-filter-chamberlin-version.html
        float lowpass = buf1 + _cutoff * buf0;
        float highpass = inputValue - lowpass - Q1 * buf0;
        float bandpass = _cutoff * highpass + buf0;
        float notch = highpass + lowpass;

        buf0 = bandpass;
        buf1 = lowpass;

        switch (mode) {
        case FILTER_MODE_LOWPASS_STATE_VARIABLE:
            return lowpass;
        case FILTER_MODE_HIGHPASS_STATE_VARIABLE:
            return highpass;
        case FILTER_MODE_BANDPASS_STATE_VARIABLE:
            return bandpass;
        case FILTER_MODE_NOTCH_STATE_VARIABLE:
            return notch;
        default:
            return inputValue;
        }
    }

    float next(float inputValue, float _cutoff)
    {
        if (mode == FILTER_MODE_OFF || inputValue == 0) {
            return inputValue;
        }

        if (mode >= FILTER_MODE_LOWPASS_STATE_VARIABLE) {
            return nextStateVariableFilter(inputValue, _cutoff);
        }

        return nextResonantFilter(inputValue, _cutoff);
    }

public:
    enum FilterMode {
        FILTER_MODE_OFF,
        FILTER_MODE_LOWPASS_12,
        FILTER_MODE_LOWPASS_24,
        FILTER_MODE_HIGHPASS_12,
        FILTER_MODE_HIGHPASS_24,
        FILTER_MODE_BANDPASS_12,
        FILTER_MODE_BANDPASS_24,
        FILTER_MODE_LOWPASS_STATE_VARIABLE,
        FILTER_MODE_HIGHPASS_STATE_VARIABLE,
        FILTER_MODE_BANDPASS_STATE_VARIABLE,
        FILTER_MODE_NOTCH_STATE_VARIABLE,
        FILTER_MODE_COUNT,
    };

    // uint16_t frequency = (uint16_t)(SAMPLE_RATE / 2.0); // Nyquist
    uint16_t frequency = 8000;
    float cutoff = 0.99;
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

    Filter* setFrequency(int16_t freq)
    {
        // Human can hear frequency between 20Hz and 20kHz. While 20 to 20,000Hz forms the absolute borders
        // of the human hearing range, our hearing is most sensitive in the 2000 - 5000 Hz frequency range.
        // But establishing the effect of sounds with frequencies under about 250 Hz has been harder. Even
        // though they're above the lower limit of 20 Hz, these low-frequency sounds tend to be either
        // inaudible or barely audible.
        frequency = range(freq, 200, 8000);
        cutoff = 2.0 * sin(M_PI * frequency / SAMPLE_RATE);
        calculateVar();

        return this;
    }

    // could set cutoff for modulation
    // but maybe it would just be better to precalculate all the possible value for the frequency...
    //
    // or use 0 to .99 to set the value and use the getFrequencyFromCutoff ???
    //
    // look up table is most likely the best option!
    Filter* setCutoff(float _cutoff)
    {
        // cutoff cannot be 1.0 else div by zero
        // cutoff = range(_cutoff, 0.01, 0.99);

        cutoff = _cutoff;
        calculateVar();

        return this;
    };

    float getFrequencyFromCutoff()
    {
        return SAMPLE_RATE * asin(cutoff / 2.0) / M_PI;
    }

    Filter* setResonance(float _res)
    {
        // resonance = range(_res, 0.00, 0.99); ??
        resonance = range(_res, 0.00, 1.00);
        calculateVar();

        return this;
    };

    Filter* setFilterMode(int8_t value)
    {
        mode = range(value, 0, FILTER_MODE_COUNT);

        // printf("setFilterMode %d %d\n", value, mode);

        return this;
    }

    const char* getName()
    {
        switch (mode) {
        case FILTER_MODE_LOWPASS_12:
            return "LPF 12dB";
        case FILTER_MODE_LOWPASS_24:
            return "LPF 24dB";
        case FILTER_MODE_HIGHPASS_12:
            return "HPF 12dB";
        case FILTER_MODE_HIGHPASS_24:
            return "HPF 24dB";
        case FILTER_MODE_BANDPASS_12:
            return "BPF 12dB";
        case FILTER_MODE_BANDPASS_24:
            return "BPF 24dB";
        case FILTER_MODE_LOWPASS_STATE_VARIABLE:
            return "LPF SV";
        case FILTER_MODE_HIGHPASS_STATE_VARIABLE:
            return "HPF SV";
        case FILTER_MODE_BANDPASS_STATE_VARIABLE:
            return "BPF SV";
        case FILTER_MODE_NOTCH_STATE_VARIABLE:
            return "NOTCH SV";
        default:
            return "OFF";
        }
    }
};

#endif
