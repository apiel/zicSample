#ifndef _PATTERN_SELECTOR_H_
#define _PATTERN_SELECTOR_H_

#include "def.h"
#include "draw.h"
#include "fs.h"
#include "grid.h"
#include "track.h"

class PatternSelector {
protected:
    unsigned int x = 20;
    unsigned int y = 22;
    uint16_t h = SCREEN_H - (y * 2);
    unsigned int w = SCREEN_W - (x * 2);

    Grid grid = Grid(8, 10);

    uint8_t selected = 0;

    Track* track = NULL;

    PatternSelector() { }

    void fixGrid()
    {
    }

    void renderSelection(int8_t row, int8_t col, SDL_Color color = COLOR_INFO)
    {
        unsigned int _y = y + 30 + (row * 25);
        if (row < 7) {
            unsigned int _x = x + 20 + (col * 20);
            drawRect({ _x - 3, _y - 3 }, { 18, 23 }, color);
        } else if (col == 0) {
            drawRect({ x + 18, _y - 3 }, { 42, 23 }, color);
        } else if (col == 1) {
            drawRect({ x + 65, _y - 3 }, { 60, 23 }, color);
        } else {
            drawRect({ x + 126, _y - 3 }, { 90, 23 }, color);
        }
    }

    void renderSelection()
    {
        renderSelection(grid.lastRow, grid.lastCol, COLOR_FOREGROUND);
        renderSelection(grid.row, grid.col);
    }

    void renderName(const char* name, unsigned int row, unsigned int col)
    {
        unsigned int _y = row * 16 + y + 4;
        unsigned int _x = col * 87 + x + 4;

        drawFilledRect({ _x, _y }, { 84, 12 }, COLOR_FOREGROUND2);
        drawText({ _x + 3, _y }, name, COLOR_INFO, 10);
    }

    void render()
    {
        drawFilledRect({ x, y }, { w, h }, COLOR_FOREGROUND);
        drawRect({ x, y }, { w, h }, COLOR_INFO);

        for (int row = 0; row < 17; row++) {
            for (int col = 0; col < 5; col++) {
                renderName("Pattern 1", row, col);
            }
        }
    }

public:
    static PatternSelector* instance;

    static PatternSelector& get()
    {
        if (!instance) {
            instance = new PatternSelector();
        }
        return *instance;
    }

    void show(Track* _track)
    {
        track = _track;
        render();
    }

    bool isVisible()
    {
        return track != NULL;
    }

    bool handle(UiKeys& keys)
    {
        if (keys.Edit) {
            track = NULL;
            return true;
        }
        return false;
    }
};

PatternSelector* PatternSelector::instance = NULL;

#endif