#ifndef _MENU_H_
#define _MENU_H_

#include "def.h"
#include "draw.h"
#include "grid.h"
#include "track.h"
#include "fs.h"

const char* MENU_ITEMS[] = {
    "Save track",
    "Save track as",
    "Reload track",
    "Delete track",
    "Exit",
};

uint8_t MENU_ITEMS_COUNT = sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0]);

class Menu {
protected:
    unsigned int x = 120;
    unsigned int y = 30;
    uint16_t h = SCREEN_H - (y * 2);
    unsigned int w = SCREEN_W - (x * 2);

    Grid grid = Grid(8, 10);

    uint8_t selected = 0;

    Track* isSaveAs = NULL;
    char saveAsOriginalName[APP_TRACK_NAME + 1];

    Menu() { }

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

    void renderSaveAs()
    {
        char name[APP_TRACK_NAME + 1];
        memset(name, 0, sizeof(name));
        strncpy(name, isSaveAs->name, sizeof(name) - 1);

        drawFilledRect({ x + 1, y + 1 }, { w - 2, 25 }, COLOR_FOREGROUND2);
        drawText({ x + 20, y + 5 }, name, COLOR_MENU);

        if (fileExists(isSaveAs->getFilePath())) {
            drawText({ x + 180, y + 8 }, "overwrite", COLOR_INFO, 11);
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
    bool isVisible = false;

    static Menu* instance;

    static Menu& get()
    {
        if (!instance) {
            instance = new Menu();
        }
        return *instance;
    }

    void render()
    {
        drawFilledRect({ x, y }, { w, h }, COLOR_FOREGROUND);
        drawRect({ x, y }, { w, h }, COLOR_INFO);

        if (isSaveAs) {
            renderSaveAs();
            return;
        }

        for (uint8_t i = 0; i < MENU_ITEMS_COUNT; i++) {
            drawText({ x + 20, y + 5 + (i * 25) }, MENU_ITEMS[i], COLOR_MENU);
            if (i == selected) {
                drawFilledRect({ x + 5, y + 5 + (i * 25) }, { 10, 18 }, COLOR_INFO);
            }
        }
    }

    bool toggle()
    {
        isVisible = !isVisible;
        if (isSaveAs) {
            strcpy(isSaveAs->name, saveAsOriginalName);
        }
        isSaveAs = NULL;

        return isVisible;
    }

    bool handleSaveAs(UiKeys& keys, Track& track)
    {
        if (grid.update(keys) == VIEW_CHANGED) {
            fixGrid();
            renderSelection();
            draw();
        } else if (keys.Action || keys.Edit) {
            if (grid.row < 7) {
                if (strlen(isSaveAs->name) < APP_TRACK_NAME) {
                    char c[2];
                    c[0] = alphanum[(grid.row * 10) + grid.col];
                    c[1] = '\0';
                    strcat(isSaveAs->name, c);
                    renderSaveAs();
                    draw();
                }
            } else if (grid.col == 0) {
                isSaveAs->save();
                isSaveAs = NULL;
                render();
                draw();
                return true;
            } else if (grid.col == 1) {
                strcpy(isSaveAs->name, saveAsOriginalName);
                isSaveAs = NULL;
                render();
                draw();
                return false;
            } else {
                isSaveAs->name[strlen(isSaveAs->name) - 1] = '\0';
                renderSaveAs();
                draw();
            }
        } else if (keys.Edit2) {
            isSaveAs->name[strlen(isSaveAs->name) - 1] = '\0';
            renderSaveAs();
            draw();
        }
        return false;
    }

    bool handle(UiKeys& keys, Track& track)
    {
        if (isSaveAs) {
            return handleSaveAs(keys, track);
        }
        if (keys.Up) {
            selected--;
            if (selected < 0) {
                selected = MENU_ITEMS_COUNT - 1;
            }
            render();
            draw();
        } else if (keys.Down) {
            selected++;
            if (selected >= MENU_ITEMS_COUNT) {
                selected = 0;
            }
            render();
            draw();
        } else if (keys.Action || keys.Edit) {
            switch (selected) {
            case 0:
                track.save();
                break;
            case 1:
                isSaveAs = &track;
                strcpy(saveAsOriginalName, isSaveAs->name);
                render();
                draw();
                return false;
            case 2:
                track.load();
                break;
            case 4:
                SDL_Log("EXIT\n");
                exit(0);
                break;
            }
            toggle();
            return true;
        }
        return false;
    }
};

Menu* Menu::instance = NULL;

#endif