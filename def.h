#ifndef _DEF_H_
#define _DEF_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

#ifndef APP_LOG
#define APP_LOG printf
#endif

#ifndef APP_SAMPLES_FOLDER
#define APP_SAMPLES_FOLDER "./samples"
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

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 44100
#endif

#ifndef APP_CHANNELS
#define APP_CHANNELS 2
#endif

#ifndef APP_AUDIO_FORMAT
// #define APP_AUDIO_FORMAT AUDIO_S16LSB
#define APP_AUDIO_FORMAT AUDIO_F32LSB
#endif

#ifndef APP_AUDIO_CHUNK
// #define APP_AUDIO_CHUNK 1024
#define APP_AUDIO_CHUNK 128
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

// #ifndef COLOR_SELECTOR // #ffb6b6
// #define COLOR_SELECTOR { 0xff, 0xb6, 0xb6, 255 }
// #endif

#ifndef COLOR_SELECTOR // #BBBBBB
#define COLOR_SELECTOR { 0xBB, 0xBB, 0xBB, 255 }
#endif

// #ifndef COLOR_SELECTOR // #0d6efd
// #define COLOR_SELECTOR { 0x0d, 0x6e, 0xfd, 255 }
// #endif

#ifndef COLOR_STEP // #3761a1
#define COLOR_STEP { 0x37, 0x61, 0xa1, 255 }
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

#ifndef APP_SAMPLE_NAME
#define APP_SAMPLE_NAME 50
#endif

#ifndef APP_DEFAULT_FONT_SIZE
#define APP_DEFAULT_FONT_SIZE 16
#endif

#define SCREEN_W 480
#define SCREEN_H 320

#ifndef isBetween
#define isBetween(x, a, b) (((a) <= (x)) && ((x) <= (b)))
#endif

#ifndef range
#define range(x, _min, _max) ((x) < (_max) ? ((x) > (_min) ? (x) : (_min)) : (_max))
#endif

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

    UiKeys(uint8_t keysBin)
    {
        Up = (keysBin >> UI_KEY_UP) & 1;
        Down = (keysBin >> UI_KEY_DOWN) & 1;
        Left = (keysBin >> UI_KEY_LEFT) & 1;
        Right = (keysBin >> UI_KEY_RIGHT) & 1;
        Edit = (keysBin >> UI_KEY_EDIT) & 1;
        Menu = (keysBin >> UI_KEY_MENU) & 1;
        Action = (keysBin >> UI_KEY_ACTION) & 1;
    }

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