#ifndef _VIEW_MAIN_MASTER_H
#define _VIEW_MAIN_MASTER_H

#include "data.h"
#include "draw.h"
#include "master.h"
#include "tempo.h"
#include "drawHeaderButtonValue.h"

class ViewMainMaster {
protected:
    Master& master = Master::get();
    Tempo& tempo = Tempo::get();
    HeaderButtonValue headerButtonValue = HeaderButtonValue();

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

    bool handle(UiKeys& keys)
    {
        if (keys.btnB) {
        } else if (keys.btnA) {
            if (keys.isHorizontal()) {
                master.filter.setResonance(master.filter.resonance + keys.getHorizontal(0.01, 0.05));
                renderA();
                draw();
                return true;
            }
        } else if (keys.btnX) {
            if (keys.isVertical()) {
                tempo.set(tempo.getBpm() + keys.getVertical(5, 1));
                renderX();
                draw();
                return true;
            }
        } else if (keys.btnY) {
            if (keys.isVertical()) {
                master.setVolume(master.getVolume() + keys.getVertical(0.05, 0.01));
                renderY();
                draw();
                return true;
            }
        }
        return false;
    }
};

#endif