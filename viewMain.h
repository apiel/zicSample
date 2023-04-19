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
    bool editMode = false;
    Grid grid = Grid(APP_TRACKS, APP_TRACK_STEPS + 1);
    Grid gridEdit = Grid(4, 6);
    ProgressBar& progressBar = ProgressBar::get();
    Data& data = Data::get();

    // 75 + 15 * row
    uint16_t rowY[APP_TRACKS] = { 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300 };

    ViewMain() { }

    void fixGridEdit()
    {
        if (gridEdit.row == 1 && gridEdit.col == 0 && gridEdit.lastRow == 0 && gridEdit.lastCol == 0) {
            gridEdit.row = 2;
        } else if (gridEdit.row == 1 && gridEdit.col > 3) {
            gridEdit.col = 3;
        } else if (gridEdit.row == 2 && gridEdit.col > 3) {
            gridEdit.col = 3;
        } else if (gridEdit.row == 3 && gridEdit.col > 0) {
            gridEdit.col = 0;
        }
    }

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

    bool isSelected(unsigned int row, unsigned int col)
    {
        return editMode && gridEdit.is(row, col);
    }

    void renderHeaderPattern(Track& track, bool clear = false)
    {
        if (clear) {
            drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 40 });
        }

        drawText({ 10, 10 }, track.name, COLOR_INFO);
        if (editMode && (gridEdit.is(0, 0) || gridEdit.is(1, 0))) {
            drawRect({ 5, 5 }, { 85, 30 }, COLOR_INFO);
        }

        unsigned int x = drawLabelValue({ 100, 5 }, "Volume:", 100, "%", isSelected(0, 1));
        x = drawLabelValue({ x + 5, 5 }, "Filter:", "LPF", NULL, isSelected(0, 2));
        x = drawLabelValue({ x + 5, 5 }, NULL, 4000.0f, "Hz", isSelected(0, 3));
        x = drawLabelValue({ x + 5, 5 }, "Res:", 0, "%", isSelected(0, 4));
        drawSelectableText(isSelected(0, 5), { x + 5, 5 }, track.sample, COLOR_INFO, 14);

        x = drawLabelValue({ 100, 22 }, "Delay:", 0, "%", isSelected(1, 1));
        x = drawLabelValue({ x + 5, 22 }, "Reverb:", 0, "%", isSelected(1, 2));
        x = drawLabelValue({ x + 5, 22 }, "Compressor", 0, "%", isSelected(1, 3));
    }

    void renderHeaderStep()
    {
        drawFilledRect({ 92, 45 }, { SCREEN_W - 97, 20 }, COLOR_STEP_HEADER);
        if (grid.col != 0) {
            Step& step = data.tracks[grid.row].steps[grid.col - 1];
            unsigned int x = step.enabled ? drawText({ 100, 47 }, "ON ", COLOR_STEP, 16, APP_FONT_BOLD)
                                          : drawText({ 100, 47 }, "OFF", COLOR_INFO);
            if (editMode && gridEdit.is(2, 1)) {
                drawRect({ 98, 46 }, { 30, 18 }, COLOR_INFO);
            }
            x = drawLabelValue({ x + 5, 47 }, "Velocity:", (int)(step.velocity * 100), "%", isSelected(2, 2));
            drawLabelValue({ x + 5, 47 }, "Condition:", STEP_CONDITIONS[step.condition], NULL, isSelected(2, 3));
        }
    }

    void renderBPM()
    {
        char bpm[4];
        sprintf(bpm, "%d", Tempo::get().getBpm());
        unsigned int x = drawText({ 10, 40 }, bpm, COLOR_INFO, 22, APP_FONT_BOLD);
        drawText({ x + 5, 45 }, "BPM", COLOR_LABEL, 10);
        if (editMode && gridEdit.is(2, 0)) {
            drawRect({ 5, 40 }, { 85, 25 }, COLOR_INFO);
        }
    }

    void renderHeader(Track& track)
    {
        drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 60 });
        renderHeaderPattern(track);
        renderHeaderStep();
        renderBPM();
        renderMasterVolume();
    }

    void renderMasterVolume()
    {
        drawFilledRect({ 4, 66 }, { 86, 7 }, COLOR_BACKGROUND);
        SDL_Color color = COLOR_ON;
        color.a = 100;
        drawFilledRect({ 5, 67 }, { 84, 5 }, color);
        color.a = 200;
        drawFilledRect({ 5, 67 }, { 60, 5 }, color);
        if (editMode && gridEdit.is(3, 0)) {
            drawRect({ 4, 66 }, { 86, 7 }, COLOR_WHITE);
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
        renderMasterVolume();

        Track& track = data.tracks[grid.row];
        renderHeader(track);

        for (unsigned int row = 0; row < APP_TRACKS; row++) {
            renderRow(row);
        }

        draw();
    }

    void update(UiKeys& keys)
    {
        if (editMode) {
            if (gridEdit.update(keys) == VIEW_CHANGED) {
                fixGridEdit();
                // FIXME ??? optimize ?
                renderHeader(data.tracks[grid.row]);
                draw();
            }
        } else if (grid.update(keys) == VIEW_CHANGED) {
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

        if (keys.Menu) {
            editMode = !editMode;
            renderSlection();
            // FIXME ??? optimize ?
            renderHeader(data.tracks[grid.row]);
            draw();
            return;
        } else if (keys.Edit) {
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