#ifndef _VIEW_FREESOUND_H
#define _VIEW_FREESOUND_H

#include "freesound.h"
#include "draw.h"
#include "view.h"

class ViewFreesound : public View {
protected:
    Freesound& data = Freesound::get();

    static ViewFreesound* instance;

    ViewFreesound() { }

public:
    static ViewFreesound& get()
    {
        if (!instance) {
            instance = new ViewFreesound();
        }
        return *instance;
    }

    void render()
    {
        drawClear();

        drawText({ 5, 5 }, "Freesound.org");

        draw();
    }

    void handle(UiKeys& keys)
    {
        if (keys.Menu) {
            ui.view = VIEW_MAIN;
            ui.needMainViewRender = true;
        }
    }
};

ViewFreesound* ViewFreesound::instance = NULL;

#endif