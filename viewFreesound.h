#ifndef _VIEW_FREESOUND_H
#define _VIEW_FREESOUND_H

#include "draw.h"
#include "freesound.h"
#include "grid.h"
#include "view.h"
#include "audioPreview.h"

class ViewFreesound : public View {
protected:
    AudioPreview& audioPreview = AudioPreview::get();
    Freesound& data = Freesound::get();
    Grid grid = Grid(4, 2);
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
        unsigned int y = 35 + pos * (h + 5);
        drawFilledRect({ 5, y }, { SCREEN_W - 10, h }, COLOR_FOREGROUND);

        y += 5;
        drawFilledRect({ 7, y + 2 }, { 20, 20 }, COLOR_INFO_DARK);
        drawPlayButton({ 12, y + 7 }, { 10, 10 }, COLOR_ON);

        drawFilledRect({ 35, y + 2 }, { 20, 20 }, COLOR_INFO_DARK);
        drawDownloadButton({ 38, y + 3 }, { 13, 15 }, COLOR_INFO);

        drawText({ 65, y + 2 }, data.items[currentPos + pos].name, COLOR_INFO_LIGHT);

        y += 30;
        drawText({ 10, y }, data.items[currentPos + pos].tags, COLOR_INFO_DARK, 12);

        y += 15;
        char info[20];
        unsigned int x = drawText({ 10, y }, "Size: ", COLOR_INFO_DARK, 12);
        x = drawText({ x + 2, y }, data.items[currentPos + pos].filesizeStr, COLOR_INFO, 12);
        x = drawText({ x + 10, y }, "Duration: ", COLOR_INFO_DARK, 12);
        sprintf(info, "%.1fsec", data.items[currentPos + pos].duration);
        x = drawText({ x + 2, y }, info, COLOR_INFO, 12);
        x = drawText({ x + 10, y }, "Downloads: ", COLOR_INFO_DARK, 12);
        sprintf(info, "%d", data.items[currentPos + pos].num_downloads);
        x = drawText({ x + 2, y }, info, COLOR_INFO, 12);
        x = drawText({ x + 10, y }, "Rating: ", COLOR_INFO_DARK, 12);
        sprintf(info, "%.1f/5", data.items[currentPos + pos].avg_rating);
        drawText({ x + 2, y }, info, COLOR_INFO, 12);
    }

    void renderPreviousNext()
    {
        unsigned int y = 5 + 35 + 3 * (h + 5);
        if (strlen(data.previousUrl)) {
            drawFilledRect({ 7, y + 2 }, { 20, 20 }, COLOR_INFO_DARK);
            drawPreviousButton({ 12, y + 6 }, { 10, 10 }, COLOR_INFO);
        } else {
            drawFilledRect({ 7, y + 2 }, { 20, 20 }, COLOR_FOREGROUND);
            drawPreviousButton({ 12, y + 6 }, { 10, 10 }, COLOR_FOREGROUND2);
        }
        drawText({ 7, y + 25 }, "Prev", COLOR_INFO_DARK, 10);

        if (strlen(data.nextUrl)) {
            drawFilledRect({ 35, y + 2 }, { 20, 20 }, COLOR_INFO_DARK);
            drawNextButton({ 40, y + 6 }, { 10, 10 }, COLOR_INFO);
        } else {
            drawFilledRect({ 35, y + 2 }, { 20, 20 }, COLOR_FOREGROUND);
            drawNextButton({ 40, y + 6 }, { 10, 10 }, COLOR_FOREGROUND2);
        }
        drawText({ 35, y + 25 }, "Next", COLOR_INFO_DARK, 10);
    }

    void renderSelection(int8_t row, int8_t col, bool clear = false)
    {
        SDL_Color color = COLOR_INFO_LIGHT;
        if (clear) {
            color = COLOR_FOREGROUND;
        }
        unsigned int y = marginTop + 35 + row * (h + 5);

        if (col == 0) {
            drawRect({ 7, y + 2 }, { 20, 20 }, color);
        } else {
            drawRect({ 35, y + 2 }, { 20, 20 }, color);
        }
    }

    void fetchAndRender(char* url)
    {
        if (strlen(url)) {
            unsigned int y = 5 + 35 + 3 * (h + 5);
            drawFilledRect({ 5, y }, { SCREEN_W - 10, h }, COLOR_BACKGROUND);
            drawText({ 10, y + 2 }, "Loading...", COLOR_INFO, 12);
            draw();

            data.fetch(url);
            currentPos = 0;
            grid.row = 0;
            render();
            draw();
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
            if (currentPos + i == (unsigned int)data.getCount()) {
                renderPreviousNext();
            } else {
                renderItem(i);
            }
        }

        renderSelection(grid.row, grid.col);

        draw();
    }

    void handle(UiKeys& keys)
    {
        if (keys.Menu) {
            ui.view = VIEW_MAIN;
            ui.needMainViewRender = true;
        } else if (grid.update(keys) == VIEW_CHANGED) {
            if (grid.rowChanged() || grid.colChanged()) {
                renderSelection(grid.lastRow, grid.lastCol, CLEAR);
                renderSelection(grid.row, grid.col);
                draw();
            } else if (grid.row == 0) {
                if (currentPos > 0) {
                    currentPos--;
                    render();
                    draw();
                }
            } else if (currentPos + 4 <= data.getCount()) {
                currentPos++;
                render();
                draw();
            }
        } else if (keys.Action || keys.Edit) {
            if (grid.row == 3 && currentPos + 3 == data.getCount()) {
                // APP_LOG("PrevNext page: r %d p %d c %d\n", grid.row, currentPos, data.getCount());
                if (grid.col == 0) {
                    fetchAndRender(data.previousUrl);
                } else {
                    fetchAndRender(data.nextUrl);
                }
            } else if (grid.col == 0) {
                APP_LOG("Download: %s\n", data.items[currentPos + grid.row].preview_hq_ogg);
                data.download(data.items[currentPos + grid.row].preview_hq_ogg, (char *)"samples/__preview.ogg");
                audioPreview.play((char *)"samples/__preview.ogg");
                APP_LOG("Downloaded\n");
            } else if (grid.col == 1) {
                APP_LOG("Download: %s\n", data.items[currentPos + grid.row].download);
                data.download(data.items[currentPos + grid.row].download, (char *)"samples/0.wav");
                APP_LOG("Downloaded\n");
            }
        }
    }
};

ViewFreesound* ViewFreesound::instance = NULL;

#endif