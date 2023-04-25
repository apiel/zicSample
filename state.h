#ifndef _STATE_H_
#define _STATE_H_

#include "def.h"

typedef struct {
    UiKeys keys;
    UiJoysticks joysticks;
    uint8_t view = VIEW_MAIN;
    bool needMainViewRender = false;
} Ui;

Ui ui;

SDL_Renderer* renderer = NULL;
bool needToRenderProgressBar = false;

#endif