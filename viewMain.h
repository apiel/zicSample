#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include "data.h"
#include "draw.h"
#include "progressBar.h"
#include "tempo.h"
#include "view.h"

#define CLEAR true

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
            trackColor = COLOR_ON;
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

    void renderHeaderPattern(Track& track, bool clear = false)
    {
        if (clear) {
            drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 40 });
        }

        drawText({ 10, 10 }, track.name, COLOR_INFO);

        unsigned int x = drawLabelValue({ 100, 2 }, "Volume:", 100, "%");
        x = drawLabelValue({ x + 5, 2 }, "Filter:", "LPF", NULL);
        x = drawLabelValue({ x + 5, 2 }, NULL, 4000.0f, "Hz");
        x = drawLabelValue({ x + 5, 2 }, "Res:", 0, "%");
        drawText({ x + 5, 2 }, track.sample, COLOR_INFO, 14);

        x = drawLabelValue({ 100, 20 }, "Delay:", 0, "%");
        x = drawLabelValue({ x + 5, 20 }, "Reverb:", 0, "%");
        x = drawLabelValue({ x + 5, 20 }, "Compressor", 0, "%");
    }

    void renderHeaderStep()
    {
        drawFilledRect({ 92, 45 }, { SCREEN_W - 97, 20 }, COLOR_STEP_HEADER);
        if (grid.col != 0) {
            Step& step = data.tracks[grid.row].steps[grid.col - 1];
            unsigned int x = step.enabled ? drawText({ 100, 47 }, "ON ", COLOR_STEP, 16, APP_FONT_BOLD)
                                          : drawText({ 100, 47 }, "OFF", COLOR_INFO);
            x = drawLabelValue({ x + 5, 47 }, "Velocity:", (int)(step.velocity * 100), "%");
            drawLabelValue({ x + 5, 47 }, "Condition:", STEP_CONDITIONS[step.condition], NULL);
        }
    }

    void renderBPM()
    {
        char bpm[4];
        sprintf(bpm, "%d", Tempo::get().getBpm());
        unsigned int x = drawText({ 10, 40 }, bpm, COLOR_INFO, 22, APP_FONT_BOLD);
        drawText({ x + 5, 45 }, "BPM", COLOR_LABEL, 10);
    }

    void renderHeader(Track& track)
    {
        drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 60 });
        renderHeaderPattern(track);
        renderHeaderStep();
        renderBPM();
    }

    void renderMasterVolume()
    {
        SDL_Color color = COLOR_ON;
        color.a = 100;
        drawFilledRect({ 5, 67 }, { 84, 5 }, color);
        color.a = 200;
        drawFilledRect({ 5, 67 }, { 60, 5 }, color);
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
        renderMasterVolume();

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
            if (grid.rowChanged()) {
                Track& track = data.tracks[grid.row];
                renderHeaderPattern(track, CLEAR);
                renderHeaderStep();
            }
            if (grid.colChanged()) {
                renderHeaderStep();
            }
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