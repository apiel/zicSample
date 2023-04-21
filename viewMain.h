#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include "audioHandler.h"
#include "data.h"
#include "draw.h"
#include "progressBar.h"
#include "tempo.h"
#include "view.h"

#define CLEAR true
#define OPTIMIZED true

class ViewMain : public View {
protected:
    bool headerEditMode = false;
    Grid grid = Grid(APP_TRACKS + 1, APP_TRACK_STEPS + 1, 1);
    Grid gridEdit = Grid(3, 5);
    ProgressBar& progressBar = ProgressBar::get();
    Data& data = Data::get();
    AudioHandler& audio = AudioHandler::get();

    // 75 + 15 * row
    uint16_t rowY[APP_TRACKS + 1] = { 67, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300 };

    ViewMain() { }

    void fixGridEdit()
    {
        if (gridEdit.row == 1 && gridEdit.col == 0 && gridEdit.lastRow == 0 && gridEdit.lastCol == 0) {
            gridEdit.row = 2;
        } else if (gridEdit.row == 1 && gridEdit.col > 3) {
            gridEdit.col = 3;
        } else if (grid.col == 0 && gridEdit.row == 2 && gridEdit.col > 0) {
            gridEdit.col = 0;
        } else if (gridEdit.row == 2 && gridEdit.col > 3) {
            gridEdit.col = 3;
        }
    }

    void renderSlection(int8_t row, int8_t col, SDL_Color color = COLOR_SELECTOR)
    {
        unsigned int y = rowY[row];
        uint8_t h = 14;
        if (row == 0) {
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

    void renderSlection()
    {
        renderSlection(grid.lastRow, grid.lastCol, COLOR_BACKGROUND);
        renderSlection(grid.row, grid.col);
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
        renderSlection();
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

    Track& getTrack() { return getTrack(grid.row); }
    Track& getTrack(int8_t gridRow) { return data.tracks[gridRow - 1]; }

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
        drawSelectableText(isSample(), { x + 5, 5 }, track.sample, COLOR_INFO, 14);

        x = drawLabelValue({ 100, 22 }, "Delay:", 0, "%", isDelay());
        x = drawLabelValue({ x + 5, 22 }, "Reverb:", 0, "%", isReverb());
        x = drawLabelValue({ x + 5, 22 }, "Distortion:", 0, "%", isDistortion());
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
    }

    void renderMasterVolume(bool selected = false)
    {
        drawFilledRect({ 4, 66 }, { 86, 7 }, COLOR_BACKGROUND);
        SDL_Color color = COLOR_ON;
        color.a = 100;
        drawFilledRect({ 5, 67 }, { 84, 5 }, color);
        color.a = 200;
        unsigned int width = 84.0 * audio.getVolume() / APP_MAX_VOLUME;
        drawFilledRect({ 5, 67 }, { width, 5 }, color);
        if (selected) {
            drawRect({ 4, 66 }, { 86, 7 }, COLOR_WHITE);
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
            audio.tempo.set(audio.tempo.getBpm() + keys.getDirection());
            renderBPM(CLEAR);
        } else if (isCutoff()) {
            handleCutoff(keys.getDirection(10, 50));
        } else if (isResonance()) {
            handelResonance(keys.getDirection(0.01));
        } else if (isSample()) {
            Track& track = getTrack();
            track.setNextSample(keys.getOneDirection());
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
        if (grid.row == 0) {
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
        if (grid.row == 0) {
            return;
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

        renderHeader();

        for (unsigned int row = 1; row < APP_TRACKS + 1; row++) {
            renderRow(row);
        }

        draw();
    }

    void update(UiKeys& keys)
    {
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
            renderSlection();
            if (grid.row == 0) {
                renderHeaderMaster();
            } else {
                if (grid.rowChanged()) {
                    renderHeaderPattern(CLEAR);
                    renderHeaderStep();
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
            renderSlection();
            renderHeader(OPTIMIZED);
            draw();
            return;
        } else if (keys.Action) {
            if (grid.row == 0) {
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