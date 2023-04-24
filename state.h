#ifndef _STATE_H_
#define _STATE_H_

#include "def.h"

typedef struct {
    UiKeys keys;
    UiJoysticks joysticks;
} Ui;

Ui ui;

SDL_Renderer* renderer = NULL;
bool needToRenderProgressBar = false;

#endif