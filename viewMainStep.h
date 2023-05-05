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

    bool handle(UiKeys& keys, Track& track, Step& step)
    {
        if (keys.btnB) {
            step.toggle();
            renderB(step);
            return true;
        } else if (keys.btnA) {
            if (keys.isVertical()) {
                step.setVelocity(step.velocity + keys.getVertical(0.05, 0.01));
                renderA(step);
                return true;
            } else if (keys.isHorizontal()) {
                step.setCondition(step.condition + keys.getHorizontal(1));
                renderA(step);
                return true;
            }
        } else if (keys.btnX) {
        } else if (keys.btnY) {
        }
        return false;
    }
};

#endif