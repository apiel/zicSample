#ifndef _MENU_H_
#define _MENU_H_

#include "def.h"
#include "draw.h"
#include "fs.h"
#include "grid.h"
#include "keyboard.h"
#include "track.h"
#ifdef FREESOUND_ENABLED
#include "viewFreesound.h"
#endif

const char* MENU_ITEMS[] = {
    "Save track",
    "Save track as",
    "Reload track",
    "Delete track",
#ifdef FREESOUND_ENABLED
    "Freesound.org",
#endif
    "Exit",
};

enum MenuItems {
    MENU_ITEM_SAVE_TRACK,
    MENU_ITEM_SAVE_TRACK_AS,
    MENU_ITEM_RELOAD_TRACK,
    MENU_ITEM_DELETE_TRACK,
#ifdef FREESOUND_ENABLED
    MENU_ITEM_FREESOUND,
#endif
    MENU_ITEM_EXIT,
};

uint8_t MENU_ITEMS_COUNT = sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0]);

class Menu {
protected:
    Keyboard& keyboard = Keyboard::get();

    unsigned int x = 120;
    unsigned int y = 30;
    uint16_t h = SCREEN_H - (y * 2);
    unsigned int w = SCREEN_W - (x * 2);

    Grid grid = Grid(8, 10);

    uint8_t selected = 0;

    Track* isSaveAs = NULL;

    static Menu* instance;

    Menu() { }

    bool renderSaveAsOverwrite()
    {
        if (fileExists(isSaveAs->getFilePath())) {
            drawText({ x + 180, y + 8 }, "overwrite", COLOR_INFO, 11);
            return true;
        }
        return false;
    }

public:
    bool isVisible = false;

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
            keyboard.render();
            renderSaveAsOverwrite();
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
            keyboard.restore();
        }
        isSaveAs = NULL;

        return isVisible;
    }

    bool handle(UiKeys& keys, Track& track)
    {
        if (isSaveAs) {
            uint8_t res = keyboard.handle(keys);
            if (res == KEYBOARD_CANCELED) {
                isSaveAs = NULL;
                render();
                draw();
            } else if (res == KEYBOARD_DRAW) {
                if (renderSaveAsOverwrite()) {
                    draw();
                }
            } else if (res == KEYBOARD_SAVED && strlen(isSaveAs->name) > 0 && strlen(isSaveAs->name) < APP_TRACK_NAME
                && strcmp(track.name, "-") != 0 && strcmp(track.name, ".") != 0 && strcmp(track.name, "..") != 0) {
                isSaveAs->save();
                isSaveAs = NULL;
                render();
                draw();
                return true;
            }
            return false;
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
        } else if (keys.btnB || keys.btnA) {
            switch (selected) {
            case MENU_ITEM_SAVE_TRACK:
                if (strcmp(track.name, "-") != 0) {
                    track.save();
                    break;
                }
            case MENU_ITEM_SAVE_TRACK_AS:
                isSaveAs = &track;
                keyboard.setTarget(isSaveAs->name, APP_TRACK_NAME).setWidth(w).setDoneButtonText("Save");
                render();
                draw();
                return false;
            case MENU_ITEM_RELOAD_TRACK:
                track.load();
                break;
#ifdef FREESOUND_ENABLED
            case MENU_ITEM_FREESOUND:
                toggle();
                ui.view = VIEW_FREESOUND;
                ViewFreesound::get().render();
                return false;
#endif
            case MENU_ITEM_EXIT:
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