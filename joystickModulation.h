#ifndef _JOYSTICK_MODULATION_H_
#define _JOYSTICK_MODULATION_H_

#include "def.h"
#include "viewMain.h"

ViewMain& viewMain = ViewMain::get();

unsigned long lastTrackFilterModulation = 0;

void joystickModulation(UiJoysticks& joysticks)
{
    // if ((joysticks.X1Changed || joysticks.Y1Changed) && (SDL_GetTicks() - lastTrackFilterModulation > 50)) {
    //     lastTrackFilterModulation = SDL_GetTicks();
    //     joysticks.Y1Changed = false;
    //     joysticks.X1Changed = false;
    //     Track& track = viewMain.getTrack();
    //     track.filter.setJoystickMod(joysticks.Y1, joysticks.X1);
    //     // printf("joystickModulation: %f, %f\n", joysticks.Y1, joysticks.X1);
    // }
}

#endif