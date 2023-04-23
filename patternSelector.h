#ifndef _PATTERN_SELECTOR_H_
#define _PATTERN_SELECTOR_H_

#include "def.h"
#include "draw.h"
#include "fs.h"
#include "grid.h"
#include "track.h"

class PatternSelector {
protected:
    int16_t count = 0;
    char names[APP_TRACK_LIST_MAX][APP_TRACK_NAME];

    unsigned int x = 20;
    unsigned int y = 22;
    uint16_t h = SCREEN_H - (y * 2);
    unsigned int w = SCREEN_W - (x * 2);

    Grid grid = Grid(17, 5);

    uint8_t selected = 0;
    uint16_t startPosition = 0;

    Track* track = NULL;

    PatternSelector()
    {
        load();
    }

    void renderSelection(int8_t row, int8_t col, SDL_Color color = COLOR_INFO)
    {
        unsigned int _y = row * 16 + y + 3;
        unsigned int _x = col * 87 + x + 3;
        drawRect({ _x, _y }, { 86, 14 }, color);
    }

    void renderSelection()
    {
        renderSelection(grid.lastRow, grid.lastCol, COLOR_FOREGROUND);
        renderSelection(grid.row, grid.col);
    }

    void render()
    {
        drawFilledRect({ x, y }, { w, h }, COLOR_FOREGROUND);
        drawRect({ x, y }, { w, h }, COLOR_INFO);

        for (int row = 0; row < grid.rows; row++) {
            for (int col = 0; col < grid.cols; col++) {
                unsigned int _y = row * 16 + y + 4;
                unsigned int _x = col * 87 + x + 4;
                SDL_Color color = COLOR_FOREGROUND2;

                uint16_t index = row * grid.cols + col;
                if (startPosition + index < count) {
                    SDL_Color textColor = COLOR_INFO;
                    if (strcmp(getName(index), track->name) == 0) {
                        color = COLOR_INFO;
                        textColor = COLOR_WHITE;
                    }
                    drawFilledRect({ _x, _y }, { 84, 12 }, color);
                    drawText({ _x + 3, _y }, getName(index), textColor, 10);
                } else {
                    color.a = 0x80;
                    drawFilledRect({ _x, _y }, { 84, 12 }, color);
                }
            }
        }
        renderSelection();
    }

    const char * getName(uint16_t index)
    {
        if (index < count) {
            return names[startPosition + index];
        }
        return NULL;
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
        uint16_t i = 0;
        for (; i < count; i++) {
            if (strcmp(names[i], track->name) == 0) {
                break;
            }
        }
        // SDL_Log("i: %d spot: %d\n", i, grid.rows * grid.cols);
        if (i < grid.rows * grid.cols) {
            startPosition = 0;
        } else {
            startPosition = i - ((grid.rows - 1) * grid.cols);
            startPosition = startPosition - startPosition % grid.cols;
        }

        grid.col = (i - startPosition) % grid.cols;
        grid.row = (i - startPosition) / grid.cols;

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
        if (keys.Action) {
            uint16_t index = grid.row * grid.cols + grid.col;
            if (index < count) {
                strcpy(track->name, getName(index));
                track->load();
                return true;
            }
        } else if (keys.Edit2) {
            uint16_t index = grid.row * grid.cols + grid.col;
            if (index < count) {
                strcpy(track->name, getName(index));
                track->load();
                render();
            }
        }

        if (grid.update(keys) == VIEW_CHANGED) {
            if (grid.row == grid.lastRow && grid.col == grid.lastCol) {
                if (keys.Up) {
                    if (startPosition > 0) {
                        startPosition -= grid.cols;
                        render();
                    }
                } else if (keys.Down) {
                    if (startPosition + grid.rows * grid.cols < count) {
                        startPosition += grid.cols;
                        render();
                    }
                }
            } else {
                renderSelection();
            }
            draw();
        }
        return false;
    }

    void load()
    {
        DIR* x = opendir(APP_DATA_FOLDER);
        if (x != NULL) {
            struct dirent* directory;
            count = 0;
            while (count < APP_TRACK_LIST_MAX && (directory = readdir(x)) != NULL) {
                if (!strcmp(directory->d_name, ".") == 0
                    && !strcmp(directory->d_name, "..") == 0
                    && !strcmp(directory->d_name, APP_DATA_MAIN_FILE) == 0) {
                    // printf("(%d) %s\n", count, directory->d_name);
                    strncpy(names[count], directory->d_name, APP_TRACK_NAME);
                    names[count][APP_TRACK_NAME - 1] = '\0';
                    count++;
                }
            }
            closedir(x);

            if (count > 1) {
                qsort(names, count, sizeof names[0], fileCompare);
            }
        }
        SDL_Log("Found %d patterns\n", count);

        // for (uint16_t i = 0; i < count; i++) {
        //     SDL_Log("# %s\n", names[i]);
        // }
    }
};

PatternSelector* PatternSelector::instance = NULL;

#endif