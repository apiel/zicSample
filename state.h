#ifndef _STATE_H_
#define _STATE_H_

#include "def.h"

typedef struct {
    bool keysChanged = false;
    uint16_t keys = 0;
} Ui;

Ui ui;

SDL_Renderer* renderer = NULL;

#endif