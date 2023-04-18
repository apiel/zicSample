#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include "data.h"
#include "draw.h"
#include "view.h"
#include "progressBar.h"

class ViewMain : public View {
protected:
    Grid grid = Grid(APP_TRACKS, APP_TRACK_STEPS);
    ProgressBar& progressBar = ProgressBar::get();

    ViewMain() { }

    void renderRow(unsigned int row)
    {
        unsigned int y = 75 + 15 * row;

        Track& track = Data::get().tracks[row];

        SDL_Color trackColor = COLOR_FOREGROUND;
        SDL_Color trackText = COLOR_INFO;
        if (track.active) {
            trackColor = COLOR_TRACK_ON;
            trackText = COLOR_WHITE;
        }
        drawFilledRect({ 5, y }, { 84, 12 }, trackColor);

        drawText({ 8, y }, track.name, trackText, 10);

        if (grid.is(row, 0)) {
            drawRect({ 4, y - 1 }, { 86, 14 }, COLOR_SELECTOR);
        }

        for (unsigned int step = 0; step < APP_TRACK_STEPS; step++) {
            unsigned int x = 92 + 12 * step;
            SDL_Color color = COLOR_FOREGROUND;
            if (track.steps[step].enabled) {
                color = COLOR_STEP;
                color.a = 255 * track.steps[step].velocity;
            } else if (step % 4 == 0) {
                color = COLOR_FOREGROUND2;
            }
            drawFilledRect({ x, y }, { 10, 12 }, color);

            if (grid.is(row, step + 1)) {
                drawRect({ x - 1, y - 1 }, { 12, 14 }, COLOR_SELECTOR);
            }
        }
    }

public:
    static ViewMain* instance;

    static ViewMain& get()
    {
        if (!instance) {
            instance = new ViewMain();
        }
        return *instance;
    }

    void render()
    {
        drawClear();

        progressBar.init();

        drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 60 });

        Track& track = Data::get().tracks[grid.row];
        drawText({ 10, 10 }, track.name, COLOR_INFO);

        for (unsigned int row = 0; row < APP_TRACKS; row++) {
            renderRow(row);
        }
    }

    uint8_t update(UiKeys& keys)
    {
        if (grid.update(keys) == VIEW_CHANGED) {
            return VIEW_CHANGED;
        }

        if (keys.Edit) {
            if (grid.col == 0) {
                Data::get().tracks[grid.row].toggleNextState();
                // printf("Track %d active: %d nextState: %d\n", grid.row, Data::get().tracks[grid.row].active, Data::get().tracks[grid.row].nextState);
            } else {
                Data::get().tracks[grid.row].steps[grid.col - 1].toggle();
            }
            return VIEW_CHANGED;
        }

        return VIEW_NONE;
    }
};

ViewMain* ViewMain::instance = NULL;

#endif