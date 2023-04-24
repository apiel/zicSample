#ifndef _JOYSTICK_MODULATION_H_
#define _JOYSTICK_MODULATION_H_

#include "def.h"
#include "viewMain.h"

ViewMain& viewMain = ViewMain::get();

unsigned long lastTrackFilterModulation = 0;

void joystickModulation(UiJoysticks& joysticks)
{
    if ((joysticks.X1Changed || joysticks.Y1Changed) && (SDL_GetTicks() - lastTrackFilterModulation > 100000)) {
        Track& track = viewMain.getTrack();
        track.filter.setJoystickMod(joysticks.Y1, joysticks.X1);
        joysticks.Y1Changed = false;
        joysticks.X1Changed = false;
        lastTrackFilterModulation = SDL_GetTicks();
    }
}

#endif