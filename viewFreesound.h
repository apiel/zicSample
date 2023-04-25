#ifndef _VIEW_FREESOUND_H
#define _VIEW_FREESOUND_H

#include "freesound.h"
#include "draw.h"
#include "view.h"

class ViewFreesound : public View {
protected:
    Freesound& data = Freesound::get();

    uint8_t currentPos = 0;

    static ViewFreesound* instance;

    ViewFreesound() {
        // FIXME might want to remove it
        data.init();
    }

    // int filesize;
    // float duration;
    // int num_downloads;
    // float avg_rating;

    void renderItem(unsigned int pos)
    {
        unsigned int y = 35 + pos * 75;
        drawFilledRect({ 5, y }, { SCREEN_W - 10 , 70 }, COLOR_FOREGROUND);
        drawText({ 10, y + 2 }, data.items[currentPos + pos].name, COLOR_INFO_LIGHT);
        drawText({ 10, y + 22 }, data.items[currentPos + pos].tags, COLOR_INFO_DARK, 12);

        char info[20];
        unsigned int x = drawText({ 10, y + 39 }, "Size: ", COLOR_INFO_DARK, 12);
        x = drawText({ x + 2, y + 39 }, data.items[currentPos + pos].filesizeStr, COLOR_INFO, 12);
        x = drawText({ x + 10, y + 39 }, "Duration: ", COLOR_INFO_DARK, 12);
        sprintf(info, "%.1fsec", data.items[currentPos + pos].duration);
        x = drawText({ x + 2, y + 39 }, info, COLOR_INFO, 12);
        x = drawText({ x + 10, y + 39 }, "Downloads: ", COLOR_INFO_DARK, 12);
        sprintf(info, "%d", data.items[currentPos + pos].num_downloads);
        x = drawText({ x + 2, y + 39 }, info, COLOR_INFO, 12);
        x = drawText({ x + 10, y + 39 }, "Rating: ", COLOR_INFO_DARK, 12);
        sprintf(info, "%.1f/5", data.items[currentPos + pos].avg_rating);
        drawText({ x + 2, y + 39 }, info, COLOR_INFO, 12);
    }

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

        drawFilledRect({ 5, 5 }, { SCREEN_W - 10 , 25 }, COLOR_FREESOUND_HEADER);
        unsigned int x = drawText({ 10, 7 }, data.query, COLOR_WHITE, 18);
        char count[16];
        sprintf(count, "%d", data.count);
        drawText({ x + 10, 13 }, count, COLOR_INFO, 12);

        for (unsigned int i = 0; i < 4; i++) {
            renderItem(i);
        }

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