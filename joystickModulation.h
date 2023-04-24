#ifndef _JOYSTICK_MODULATION_H_
#define _JOYSTICK_MODULATION_H_

#include "def.h"
#include "viewMain.h"

ViewMain& viewMain = ViewMain::get();

void joystickModulation(UiJoysticks& joysticks)
{
    if (joysticks.X1Changed) {
        Track& track = viewMain.getTrack();
        track.filter.setJoystickResonanceMod(joysticks.X1);
        joysticks.X1Changed = false;
    }
    if (joysticks.Y1Changed) {
        Track& track = viewMain.getTrack();
        track.filter.setJoystickCutoffMod(joysticks.Y1);
        joysticks.Y1Changed = false;
    }
}

#endif