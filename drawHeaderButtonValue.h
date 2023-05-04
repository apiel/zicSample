#ifndef _DRAW_HEADER_BUTTON_VALUE_H_
#define _DRAW_HEADER_BUTTON_VALUE_H_

#include "draw.h"

#include <vector>

class HeaderButtonValue {
protected:
    const uint32_t width = (SCREEN_W - 97) * 0.25;
    const uint32_t height = 60;
    const uint32_t halfHeight = height * 0.5 - 1;
    const Point posX = { 92, 5 };
    const Point posY = { posX.x + width + 2, posX.y };
    const Point posB = { posY.x, posX.y + halfHeight + 2 };
    const Point posA = { 2 + posY.x + width * 2, posX.y };

    void drawTriangle(Point position, SDL_Color color, const char* letter)
    {
        uint32_t size = 15;
        const std::vector<SDL_Vertex> verts = {
            {
                SDL_FPoint { position.x - size, position.y },
                color,
                SDL_FPoint { 0 },
            },
            {
                SDL_FPoint { position.x, position.y },
                color,
                SDL_FPoint { 0 },
            },
            {
                SDL_FPoint { position.x, position.y + size },
                color,
                SDL_FPoint { 0 },
            },
        };
        SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);

        drawText({ position.x - (uint32_t)(size * 0.5), position.y }, letter, COLOR_WHITE, 8);
    }

public:
    void drawY()
    {
        drawFilledRect(posX, { width, height }, COLOR_FOREGROUND2);
        drawTriangle({posX.x + width, posX.y }, COLOR_BTN_Y, "Y");
    }

    void drawX()
    {
        drawFilledRect(posY, { width * 2, halfHeight }, COLOR_FOREGROUND2);
        drawTriangle({ posY.x + width * 2, posY.y }, COLOR_BTN_X, "X");
    }

    void drawB()
    {
        drawFilledRect(posB, { width * 2, halfHeight }, COLOR_FOREGROUND2);
        drawTriangle({ posB.x + width * 2, posB.y }, COLOR_BTN_B, "B");
    }

    void drawA()
    {
        drawFilledRect(posA, { width, height }, COLOR_FOREGROUND2);
        drawTriangle({ posA.x + width, posA.y }, COLOR_BTN_A, "A");
    }

    void draw()
    {
        // FIXME remove this
        drawFilledRect(posX, { width * 4, height }, COLOR_BACKGROUND);

        drawY();
        drawX();
        drawB();
        drawA();
    }
};

#endif
