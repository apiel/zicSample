#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "def.h"
#include "draw.h"
#include "grid.h"

#ifndef KEYBOARD_VALUE_MAX
#define KEYBOARD_VALUE_MAX 256
#endif

enum {
    KEYBOARD_DRAW,
    KEYBOARD_SAVED,
    KEYBOARD_CANCELED,
};

class Keyboard {
protected:
    unsigned int x = 120;
    unsigned int y = 30;
    uint16_t h = SCREEN_H - (y * 2);
    unsigned int w = SCREEN_W - (x * 2);

    Grid grid = Grid(8, 10);

    uint8_t selected = 0;

    char* value = (char*)"undefined";
    char originalValue[KEYBOARD_VALUE_MAX];
    uint8_t length = 0;

    static Keyboard* instance;

    Keyboard() { }

    void fixGrid()
    {
        if (grid.row == 7) {
            if (grid.lastRow != 7) {
                if (grid.lastCol < 3) {
                    grid.col = 0;
                } else if (grid.lastCol < 6) {
                    grid.col = 1;
                } else {
                    grid.col = 2;
                }
            } else if (grid.lastCol > 2) {
                grid.col = 2;
            }
        } else if (grid.lastRow == 7) {
            if (grid.lastCol == 0) {
                grid.col = 0;
            } else if (grid.lastCol == 1) {
                grid.col = 3;
            } else {
                grid.col = 6;
            }
        }
    }

    const char* alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-.!@$+&";

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

public:
    static Keyboard& get()
    {
        if (!instance) {
            instance = new Keyboard();
        }
        return *instance;
    }

    Keyboard& setTarget(char* target, uint8_t len)
    {
        length = range(len, 1, KEYBOARD_VALUE_MAX);
        value = target;
        strncpy(originalValue, value, length);
        grid.reset();

        return *this;
    }

    void restore()
    {
        strncpy(value, originalValue, length);
    }

    void render()
    {
        drawFilledRect({ x, y }, { w, h }, COLOR_FOREGROUND);
        drawRect({ x, y }, { w, h }, COLOR_INFO);

        drawFilledRect({ x + 1, y + 1 }, { w - 2, 25 }, COLOR_FOREGROUND2);
        if (strlen(value) > 0) {
            drawText({ x + 20, y + 5 }, value, COLOR_MENU);
        }

        for (uint8_t i = 0; i < 7; i++) {
            for (uint8_t j = 0; j < 10; j++) {
                char c[2];
                c[0] = alphanum[(i * 10) + j];
                c[1] = '\0';
                drawText({ x + 20 + (j * 20), y + 30 + (i * 25) }, c, COLOR_INFO);
            }
        }

        drawText({ x + 20, y + 30 + (7 * 25) }, "Save   Cancel   Backspace", COLOR_INFO);

        renderSelection();
    }

    uint8_t handle(UiKeys& keys)
    {
        if (grid.update(keys) == VIEW_CHANGED) {
            fixGrid();
            renderSelection();
            draw();
        } else if (keys.Action || keys.Edit) {
            if (grid.row < 7) {
                if (strlen(value) < length) {
                    char c[2];
                    c[0] = alphanum[(grid.row * 10) + grid.col];
                    c[1] = '\0';
                    strcat(value, c);
                    render();
                    draw();
                }
            } else if (grid.col == 0) {
                return KEYBOARD_SAVED;
            } else if (grid.col == 1) {
                restore();
                return KEYBOARD_CANCELED;
            } else {
                value[strlen(value) - 1] = '\0';
                render();
                draw();
            }
        } else if (keys.Edit2) {
            value[strlen(value) - 1] = '\0';
            render();
            draw();
        }
        return KEYBOARD_DRAW;
    }
};

Keyboard* Keyboard::instance = NULL;

#endif