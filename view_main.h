#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include "draw.h"
#include "view.h"
#include "data.h"

class ViewMain : public View {
protected:
    ViewMain() { }

    void renderRow(unsigned int row)
    {
        unsigned int y = 70 + 15 * row;
        drawFilledRect({ 5, y }, { 84, 12 });

        Track& track = Data::getInstance()->tracks[row];
        drawText({ 8, y }, track.name, COLOR_INFO, 10);

        for (unsigned int step = 0; step < APP_TRACK_STEPS; step++) {
            unsigned int x = 92 + 12 * step;
            SDL_Color color = COLOR_FOREGROUND;
            if (step % 4 == 0) {
                color = COLOR_FOREGROUND2;
            }
            drawFilledRect({ x, y }, { 10, 12 }, color);
        }
    }

public:
    static ViewMain* instance;

    static ViewMain* getInstance()
    {
        if (!instance) {
            instance = new ViewMain();
        }
        return instance;
    }

    void render()
    {
        drawClear();

        drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 60 });

        drawText({ 10, 10 }, "Hello World", COLOR_INFO);

        for (unsigned int row = 0; row < APP_TRACKS; row++) {
            renderRow(row);
        }
    }

    uint8_t update(UiKeys* keys)
    {
        return VIEW_NONE;
    }
};

ViewMain* ViewMain::instance = NULL;

#endif