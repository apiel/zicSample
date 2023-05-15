#ifndef _POPUP_MESSAGE_H_
#define _POPUP_MESSAGE_H_

#include "def.h"
#include "draw.h"

class PopupMessage {
protected:
    bool shown = false;

    static PopupMessage* instance;
    PopupMessage() { }

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

    void render() {
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
                render();
            } else if (now - shownTime > 2000) {
                // TODO restore state
                message = NULL;
            }
        }
    }
};

PopupMessage* PopupMessage::instance = NULL;

#endif