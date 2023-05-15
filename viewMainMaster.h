#ifndef _VIEW_MAIN_MASTER_H
#define _VIEW_MAIN_MASTER_H

#include "data.h"
#include "draw.h"
#include "drawHeaderButtonValue.h"
#include "master.h"
#include "tempo.h"
#include "progressBar.h"

class ViewMainMaster {
protected:
    Master& master = Master::get();
    Tempo& tempo = Tempo::get();
    HeaderButtonValue headerButtonValue = HeaderButtonValue();
    ProgressBar& progressBar = ProgressBar::get();

public:
    void renderY()
    {
        headerButtonValue.btnY.label1 = "Volume";
        char volume[4];
        sprintf(volume, "%d", (int)(master.getVolume() * 100));
        headerButtonValue.btnY.value1 = volume;
        headerButtonValue.btnY.unit1 = "%";
        headerButtonValue.drawY();
    }

    void renderX()
    {
        headerButtonValue.btnX.label1 = "BPM";
        char bpm[4];
        sprintf(bpm, "%d", tempo.getBpm());
        headerButtonValue.btnX.value1 = bpm;
        headerButtonValue.drawX();
    }

    void renderA()
    {
        headerButtonValue.btnA.label2 = "Resonance";
        char resonance[4];
        sprintf(resonance, "%d", (int)(master.filter.resonance * 100));
        headerButtonValue.btnA.value2 = resonance;
        headerButtonValue.drawA();
    }

    void renderB()
    {
        headerButtonValue.drawB();
    }

    void render()
    {
        drawText({ 10, 10 }, "Master", COLOR_INFO);

        renderY();
        renderX();
        renderA();
        renderB();
    }

// TODO TODO
    void renderMasterVolume(bool selected = false)
    {
        drawFilledRect({ 4, progressBar.y - 1 }, { 86, progressBar.h + 2 }, COLOR_BACKGROUND);
        SDL_Color color = COLOR_ON;
        color.a = 100;
        drawFilledRect({ 5, progressBar.y }, { 84, progressBar.h }, color);
        color.a = 200;
        unsigned int width = 84.0 * master.getVolume() / APP_MAX_VOLUME;
        drawFilledRect({ 5, progressBar.y }, { width, progressBar.h }, color);
        if (selected) {
            drawRect({ 4, progressBar.y - 1 }, { 86, progressBar.h + 2 }, COLOR_WHITE);
        }
    }

    bool handle(UiKeys& keys)
    {
        if (keys.btnB) {
            return true;
        } else if (keys.btnA) {
            if (keys.isHorizontal()) {
                master.filter.setResonance(master.filter.resonance + keys.getHorizontal(0.01, 0.05));
                renderA();
                draw();
                return true;
            } else if (keys.isVertical()) {
                return true;
            }
        } else if (keys.btnX) {
            if (keys.isVertical()) {
                tempo.set(tempo.getBpm() + keys.getVertical(5, 1));
                renderX();
                draw();
                return true;
            } else if (keys.isHorizontal()) {
                return true;
            }
        } else if (keys.btnY) {
            if (keys.isVertical()) {
                master.setVolume(master.getVolume() + keys.getVertical(0.05, 0.01));
                renderY();
                renderMasterVolume();
                draw();
                return true;
            } else if (keys.isHorizontal()) {
                return true;
            }
        }
        return false;
    }
};

#endif