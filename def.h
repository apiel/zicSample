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

#ifndef APP_DATA_MAIN_FILE
#define APP_DATA_MAIN_FILE "__main__"
#endif

#ifndef APP_DATA_MAIN
#define APP_DATA_MAIN APP_DATA_FOLDER "/" APP_DATA_MAIN_FILE
#endif

#ifndef APP_FONT
#ifdef IS_RPI
#define APP_FONT "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf"
#else
#define APP_FONT "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"
#endif
#endif

#ifndef APP_FONT_BOLD
#ifdef IS_RPI
#define APP_FONT_BOLD "/usr/share/fonts/truetype/liberation2/LiberationSans-Bold.ttf"
#else
#define APP_FONT_BOLD "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf"
#endif
#endif

#ifndef APP_FONT_ITALIC
#ifdef IS_RPI
#define APP_FONT_ITALIC "/usr/share/fonts/truetype/liberation2/LiberationSans-Italic.ttf"
#else
#define APP_FONT_ITALIC "/usr/share/fonts/truetype/liberation/LiberationSans-Italic.ttf"
#endif
#endif

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 44100
#endif

#ifndef APP_CHANNELS
#define APP_CHANNELS 2
// #define APP_CHANNELS 1
#endif

#ifndef APP_AUDIO_FORMAT
// #define APP_AUDIO_FORMAT AUDIO_S16LSB
#define APP_AUDIO_FORMAT AUDIO_F32LSB
#endif

#ifndef APP_AUDIO_CHUNK
// Seem like what rg350 expects
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

#ifndef COLOR_INFO_LIGHT // #CCCCCC
#define COLOR_INFO_LIGHT { 0xCC, 0xCC, 0xCC, 255 }
#endif

#ifndef COLOR_INFO // #888888
#define COLOR_INFO { 0x88, 0x88, 0x88, 255 }
#endif

#ifndef COLOR_INFO_DARK // #555555
#define COLOR_INFO_DARK { 0x55, 0x55, 0x55, 255 }
#endif

#ifndef COLOR_MENU // #AAAAAA
#define COLOR_MENU { 0xAA, 0xAA, 0xAA, 255 }
#endif

#ifndef COLOR_LABEL // #555555
#define COLOR_LABEL { 0x55, 0x55, 0x55, 255 }
#endif

#ifndef COLOR_ON // #00b300
#define COLOR_ON { 0x00, 0xb3, 0x00, 255 }
#endif

#ifndef COLOR_SELECTOR // #BBBBBB
#define COLOR_SELECTOR { 0xBB, 0xBB, 0xBB, 255 }
#endif

#ifndef COLOR_STEP // #3761a1
#define COLOR_STEP { 0x37, 0x61, 0xa1, 255 }
#endif

#ifndef COLOR_STEP_WITH_CONDITION // #3791a1
#define COLOR_STEP_WITH_CONDITION { 0x37, 0x91, 0xa1, 255 }
#endif

#ifndef COLOR_STEP_HEADER // #2c343f
#define COLOR_STEP_HEADER { 0x2c, 0x34, 0x3f, 255 }
#endif

#ifndef COLOR_FREESOUND_HEADER // #2c343f
#define COLOR_FREESOUND_HEADER { 0x2c, 0x34, 0x3f, 255 }
#endif

#ifndef APP_DEFAULT_FONT_COLOR
#define APP_DEFAULT_FONT_COLOR COLOR_WHITE
#endif

#ifndef COLOR_BTN_X // #2a3696
#define COLOR_BTN_X { 0x2a, 0x36, 0x96, 255 }
#endif

#ifndef COLOR_BTN_Y // #03a115
#define COLOR_BTN_Y { 0x03, 0xa1, 0x15, 255 }
#endif

#ifndef COLOR_BTN_A // #ab0400
#define COLOR_BTN_A { 0xab, 0x04, 0x00, 255 }
#endif

#ifndef COLOR_BTN_B // #edbf02
#define COLOR_BTN_B { 0xed, 0xbf, 0x02, 255 }
#endif

// clang-format on

#ifndef APP_MAX_VOLUME
#define APP_MAX_VOLUME 2.0
#endif

#ifndef APP_TRACKS
#define APP_TRACKS 16
// #define APP_TRACKS 1
#endif

#ifndef APP_TRACK_STEPS
#define APP_TRACK_STEPS 32
#endif

#ifndef APP_TRACK_NAME
#define APP_TRACK_NAME 12
#endif

#ifndef APP_TRACK_LIST_MAX
#define APP_TRACK_LIST_MAX 1000
#endif

#ifndef APP_SAMPLE_NAME
#define APP_SAMPLE_NAME 255
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

#ifndef CLEAR
#define CLEAR true
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
    VIEW_MAIN,
    VIEW_FREESOUND,
    VIEW_COUNT,
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
    UI_KEY_EDIT2,
    UI_KEY_MODE,
    UI_KEY_ACTION,
    UI_KEY_COUNT
};

enum {
    UI_PAD_X1,
    UI_PAD_Y1,
    UI_PAD_X2,
    UI_PAD_Y2,
};

class UiKeys {
public:
    bool update = false;
    bool controllerDirectional = false;

    bool Up = false;
    bool Down = false;
    bool Right = false;
    bool Left = false;
    bool btnA = false;
    bool btnY = false;
    bool btnX = false;
    bool Mode = false;
    bool Menu = false;
    bool btnB = false;
    bool R1 = false;
    bool R2 = false;
    bool L1 = false;
    bool L2 = false;
    bool joystickRight = false;
    bool joystickLeft = false;

    bool isVertical()
    {
        return Up || Down;
    }

    bool isHorizontal()
    {
        return Left || Right;
    }

    float getVertical(float step = 1.0f)
    {
        return getVertical(step, step);
    }

    float getVertical(float step, float stepR1)
    {
        if (R1) {
            step = stepR1;
        }
        if (Up) {
            return step;
        } else if (Down) {
            return -step;
        }
        return 0.0f;
    }

    float getHorizontal(float step = 1.0f)
    {
        return getHorizontal(step, step);
    }

    float getHorizontal(float step, float stepR1)
    {
        if (R1) {
            step = stepR1;
        }
        if (Left) {
            return -step;
        } else if (Right) {
            return step;
        }
        return 0.0f;
    }

    float getDirection(float stepA, float stepB)
    {
        if (Right) {
            return stepA;
        } else if (Up) {
            return stepB;
        } else if (Left) {
            return -stepA;
        } else if (Down) {
            return -stepB;
        }
        return 0.0;
    }

    float getDirection(float step = 1.0)
    {
        return getDirection(step, step * 10);
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