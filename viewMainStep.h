#ifndef _VIEW_MAIN_STEP_H
#define _VIEW_MAIN_STEP_H

#include "data.h"
#include "draw.h"
#include "drawHeaderButtonValue.h"

class ViewMainStep {
protected:
    HeaderButtonValue headerButtonValue = HeaderButtonValue();

public:
    void renderY(Step& step)
    {
        headerButtonValue.drawY();
    }

    void renderX(Step& step)
    {
        headerButtonValue.drawX();
    }

    void renderA(Step& step)
    {
        headerButtonValue.btnA.label1 = "Velocity";
        char velocity[6];
        sprintf(velocity, "%d", (int)(step.velocity * 100));
        headerButtonValue.btnA.value1 = velocity;
        headerButtonValue.btnA.unit1 = "%";

        headerButtonValue.btnA.label2 = "Condition";
        headerButtonValue.btnA.value2 = STEP_CONDITIONS[step.condition];

        headerButtonValue.drawA();
    }

    void renderB(Step& step)
    {
        headerButtonValue.btnB.label1 = "Status";
        headerButtonValue.btnB.value1 = step.enabled ? "On" : "Off";

        headerButtonValue.drawB();
    }

    void render(Track& track, uint8_t stepIndex)
    {
        drawText({ 10, 10 }, track.name, COLOR_INFO);

        char stepNumber[4];
        sprintf(stepNumber, "%d", stepIndex + 1);
        uint32_t x = drawText({ 10, 40 }, "Step", COLOR_LABEL, 10);
        x = drawText({ x + 2, 35 }, stepNumber, COLOR_INFO);
        drawText({ x + 3, 37 }, "/32", COLOR_LABEL, 14);

        Step& step = track.steps[stepIndex];
        renderY(step);
        renderX(step);
        renderA(step);
        renderB(step);
    }

    bool handle(UiKeys& keys, Track& track, unsigned int y)
    {
        // if (keys.btnB) {
        //     track.toggle();
        //     renderB(track);
        //     renderName(track, y);
        //     draw();
        //     return true;
        // } else if (keys.btnA) {
        //     if (keys.isVertical()) {
        //         track.filter.set(track.filter.value + keys.getVertical(10, 50));
        //         renderA(track);
        //         draw();
        //         return true;
        //     } else if (keys.isHorizontal()) {
        //         track.filter.setResonance(track.filter.resonance + keys.getHorizontal(0.01, 0.05));
        //         renderA(track);
        //         draw();
        //         return true;
        //     }
        // } else if (keys.btnX) {
        //     printf("keys.btnX\n");
        //     track.setNextAudioFileName(keys.getOneDirection());
        //     renderX(track);
        //     draw();
        //     return true;
        // } else if (keys.btnY) {
        //     if (keys.isVertical()) {
        //         track.setVolume(track.volume + keys.getVertical(0.05, 0.01));
        //         renderY(track);
        //         renderName(track, y);
        //         draw();
        //         return true;
        //     } else if (keys.isHorizontal()) {
        //         track.distortion.set(track.distortion.drive + keys.getHorizontal(0.01, 0.05));
        //         renderY(track);
        //         draw();
        //         return true;
        //     }
        // }
        return false;
    }
};

#endif