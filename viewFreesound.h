#ifndef _VIEW_FREESOUND_H
#define _VIEW_FREESOUND_H

#include "draw.h"
#include "freesound.h"
#include "grid.h"
#include "view.h"

class ViewFreesound : public View {
protected:
    Freesound& data = Freesound::get();

    int8_t cursorPos = 0;
    uint8_t currentPos = 0;

    static ViewFreesound* instance;

    ViewFreesound()
    {
        // FIXME might want to remove it
        data.init();
    }

    unsigned int h = 66;
    unsigned int marginTop = 5;

    void renderItem(unsigned int pos)
    {
        unsigned int marginLeft = 25;

        unsigned int y = 35 + pos * (h + 5);
        drawFilledRect({ 5, y }, { SCREEN_W - 10, h }, COLOR_FOREGROUND);
        y += marginTop;
        drawText({ 10 + marginLeft, y + 2 }, data.items[currentPos + pos].name, COLOR_INFO_LIGHT);
        drawText({ 10 + marginLeft, y + 22 }, data.items[currentPos + pos].tags, COLOR_INFO_DARK, 12);

        char info[20];
        unsigned int x = drawText({ 10 + marginLeft, y + 39 }, "Size: ", COLOR_INFO_DARK, 12);
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

        drawFilledRect({ 7, y + 2 }, { 20, 20 }, COLOR_INFO_DARK);
        drawPlayButton({ 12, y + 7 }, { 10, 10 }, COLOR_ON);

        drawFilledRect({ 7, y + 30 }, { 20, 20 }, COLOR_INFO_DARK);
        drawDownloadButton({ 10, y + 31 }, { 13, 15 }, COLOR_INFO);

        renderSelection();
    }

    void renderSelection(bool clear = false)
    {
        SDL_Color color = COLOR_INFO_LIGHT;
        if (clear) {
            color = COLOR_FOREGROUND;
        }
        unsigned int pos = cursorPos * 0.5;
        unsigned int y = marginTop + 35 + pos * (h + 5);

        if (cursorPos % 2 == 0) {
            drawRect({ 7, y + 2 }, { 20, 20 }, color);
        } else {
            drawRect({ 7, y + 30 }, { 20, 20 }, color);
        }
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

        drawFilledRect({ 5, 5 }, { SCREEN_W - 10, 25 }, COLOR_FREESOUND_HEADER);
        unsigned int x = drawText({ 10, 7 }, data.query, COLOR_WHITE, 18);
        char count[16];
        sprintf(count, "%d results", data.totalCount);
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
        } else if (keys.Up) {
            if (cursorPos > 0) {
                renderSelection(CLEAR);
                cursorPos--;
                renderSelection();
                draw();
            } else if (currentPos > 0) {
                currentPos--;
                render();
                draw();
            }
        } else if (keys.Down) {
            if (cursorPos < 7) {
                renderSelection(CLEAR);
                cursorPos++;
                renderSelection();
                draw();
            } else if (currentPos + 8 < data.getCount()) {
                currentPos++;
                render();
                draw();
            }
        }
    }
};

ViewFreesound* ViewFreesound::instance = NULL;

#endif