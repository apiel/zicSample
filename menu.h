#ifndef _MENU_H_
#define _MENU_H_

#include "def.h"
#include "draw.h"

const char* MENU_ITEMS[] = {
    "Save track",
    "Save track as",
    "Reload track",
    "Exit",
};

uint8_t MENU_ITEMS_COUNT = sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0]);

class Menu {
protected:
    uint8_t x = 120;
    uint8_t y = 75;
    uint16_t h = SCREEN_H - (y + 5);
    uint16_t w = SCREEN_W - (x * 2);

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
        SDL_Color color = COLOR_FOREGROUND2;
        color.a = 235;
        drawFilledRect({ x, y }, { w, h }, color);
        drawRect({ x, y }, { w, h }, COLOR_INFO);

        for (uint8_t i = 0; i < MENU_ITEMS_COUNT; i++) {
            drawText({ (unsigned int)x + 20, (unsigned int)y + 5 + (i * 25) }, MENU_ITEMS[i], COLOR_MENU);
        }
    }

    bool toggle()
    {
        isVisible = !isVisible;

        return isVisible;
    }

    void handle(UiKeys& keys)
    {
    }
};

Menu* Menu::instance = NULL;

#endif