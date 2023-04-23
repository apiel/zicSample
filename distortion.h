#ifndef _DISTORTION_H_
#define _DISTORTION_H_

#include "def.h"

#include <math.h>

class Distortion {
protected:
    float shape;

public:
    float drive = 0.0;

    Distortion()
    {
        set(drive);
    };

    float next(float sample)
    {
        if (drive == 0.0) {
            return sample;
        }
        return (1 + shape) * sample / (1 + shape * fabsf(sample));
    }

    Distortion& set(float _drive)
    {
        drive = range(_drive, 0.0, 1.0);
        if (drive > 0.0) {
            shape = 2 * (drive - 0.000001) / (1 - (drive - 0.000001));
        }

        return *this;
    }
};

#endif
