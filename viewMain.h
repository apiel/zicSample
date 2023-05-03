#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include "audioHandler.h"
#include "data.h"
#include "draw.h"
#include "grid.h"
#include "menu.h"
#include "patternSelector.h"
#include "progressBar.h"
#include "tempo.h"
#include "view.h"
#include "drawHeaderButtonValue.h"

#define OPTIMIZED true

#define GRID_PATTERN_TOP 69
#define GRID_PATTERN_ROW_H 15

class ViewMain : public View {
protected:
    bool headerEditMode = false;
    Grid grid = Grid(APP_TRACKS + 1, APP_TRACK_STEPS + 1);
    Grid gridEdit = Grid(3, 5);
    ProgressBar& progressBar = ProgressBar::get();
    Data& data = Data::get();
    AudioHandler& audio = AudioHandler::get();
    Menu& menu = Menu::get();
    PatternSelector& patternSelector = PatternSelector::get();

    HeaderButtonValue headerButtonValue = HeaderButtonValue();

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

    bool isName() { return headerEditMode && (gridEdit.is(0, 0) || gridEdit.is(1, 0)); }
    bool isVolume() { return headerEditMode && gridEdit.is(0, 1); }
    bool isCutoff() { return headerEditMode && gridEdit.is(0, 2); }
    bool isResonance() { return headerEditMode && gridEdit.is(0, 3); }
    bool isSample() { return headerEditMode && gridEdit.is(0, 4); }
    bool isDelay() { return headerEditMode && gridEdit.is(1, 1); }
    bool isReverb() { return headerEditMode && gridEdit.is(1, 2); }
    bool isDistortion() { return headerEditMode && gridEdit.is(1, 3); }
    bool isBpm() { return headerEditMode && gridEdit.is(2, 0); }
    bool isStepStatus() { return headerEditMode && gridEdit.is(2, 1); }
    bool isStepVelocity() { return headerEditMode && gridEdit.is(2, 2); }
    bool isStepCondition() { return headerEditMode && gridEdit.is(2, 3); }

    Track& getTrack(int8_t gridRow) { return data.tracks[gridRow]; }

    void renderHeaderPattern(bool clear = false)
    {
        if (clear) {
            drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 40 });
        }

        Track& track = getTrack();
        drawText({ 10, 10 }, track.name, COLOR_INFO);
        if (isName()) {
            drawRect({ 5, 5 }, { 85, 30 }, COLOR_INFO);
        }

        unsigned int x = drawLabelValue({ 100, 5 }, "Volume:", (int)(track.volume * 100), "%", isVolume());
        x = drawLabelValue({ x + 5, 5 }, track.filter.getName(), track.filter.getPctValue(), "%", isCutoff());
        x = drawLabelValue({ x + 5, 5 }, "Res:", (int)(track.filter.resonance * 100), "%", isResonance());
        drawSelectableText(isSample(), { x + 5, 5 }, track.audioFileName, COLOR_INFO, 14);

        x = drawLabelValue({ 100, 22 }, "Delay:", 0, "%", isDelay());
        x = drawLabelValue({ x + 5, 22 }, "Reverb:", 0, "%", isReverb());
        x = drawLabelValue({ x + 5, 22 }, "Distortion:", (int)(track.distortion.drive * 100), "%", isDistortion());
    }

    void renderHeaderStep()
    {
        drawFilledRect({ 92, 45 }, { SCREEN_W - 97, 20 }, COLOR_STEP_HEADER);
        if (grid.col != 0) {
            Step& step = getTrack().steps[grid.col - 1];
            unsigned int x = step.enabled ? drawText({ 100, 47 }, "ON  ", COLOR_STEP, 16, APP_FONT_BOLD)
                                          : drawText({ 100, 47 }, "OFF", COLOR_INFO);
            if (isStepStatus()) {
                drawRect({ 98, 46 }, { 36, 18 }, COLOR_INFO);
            }
            x = drawLabelValue({ x + 5, 47 }, "Velocity:", (int)(step.velocity * 100), "%", isStepVelocity());
            drawLabelValue({ x + 5, 47 }, "Condition:", STEP_CONDITIONS[step.condition], NULL, isStepCondition());
        }
    }

    void renderBPM(bool clear = false)
    {
        if (clear) {
            drawFilledRect({ 5, 40 }, { 85, 25 }, COLOR_FOREGROUND);
        }
        char bpm[4];
        sprintf(bpm, "%d", audio.tempo.getBpm());
        unsigned int x = drawText({ 10, 40 }, bpm, COLOR_INFO, 22, APP_FONT_BOLD);
        drawText({ x + 5, 45 }, "BPM", COLOR_LABEL, 10);
        if (headerEditMode && gridEdit.is(2, 0)) {
            drawRect({ 5, 40 }, { 85, 25 }, COLOR_INFO);
        }
    }

    void renderHeader(bool optimized = false)
    {
        if (!optimized) {
            drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 60 });
        }
        if (!optimized || gridEdit.lastRow == 0 || gridEdit.lastRow == 1 || gridEdit.row == 0 || gridEdit.row == 1) {
            renderHeaderPattern(optimized);
        }
        if (!optimized || gridEdit.lastRow == 2 || gridEdit.row == 2) {
            renderHeaderStep();
        }
    }

    void renderHeaderMaster()
    {
        drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 60 });
        drawLabelValue({ 10, 10 }, "Vol:", (int)(audio.getVolume() * 100), "%", isVolume(), 20);
        renderBPM();

        drawLabelValue({ 100, 5 }, "Res:", (int)(audio.filter.resonance * 100), "%", isResonance());
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

    void handleHeader(UiKeys& keys)
    {
        if (isVolume()) {
            handleVolume(keys.getDirection(0.01));
        } else if (isStepStatus()) {
            Track& track = getTrack();
            Step& step = track.steps[grid.col - 1];
            step.enabled = !step.enabled;
            renderHeaderStep();
            renderStep(track, grid.col - 1, grid.row);
        } else if (isStepVelocity()) {
            handleStepVelocity(keys.getDirection());
        } else if (isStepCondition()) {
            handleStepCondition(keys.getOneDirection());
        } else if (isBpm()) {
            // FIXME
            audio.tempo.set(audio.tempo.getBpm() + keys.getDirection());
            renderBPM(CLEAR);
        } else if (isCutoff()) {
            handleCutoff(keys.getDirection(10, 50));
        } else if (isResonance()) {
            handelResonance(keys.getDirection(0.01));
        } else if (isDistortion()) {
            Track& track = getTrack();
            track.distortion.set(track.distortion.drive + keys.getDirection(0.01));
            renderHeaderPattern(CLEAR);
        } else if (isName()) {
            patternSelector.show(&getTrack());
        } else if (isSample()) {
            Track& track = getTrack();
            track.setNextAudioFileName(keys.getOneDirection());
            renderHeaderPattern(CLEAR);
        } else {
            return;
        }
        draw();
    }

    void handelResonance(float direction)
    {
        Track& track = getTrack();
        track.filter.setResonance(track.filter.resonance + direction);
        renderHeaderPattern(CLEAR);
    }

    void handleCutoff(int16_t direction)
    {
        Track& track = getTrack();
        track.filter.set(track.filter.value + direction);
        renderHeaderPattern(CLEAR);
    }

    void handleVolume(float direction)
    {
        Track& track = getTrack();
        track.setVolume(track.volume + direction);
        renderHeaderPattern(CLEAR);
    }

    void handleStepVelocity(int8_t direction)
    {
        Track& track = getTrack();
        Step& step = track.steps[grid.col - 1];
        step.setVelocity(step.velocity + direction * 0.01);
        renderHeaderStep();
        renderStep(track, grid.col - 1, grid.row);
    }

    void handleStepCondition(int8_t direction)
    {
        Track& track = getTrack();
        Step& step = track.steps[grid.col - 1];
        step.setCondition(step.condition + direction);
        renderHeaderStep();
        renderStep(track, grid.col - 1, grid.row);
    }

    void handleMain(UiKeys& keys)
    {
        if (grid.row == APP_TRACKS) {
            audio.setVolume(audio.getVolume() + keys.getDirection(0.05, 0.1));
            renderMasterVolume(true);
            renderHeaderMaster();
        } else if (grid.col == 0) {
            if (keys.Right) {
                handleVolume(0.05);
                renderTrackName(getTrack(), grid.row);
            } else if (keys.Left) {
                handleVolume(-0.05);
                renderTrackName(getTrack(), grid.row);
            } else if (keys.Up) {
                handleCutoff(50);
            } else if (keys.Down) {
                handleCutoff(-50);
            }
            return;
        } else {
            if (keys.Right) {
                handleStepCondition(1);
            } else if (keys.Left) {
                handleStepCondition(-1);
            } else if (keys.Up) {
                handleStepVelocity(5);
            } else if (keys.Down) {
                handleStepVelocity(-5);
            }
            return;
        }
        draw();
    }

    void handleMainEdit2(UiKeys& keys)
    {
        if (grid.row == APP_TRACKS) {
            audio.filter.setResonance(audio.filter.resonance + keys.getDirection(0.01));
            renderHeaderMaster();
        } else if (grid.col == 0) {
            if (keys.Right) {
                handelResonance(0.01);
            } else if (keys.Left) {
                handelResonance(-0.01);
            } else if (keys.Up) {
                handleCutoff(50);
            } else if (keys.Down) {
                handleCutoff(-50);
            }
            return;
        } else {
            return;
        }
        draw();
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

        headerButtonValue.draw();

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

        if (keys.Edit) {
            if (headerEditMode) {
                handleHeader(keys);
            } else {
                handleMain(keys);
            }
            return;
        }

        if (keys.Edit2) {
            handleMainEdit2(keys);
            return;
        }

        if (headerEditMode) {
            if (gridEdit.update(keys) == VIEW_CHANGED) {
                fixGridEdit();
                renderHeader(OPTIMIZED);
                draw();
            }
        } else if (grid.update(keys) == VIEW_CHANGED) {
            renderSelection();
            if (grid.row == APP_TRACKS) {
                renderHeaderMaster();
            } else {
                if (grid.rowChanged()) {
                    if (grid.lastRow == APP_TRACKS) { // If last row was 0, fully re-render
                        renderHeader();
                    } else {
                        renderHeaderPattern(CLEAR);
                        renderHeaderStep();
                    }
                }
                if (grid.colChanged()) {
                    renderHeaderStep();
                }
            }
            draw();
            return;
        }

        if (keys.Mode) {
            headerEditMode = !headerEditMode;
            fixGridEdit();
            renderSelection();
            renderHeader(OPTIMIZED);
            draw();
            return;
        } else if (keys.Action) {
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
                renderHeaderStep();
                renderStep(track, step, grid.row);
                draw();
                return;
            }
        }
    }
};

ViewMain* ViewMain::instance = NULL;

#endif