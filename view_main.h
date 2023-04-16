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
        drawText({ 0, 0 }, "Hello World");
    }

    uint8_t update(UiKeys* keys)
    {
        return VIEW_NONE;
    }
};

ViewMain* ViewMain::instance = NULL;

#endif