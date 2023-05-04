#ifndef _DRAW_HEADER_BUTTON_VALUE_H_
#define _DRAW_HEADER_BUTTON_VALUE_H_

#include "draw.h"

#include <vector>

class HeaderButtonValue {
protected:
    const uint32_t width = (SCREEN_W - 97) * 0.25;
    const uint32_t height = 60;
    const uint32_t halfHeight = height * 0.5 - 1;
    const uint32_t Yx = 92;
    const uint32_t Yy = 5;
    const uint32_t Xx = Yx + width + 2;
    const uint32_t Xy = Yy;
    const uint32_t Bx = Xx;
    const uint32_t By = Yy + halfHeight + 2;
    const uint32_t Ax = 2 + Xx + width * 2;
    const uint32_t Ay = Yy;

    void drawTriangle(uint32_t x, uint32_t y, SDL_Color color, const char *letter)
    {
        uint32_t size = 15;
        const std::vector<SDL_Vertex> verts = {
            {
                SDL_FPoint { x - size, y },
                color,
                SDL_FPoint { 0 },
            },
            {
                SDL_FPoint { x, y },
                color,
                SDL_FPoint { 0 },
            },
            {
                SDL_FPoint { x, y + size },
                color,
                SDL_FPoint { 0 },
            },
        };
        SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);

        drawText({ x - (uint32_t)(size * 0.5), y }, letter, COLOR_WHITE, 8);
    }

public:
    void drawY()
    {
        drawFilledRect({ Yx, Yy }, { width, height }, COLOR_FOREGROUND2);
        drawTriangle(Yx + width, Yy, COLOR_BTN_Y, "Y");
    }

    void drawX()
    {
        drawFilledRect({ Xx, Xy }, { width * 2, halfHeight }, COLOR_FOREGROUND2);
        drawTriangle(Xx + width * 2, Xy, COLOR_BTN_X, "X");
    }

    void drawB()
    {
        drawFilledRect({ Bx, By }, { width * 2, halfHeight }, COLOR_FOREGROUND2);
        drawTriangle(Bx + width * 2, By, COLOR_BTN_B, "B");
    }

    void drawA()
    {
        drawFilledRect({ Ax, Ay }, { width, height }, COLOR_FOREGROUND2);
        drawTriangle(Ax + width, Ay, COLOR_BTN_A, "A");
    }

    void draw()
    {
        // FIXME remove this
        drawFilledRect({ Yx, Yy }, { width * 4, height }, COLOR_BACKGROUND);

        drawY();
        drawX();
        drawB();
        drawA();
    }
};

#endif
