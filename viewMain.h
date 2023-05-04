#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include "audioHandler.h"
#include "data.h"
#include "draw.h"
#include "drawHeaderButtonValue.h"
#include "grid.h"
#include "menu.h"
#include "patternSelector.h"
#include "progressBar.h"
#include "tempo.h"
#include "view.h"

#define GRID_PATTERN_TOP 69
#define GRID_PATTERN_ROW_H 15

class ViewMainHeaderTrack {
protected:
    HeaderButtonValue headerButtonValue = HeaderButtonValue();

public:
    void renderY(Track& track)
    {
        headerButtonValue.btnY.label1 = "Volume";
        char volume[4];
        sprintf(volume, "%d", (int)(track.volume * 100));
        headerButtonValue.btnY.value1 = volume;
        headerButtonValue.btnY.unit1 = "%";

        headerButtonValue.btnY.label2 = "Distortion";
        char distortion[4];
        sprintf(distortion, "%d", (int)(track.distortion.drive * 100));
        headerButtonValue.btnY.value2 = distortion;
        headerButtonValue.btnY.unit2 = "%";

        headerButtonValue.drawY();
    }

    void renderX(Track& track)
    {
        headerButtonValue.btnX.label1 = "Sample";
        headerButtonValue.btnX.value1 = track.audioFileName;

        headerButtonValue.drawX();
    }

    void renderA(Track& track)
    {
        headerButtonValue.btnA.label1 = track.filter.getName();
        char filter[6];
        sprintf(filter, "%.1f", track.filter.getPctValue());
        headerButtonValue.btnA.value1 = filter;
        headerButtonValue.btnA.unit1 = "%";

        headerButtonValue.btnA.label2 = "Resonance";
        char resonance[4];
        sprintf(resonance, "%d", (int)(track.filter.resonance * 100));
        headerButtonValue.btnA.value2 = resonance;
        headerButtonValue.btnA.unit2 = "%";

        headerButtonValue.drawA();
    }

    void renderB(Track& track)
    {
        headerButtonValue.btnB.label1 = "Status";
        headerButtonValue.btnB.value1 = track.active ? "Playing" : "Stopped";

        // headerButtonValue.btnB.label2 = "Velocity";
        // headerButtonValue.btnB.value2 = "100";

        headerButtonValue.drawB();
    }

    void render(Track& track)
    {
        drawText({ 10, 10 }, track.name, COLOR_INFO);

        renderY(track);
        renderX(track);
        renderA(track);
        renderB(track);
    }
};

class ViewMain : public View {
protected:
    Grid grid = Grid(APP_TRACKS + 1, APP_TRACK_STEPS + 1);
    Grid gridEdit = Grid(3, 5);
    ProgressBar& progressBar = ProgressBar::get();
    Data& data = Data::get();
    AudioHandler& audio = AudioHandler::get();
    Menu& menu = Menu::get();
    PatternSelector& patternSelector = PatternSelector::get();

    ViewMainHeaderTrack mainHeaderTrack = ViewMainHeaderTrack();

    uint16_t rowY[APP_TRACKS + 1] = {
        GRID_PATTERN_TOP,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 2,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 3,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 4,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 5,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 6,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 7,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 8,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 9,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 10,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 11,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 12,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 13,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 14,
        GRID_PATTERN_TOP + GRID_PATTERN_ROW_H * 15,
        progressBar.y
    };

    static ViewMain* instance;

    ViewMain() { }

    void fixGridEdit()
    {
        if (gridEdit.row == 1 && gridEdit.col > 3) {
            gridEdit.col = 3;
        } else if (gridEdit.row == 2 && grid.col == 0) {
            gridEdit.row = 1;
        } else if (gridEdit.row == 2 && gridEdit.col < 1) {
            gridEdit.col = 1;
        } else if (gridEdit.row == 2 && gridEdit.col > 3) {
            gridEdit.col = 3;
        }
    }

    void renderSelection(int8_t row, int8_t col, SDL_Color color = COLOR_SELECTOR)
    {
        unsigned int y = rowY[row];
        uint8_t h = 14;
        if (row == APP_TRACKS) {
            h = 7;
            col = 0;
        }

        if (col == 0) {
            drawRect({ 4, y - 1 }, { 86, h }, color);
        } else {
            unsigned int x = 92 + 12 * (col - 1);
            drawRect({ x - 1, y - 1 }, { 12, h }, color);
        }
    }

    void renderSelection()
    {
        renderSelection(grid.lastRow, grid.lastCol, COLOR_BACKGROUND);
        renderSelection(grid.row, grid.col);
    }

    void renderTrackName(Track& track, unsigned int row)
    {
        unsigned int y = rowY[row];

        drawFilledRect({ 5, y }, { 84, 12 }, COLOR_FOREGROUND);

        SDL_Color trackColor = COLOR_FOREGROUND2;
        SDL_Color trackText = COLOR_INFO;
        if (track.active) {
            trackColor = COLOR_ON;
            trackText = COLOR_WHITE;
        }
        trackColor.a = 50;
        drawFilledRect({ 5, y }, { 84, 12 }, trackColor);
        trackColor.a = 200;
        unsigned int width = 84.0 * track.volume;
        drawFilledRect({ 5, y }, { width, 12 }, trackColor);

        drawText({ 8, y }, track.name, trackText, 10);
    }

    void renderStep(Track& track, unsigned int step, unsigned int row)
    {
        unsigned int y = rowY[row];
        unsigned int x = 92 + 12 * step;
        SDL_Color color = COLOR_FOREGROUND;
        if (track.steps[step].enabled) {
            color = COLOR_STEP;
            if (track.steps[step].condition) {
                color = COLOR_STEP_WITH_CONDITION;
            }
            color.a = 255 * track.steps[step].velocity;
        } else if (step % 4 == 0) {
            color = COLOR_FOREGROUND2;
        }
        drawFilledRect({ x, y }, { 10, 12 }, color);
    }

    void renderRow(unsigned int row)
    {
        Track& track = getTrack(row);
        renderTrackName(track, row);

        for (unsigned int step = 0; step < APP_TRACK_STEPS; step++) {
            renderStep(track, step, row);
        }
        renderSelection();
    }

    void renderMasterVolume(bool selected = false)
    {
        drawFilledRect({ 4, progressBar.y - 1 }, { 86, progressBar.h + 2 }, COLOR_BACKGROUND);
        SDL_Color color = COLOR_ON;
        color.a = 100;
        drawFilledRect({ 5, progressBar.y }, { 84, progressBar.h }, color);
        color.a = 200;
        unsigned int width = 84.0 * audio.getVolume() / APP_MAX_VOLUME;
        drawFilledRect({ 5, progressBar.y }, { width, progressBar.h }, color);
        if (selected) {
            drawRect({ 4, progressBar.y - 1 }, { 86, progressBar.h + 2 }, COLOR_WHITE);
        }
    }

    Track& getTrack(int8_t gridRow) { return data.tracks[gridRow]; }

    void renderHeaderStep()
    {
        Track& track = getTrack();
        drawText({ 10, 10 }, track.name, COLOR_INFO);
    }

    void renderHeaderMaster()
    {
        drawText({ 10, 10 }, "Master", COLOR_INFO);
    }

    void renderHeader()
    {
        drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 60 });
        if (grid.row == APP_TRACKS) {
            renderHeaderMaster();
        } else if (grid.col == 0) {
            mainHeaderTrack.render(getTrack());
        } else {
            renderHeaderStep();
        }
    }

    void renderRows(bool clear = false)
    {
        if (clear) {
            drawFilledRect({ 0, rowY[0] }, { SCREEN_W, rowY[0] - progressBar.y }, COLOR_BACKGROUND);
        }
        for (unsigned int row = 0; row < APP_TRACKS; row++) {
            renderRow(row);
        }
    }

public:
    static ViewMain& get()
    {
        if (!instance) {
            instance = new ViewMain();
        }
        return *instance;
    }

    Track& getTrack() { return getTrack(grid.row); }

    void render()
    {
        drawClear();

        progressBar.init();
        renderMasterVolume();

        renderHeader();
        renderRows();

        draw();
    }

    void handle(UiKeys& keys)
    {
        if (patternSelector.isVisible()) {
            if (patternSelector.handle(keys)) {
                render();
                draw();
            }
            return;
        }

        if (keys.Menu) {
            if (menu.toggle()) {
                menu.render();
            } else {
                render();
            }
            draw();
            return;
        }

        if (menu.isVisible) {
            if (menu.handle(keys, getTrack())) {
                render();
                draw();
            }
            return;
        }

        if (keys.btnA) {
            //     handleMain(keys);
            return;
        }

        if (keys.btnY) {
            // handleMainbtnY(keys);
            return;
        }

        if (keys.btnB) {
            if (grid.row == APP_TRACKS) {
                // Could start to play pause
                // audio.tempo.toggle();
                // draw();
                return;
            }
            Track& track = getTrack();
            if (grid.col == 0) {
                track.toggle();
                renderTrackName(track, grid.row);
                draw();
                return;
            } else {
                uint8_t step = grid.col - 1;
                track.steps[step].toggle();
                // renderHeaderStep();
                renderStep(track, step, grid.row);
                draw();
                return;
            }
        }

        if (grid.update(keys) == VIEW_CHANGED) {
            renderSelection();
            renderHeader();
            draw();
            return;
        }
    }
};

ViewMain* ViewMain::instance = NULL;

#endif