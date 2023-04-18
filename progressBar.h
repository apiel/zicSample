#ifndef _PROGRESS_BAR_H_
#define _PROGRESS_BAR_H_

#include "data.h"
#include "draw.h"
#include "view.h"

class ProgressBar {
protected:
    unsigned int y = 67;
    uint8_t h = 5;

    ProgressBar() { }

public:
    static ProgressBar* instance;

    static ProgressBar& get()
    {
        if (!instance) {
            instance = new ProgressBar();
        }
        return *instance;
    }

    void init()
    {
        for (unsigned int step = 0; step < APP_TRACK_STEPS; step++) {
            unsigned int x = 92 + 12 * step;
            drawFilledRect({ x, y }, { 10, h }, COLOR_FOREGROUND2);
        }
    }

    void render(uint8_t stepCounter)
    {
        unsigned int xPrevious = 92 + 12 * ((stepCounter - 1 + APP_TRACK_STEPS) % APP_TRACK_STEPS);
        drawFilledRect({ xPrevious, y }, { 10, h }, COLOR_FOREGROUND2);
        unsigned int x = 92 + 12 * stepCounter;
        drawFilledRect({ x, y }, { 10, h }, COLOR_TRACK_ON);
    }
};

ProgressBar* ProgressBar::instance = NULL;

#endif