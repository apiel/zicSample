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

    float sample(float buf)
    {
        if (drive == 0.0 || buf == 0.0) {
            return buf;
        }
        return (1 + shape) * buf / (1 + shape * fabsf(buf));
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
