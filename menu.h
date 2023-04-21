#ifndef _MENU_H_
#define _MENU_H_

#include "def.h"
#include "draw.h"
#include "track.h"

const char* MENU_ITEMS[] = {
    "Save track",
    "Save track as",
    "Reload track",
    "Exit",
};

uint8_t MENU_ITEMS_COUNT = sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0]);

class Menu {
protected:
    unsigned int x = 120;
    unsigned int y = 75;
    uint16_t h = SCREEN_H - (y + 5);
    uint16_t w = SCREEN_W - (x * 2);

    uint8_t selected = 0;

    Menu() { }

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

        return isVisible;
    }

    bool handle(UiKeys& keys, Track& track)
    {
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
                // case 1:
                //     track.saveAs();
                //     break;
                case 2:
                    track.load();
                    break;
                case 3:
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