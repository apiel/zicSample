#ifndef _DEF_H_
#define _DEF_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

#ifndef APP_LOG
#define APP_LOG printf
#endif

#ifndef APP_DATA_FOLDER
#define APP_DATA_FOLDER "./data"
#endif

#ifndef APP_DATA_MAIN
#define APP_DATA_MAIN APP_DATA_FOLDER "/__main__"
#endif

#ifndef APP_FONT
#define APP_FONT "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf"
#endif

#ifndef APP_FONT_BOLD
#define APP_FONT_BOLD "/usr/share/fonts/truetype/liberation2/LiberationSans-Bold.ttf"
#endif

#ifndef APP_FONT_ITALIC
#define APP_FONT_ITALIC "/usr/share/fonts/truetype/liberation2/LiberationSans-Italic.ttf"
#endif

// clang-format off

#ifndef COLOR_WHITE
#define COLOR_WHITE { 255, 255, 255, 255 }
#endif

#ifndef COLOR_BACKGROUND // #21252b
#define COLOR_BACKGROUND { 0x21, 0x25, 0x2b, 255 }
#endif

#ifndef COLOR_FOREGROUND // #2b2c2e
#define COLOR_FOREGROUND { 0x2b, 0x2c, 0x2e, 255 }
#endif

#ifndef COLOR_FOREGROUND2 // #383a3d
#define COLOR_FOREGROUND2 { 0x38, 0x3a, 0x3d, 255 }
#endif

#ifndef COLOR_PRIMARY // #0d6efd
#define COLOR_PRIMARY { 0x0d, 0x6e, 0xfd, 255 }
#endif

#ifndef COLOR_INFO // #888888
#define COLOR_INFO { 0x88, 0x88, 0x88, 255 }
#endif

#ifndef APP_DEFAULT_FONT_COLOR
#define APP_DEFAULT_FONT_COLOR COLOR_WHITE
#endif

// clang-format on

#ifndef APP_TRACKS
#define APP_TRACKS 16
#endif

#ifndef APP_TRACK_STEPS
#define APP_TRACK_STEPS 32
#endif

#ifndef APP_TRACK_NAME
#define APP_TRACK_NAME 12
#endif

#ifndef APP_DEFAULT_FONT_SIZE
#define APP_DEFAULT_FONT_SIZE 16
#endif

#define SCREEN_W 480
#define SCREEN_H 320

struct Size {
    uint32_t w;
    uint32_t h;
};

struct Point {
    uint32_t x;
    uint32_t y;
};

enum {
    VIEW_NONE,
    VIEW_CHANGED,
    VIEW_STATE_CHANGED,
};

enum {
    UI_KEY_UP,
    UI_KEY_DOWN,
    UI_KEY_LEFT,
    UI_KEY_RIGHT,
    UI_KEY_EDIT,
    UI_KEY_MENU,
    UI_KEY_ACTION,
    UI_KEY_COUNT
};

class UiKeys {
public:
    bool Up = false;
    bool Down = false;
    bool Right = false;
    bool Left = false;
    bool Edit = false;
    bool Menu = false;
    bool Action = false;

    int8_t getDirection()
    {
        if (Right) {
            return 1;
        } else if (Up) {
            return 10;
        } else if (Left) {
            return -1;
        } else if (Down) {
            return -10;
        }
        return 0;
    }

    float getDirection(float step)
    {
        return getDirection() * step;
    }

    int8_t getOneDirection()
    {
        if (Right || Up) {
            return 1;
        } else if (Left || Down) {
            return -1;
        }
        return 0;
    }
};

#endif