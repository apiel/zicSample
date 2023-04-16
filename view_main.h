#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include "draw.h"
#include "view.h"

class ViewMain : public View {
protected:
    ViewMain() { }

public:
    static ViewMain* instance;

    static ViewMain* getInstance()
    {
        if (!instance) {
            instance = new ViewMain();
        }
        return instance;
    }

    void render()
    {
        drawClear();

        drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 60 });

        drawText({ 10, 10 }, "Hello World", COLOR_INFO);
    }

    uint8_t update(UiKeys* keys)
    {
        return VIEW_NONE;
    }
};

ViewMain* ViewMain::instance = NULL;

#endif