#ifndef _STATE_H_
#define _STATE_H_

#include "def.h"

typedef struct {
    UiKeys keys;
    uint8_t view = VIEW_MAIN;
    bool needMainViewRender = false;
    SDL_Joystick* joystick = NULL;
} Ui;

Ui ui;

SDL_Renderer* renderer = NULL;
bool needToRenderProgressBar = false;

#endif