#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include "data.h"
#include "draw.h"
#include "progressBar.h"
#include "view.h"

class ViewMain : public View {
protected:
    Grid grid = Grid(APP_TRACKS, APP_TRACK_STEPS + 1);
    ProgressBar& progressBar = ProgressBar::get();
    Data& data = Data::get();

    // 75 + 15 * row
    uint16_t rowY[APP_TRACKS] = { 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300 };

    ViewMain() { }

    void renderSlection(int8_t row, int8_t col, SDL_Color color = COLOR_SELECTOR)
    {
        unsigned int y = rowY[row];
        if (col == 0) {
            drawRect({ 4, y - 1 }, { 86, 14 }, color);
        } else {
            unsigned int x = 92 + 12 * (col - 1);
            drawRect({ x - 1, y - 1 }, { 12, 14 }, color);
        }
    }

    void renderSlection()
    {
        renderSlection(grid.lastRow, grid.lastCol, COLOR_BACKGROUND);
        renderSlection(grid.row, grid.col);
    }

    void renderTrackName(Track& track, unsigned int row)
    {
        unsigned int y = rowY[row];

        SDL_Color trackColor = COLOR_FOREGROUND;
        SDL_Color trackText = COLOR_INFO;
        if (track.active) {
            trackColor = COLOR_TRACK_ON;
            trackText = COLOR_WHITE;
        }
        drawFilledRect({ 5, y }, { 84, 12 }, trackColor);

        drawText({ 8, y }, track.name, trackText, 10);
    }

    void renderStep(Track& track, unsigned int step, unsigned int row)
    {
        unsigned int y = rowY[row];
        unsigned int x = 92 + 12 * step;
        SDL_Color color = COLOR_FOREGROUND;
        if (track.steps[step].enabled) {
            color = COLOR_STEP;
            color.a = 255 * track.steps[step].velocity;
        } else if (step % 4 == 0) {
            color = COLOR_FOREGROUND2;
        }
        drawFilledRect({ x, y }, { 10, 12 }, color);
    }

    void renderRow(unsigned int row)
    {
        Track& track = data.tracks[row];
        renderTrackName(track, row);

        for (unsigned int step = 0; step < APP_TRACK_STEPS; step++) {
            renderStep(track, step, row);
        }
        renderSlection();
    }

    void renderHeader(Track& track)
    {
        drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 60 });
        drawText({ 10, 10 }, track.name, COLOR_INFO);

        unsigned int x = drawLabelValue({ 100, 10 }, "Volume:", 100, "%");
        x = drawLabelValue({ x + 5, 10 }, "Filter:", "LPF", NULL);
        x = drawLabelValue({ x + 5, 10 }, NULL, 4000.0f, "Hz");
        x = drawLabelValue({ x + 5, 10 }, "Res:", 0, "%");

        SDL_Color color = COLOR_STEP;
        color.a = 40;
        drawFilledRect({ 92, 45 }, { SCREEN_W - 97, 20 }, color);
        x = drawLabelValue({ 100, 47 }, "Velocity:", 100, "%");
        x = drawLabelValue({ x + 5, 47 }, "Condition:", 80, "%");
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

        Track& track = Data::get().tracks[grid.row];
        renderHeader(track);

        for (unsigned int row = 0; row < APP_TRACKS; row++) {
            renderRow(row);
        }

        draw();
    }

    void update(UiKeys& keys)
    {
        if (grid.update(keys) == VIEW_CHANGED) {
            renderSlection();
            draw();
            return;
        }

        if (keys.Edit) {
            Track& track = data.tracks[grid.row];
            if (grid.col == 0) {
                track.toggle();
                renderTrackName(track, grid.row);
                draw();
                return;
            } else {
                uint8_t step = grid.col - 1;
                track.steps[step].toggle();
                renderStep(track, step, grid.row);
                draw();
                return;
            }
        }
    }
};

ViewMain* ViewMain::instance = NULL;

#endif