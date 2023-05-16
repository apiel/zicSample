#ifndef _POPUP_MESSAGE_H_
#define _POPUP_MESSAGE_H_

#include "def.h"
#include "draw.h"
#include "state.h"

class PopupMessage {
protected:
    bool shown = false;
    SDL_Texture* snapshot;

    static PopupMessage* instance;
    PopupMessage()
    {
        snapshot = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    }

public:
    const unsigned int h = 25;
    const unsigned int w = SCREEN_W - (5 * 2);
    const unsigned int x = 5;
    const unsigned int y = 5;

    unsigned long shownTime = 0;
    const char* message = NULL;

    static PopupMessage& get()
    {
        if (!instance) {
            instance = new PopupMessage();
        }
        return *instance;
    }

    void render()
    {
        drawFilledRect({ x, y }, { w, h }, COLOR_FOREGROUND2);
        drawRect({ x, y }, { w, h }, COLOR_INFO);
        drawText({ x + 5, y + 4 }, message, COLOR_INFO);
        draw();
    }

    void show(const char* msg, SDL_Color color = (SDL_Color)COLOR_INFO)
    {
        shown = false;
        message = msg;
    }

    void handleRendering(unsigned long now)
    {
        if (message) {
            if (shown == false) {
                shownTime = SDL_GetTicks();
                shown = true;

                SDL_SetRenderTarget(renderer, snapshot);
                SDL_Rect rect = { x, y, w, h };
                SDL_RenderCopy(renderer, texture, &rect, NULL);
                SDL_SetRenderTarget(renderer, texture);

                render();
            } else if (now - shownTime > 1000) {
                SDL_Rect rect = { x, y, w, h };
                SDL_RenderCopy(renderer, snapshot, NULL, &rect);
                message = NULL;
            }
        }
    }
};

PopupMessage* PopupMessage::instance = NULL;

#endif